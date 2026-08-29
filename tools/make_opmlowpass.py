#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""OPM 出力用ポリフェーズ FIR 係数 (opmlowpass_*.dat) を生成する。

x68sound は OPM も ADPCM も常に 62500Hz (OPM クロック 4MHz / 64) で合成し、
最終段のポリフェーズ FIR だけで出力レートへ変換する (x68sound_opm.cpp の
pcmset62)。このスクリプトはその係数表を作る。

出力する .dat は 1 行 = 1 位相 (64 タップ) の C 初期化子で、
x68sound_global.h から #include される。

  位相数 ROW = 出力レート / gcd(62500, 出力レート)
    44100 -> 441   48000 -> 96   96000 -> 192

畳み込みは OpmFir_Normal (x68sound_global.h):

    result = sum(buf[k] * p[k] for k in 0..63) >> 15

buf[0] が最新の入力サンプルで、添字が増えるほど過去。したがって係数の
総和が 32768 のとき DC ゲインが 1.0 になる。

各位相の小数遅延は、pcmset62 のアキュムレータから導ける。出力 n 本目の
処理は

    OpmLPFidx += 62500
    while (OpmLPFidx >= WaveOutSamp): OpmLPFidx -= WaveOutSamp; 入力を 1 つ生成

なので、n 本目の直後は acc = ((n+1) * 62500) mod WaveOutSamp、
buf[0] が指す入力の番号は floor((n+1) * 62500 / WaveOutSamp) - 1。
出力時刻 n / WaveOutSamp を入力サンプル単位で測ると
n * 62500 / WaveOutSamp となるので、buf[0] からの遅延は

    (62500 - acc) / WaveOutSamp - 1   [+ 窓の中心へ寄せる定数]

同梱の 44100 / 48000 の表もこの式に一致する（実測の DC 群遅延で確認）。

使い方:
    python tools/make_opmlowpass.py 96000 > src/x68sound/opmlowpass_96.dat
"""

import math
import sys

# 内部の合成レート。OPM クロック 4MHz / 64。
SAMPRATE = 62500
# 1 位相あたりのタップ数。OpmFir_Normal が 64 個で展開されているので固定。
COL = 64
# 係数の固定小数。OpmFir が >> 15 するので総和 32768 で DC ゲイン 1.0。
SCALE = 1 << 15


def kaiser_beta(atten_db):
    """必要な阻止域減衰量から Kaiser 窓の β を求める（標準的な近似式）。"""
    if atten_db > 50.0:
        return 0.1102 * (atten_db - 8.7)
    if atten_db >= 21.0:
        return 0.5842 * (atten_db - 21.0) ** 0.4 + 0.07886 * (atten_db - 21.0)
    return 0.0


def kaiser_atten(taps, trans_hz):
    """タップ数と遷移帯域から得られる減衰量の目安（上の式の逆算）。"""
    dw = 2.0 * math.pi * trans_hz / SAMPRATE
    return taps * 2.285 * dw + 8.0


def bessel_i0(x):
    """第 1 種変形ベッセル関数 I0（級数展開）。"""
    total = 1.0
    term = 1.0
    k = 1
    while True:
        term *= (x / (2.0 * k)) ** 2
        total += term
        if term < 1e-16 * total:
            return total
        k += 1


def design(out_rate, pass_hz, stop_hz):
    """出力レートぶんの位相を並べた係数表を返す。"""
    fc = 0.5 * (pass_hz + stop_hz)  # -6dB 点
    beta = kaiser_beta(kaiser_atten(COL, stop_hz - pass_hz))

    step = math.gcd(SAMPRATE, out_rate)
    rows = out_rate // step

    # 位相ごとの小数遅延。窓（タップの真ん中 = 31.5）に寄せるための定数を
    # 足して、全位相を 64 タップの中央へ収める。
    def raw_delay(row):
        acc = ((row + 1) * SAMPRATE) % out_rate
        return (SAMPRATE - acc) / float(out_rate) - 1.0

    raws = [raw_delay(r) for r in range(rows)]
    center = (COL - 1) * 0.5 - 0.5 * (min(raws) + max(raws))

    # 窓の半幅。位相の小数遅延は d ∈ [center-0.5, center+0.5] に収まるので、
    # t = k - d は必ず [-half, +half] の中へ入る。
    half = COL * 0.5

    def kernel(t):
        """全位相が共有する 1 本の連続カーネル g(t)。

        **窓は sinc と同じ t で掛けること。** タップ列の中央 (31.5) を基準に
        窓を掛けると、位相ごとに窓と sinc の位置関係がずれて「1 本の連続
        カーネルを拾っている」という前提が崩れ、折り返し像の減衰が 20dB 以上
        悪化する（実測）。
        """
        if abs(t) > half:
            return 0.0
        x = 2.0 * fc * t / SAMPRATE
        sinc = 1.0 if abs(x) < 1e-12 else math.sin(math.pi * x) / (math.pi * x)
        u = t / half
        w = bessel_i0(beta * math.sqrt(max(0.0, 1.0 - u * u))) / bessel_i0(beta)
        return sinc * w

    table = []
    for r in range(rows):
        d = raws[r] + center
        coefs = [kernel(k - d) for k in range(COL)]

        # DC ゲインをぴったり 1.0 に揃える。位相ごとにゲインが揺れると
        # 出力レートの周期でうなりになるので、丸め誤差も潰しておく。
        total = sum(coefs)
        ints = [int(round(c * SCALE / total)) for c in coefs]
        err = SCALE - sum(ints)
        if err != 0:
            peak = max(range(COL), key=lambda i: abs(ints[i]))
            ints[peak] += err
        for v in ints:
            if not (-32768 <= v <= 32767):
                raise ValueError("係数が short に収まりません: %d" % v)
        table.append(ints)
    return table, fc, beta


def emit(table, path):
    """同梱の opmlowpass_44.dat / _48.dat と同じ体裁で書き出す
    （ASCII・CRLF・1 行 1 位相・"{\\t%5d,...},"）。"""
    with open(path, "wb") as fp:
        for row in table:
            line = "{\t" + "".join("%5d," % v for v in row) + "},\r\n"
            fp.write(line.encode("ascii"))


def main(argv):
    if len(argv) < 2:
        sys.stderr.write(__doc__)
        return 1
    out_path = argv[1]
    out_rate = int(argv[2]) if len(argv) > 2 else 96000
    # 通過端と阻止端。既定は 96000 用。62500Hz 入力からの補間なので折り返し像は
    # 入力ナイキスト (31250Hz) のすぐ上から現れる。阻止端をそこへ置いて、
    # 像の出る帯域を頭から潰す。通過端は可聴帯域よりずっと上に取ってある。
    pass_hz = int(argv[3]) if len(argv) > 3 else 26500
    stop_hz = int(argv[4]) if len(argv) > 4 else 31000
    table, fc, beta = design(out_rate, pass_hz, stop_hz)
    emit(table, out_path)
    sys.stderr.write(
        "%s: %d Hz / %d 位相 x %d タップ / 通過端 %d / 阻止端 %d / "
        "-6dB %.1f / Kaiser beta=%.3f\n"
        % (out_path, out_rate, len(table), COL, pass_hz, stop_hz, fc, beta))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
