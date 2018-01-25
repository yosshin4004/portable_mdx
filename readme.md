# Portable mdx decoder

	X68k MXDRV music driver version 2.06+17 Rel.X5-S
		(c)1988-92 milk.,K.MAEKAWA, Missy.M, Yatsube

	Converted for Win32 [MXDRVg] V2.00a
		Copyright (C) 2000-2002 GORRY.

	X68Sound_src020615
		Copyright (C) m_puusan.

	Ported for 64bit environments
		Copyright (C) 2018 Yosshin.

# 解説

mdx デコーダーです。

GORRY 氏の MXDRVg V2.00b と、m_puusan 氏の X68Sound.dll（src 020615）のソース
コードを、他環境に移植しやすい形に改変したものです。

オリジナル版とは、以下の点が異なっています。

* 64 bit 環境に対応
* vc に加え gcc / clang でのビルドが可能
* デコーダーのインスタンスが複数生成可能
* dll でない
* OPM レジスタ値取得 API、キーオン取得 API が拡張されている
* mdx ファイルのパースを簡単に行えるユーティリティ関数が拡張されている

# 使い方
ドキュメントは用意されていません。サンプルコードをご参照ください。

* ## simple_mdx2wav
	MDX ファイルを wav ファイルに変換する簡易ツールの実装例です。
* ## simple_player
	MDX ファイルを演奏し、かつ現在の内部ステートをリアルタイムに可視化します。

# オリジナル版の入手元
* MXDRVg V2.00b  
  http://gorry.haun.org/mx/

* X68Sound.dll  
  http://mpuusan.web.fc2.com/x68sound/index.htm


# ライセンス
* ## Yosshin が制作したソースコード

	該当ファイル  
	* include/mxdrv_context.h
	* include/x68sound_context.h
	* include/mdx_util.h
	* src/mdx_util.cpp
	* examples/simple_mdx2wav/*
	* examples/simple_player/*

	Apache License Version 2.0 が適用されます。


* ## MXDRVg/MXDRV.X に由来するソースコード

	該当ファイル  
	* include/mxdrv.h
	* mxdrv/*

	MXDRVg を元に、Yosshin が改変を行ったものです。
	Yosshin による改変箇所のライセンスは、オリジナル版の MXDRVg/MXDRV.X に従う
	ものとします。

	MXDRVg は、X68K ネイティブ版である MXDRV.X をディスアセンブルしたソース
	コードを元に、GORRY 氏が C 言語にコンバートされたものです。
	MXDRV.X は、milk., K.MAEKAWA, Missy.M, Yatsube 氏によるプロダクトです。
	MXDRV.X/MXDRVg 由来のソースコードに適用されるライセンスは、明確な文書は存在
	しませんが、「X68的defaultのフリーソフトウェア扱い」になると考えられます。


* ## X68Sound.dll に由来するソースコード

	該当ファイル  
	* include/x68sound.h
	* x68sound/*

	X68Sound.dll のソースコードを元に、Yosshin が改変を行ったものです。
	Yosshin による改変箇所のライセンスは、オリジナル版の X68Sound.dll に従う
	ものとします。

	オリジナル版のライセンスは以下の通りです（readme.txt より抜粋）

		[使用許諾契約]
		・本ソースの改変および改変物の公開、自作ソフト等への組み込みおよびそのソフトの
		　配布は自由です。m_puusanへの報告は必要ありません。  
		・本ソフトウェアの使用または使用不能から生じるいかなる損害（利益やデータの損失、
		　その他の金銭的損失など）に関してm_puusanは一切責任を負わないこととします。 


* ## SDL2 に由来するファイル群

	該当ファイル
	* examples/SDL2-2.0.7/*

	zlib ライセンスが適用されます。  
	入手元 https://www.libsdl.org/index.php

