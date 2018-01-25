#include "mxdrv_config.h"

#if MXDRV_ENABLE_PORTABLE_CODE
	#include <stdio.h>
	#include <x68sound.h>
	#include <x68sound_context.h>
	#include "sound_iocs.h"
#else
	#include <stdio.h>
	#include <conio.h>

	#include "sound_iocs.h"
	#include "x68sound.h"
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
	#include "mxdrv_context.internal.h"
	#define AdpcmStat		(context->m_impl->m_AdpcmStat)
	#define OpmReg1B		(context->m_impl->m_OpmReg1B)
	#define DmaErrCode		(context->m_impl->m_DmaErrCode)
	#define Adpcmcot_adrs	(context->m_impl->m_Adpcmcot_adrs)
	#define Adpcmcot_len	(context->m_impl->m_Adpcmcot_len)
	#define OpmIntProc		(context->m_impl->m_OpmIntProc)
	#define OpmIntArg		(context->m_impl->m_OpmIntArg)
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
	// DMAに16bit値をビッグエンディアン(68オーダー)で書き込む
	#define	X68Sound_DmaPokeW(context,adrs,data)	(X68Sound_DmaPoke(context,(adrs), (unsigned char)((unsigned short)(data)>>8)), \
													 X68Sound_DmaPoke(context,(adrs)+1, (unsigned char)((unsigned short)(data))))
	// DMAに32bit値をビッグエンディアン(68オーダー)で書き込む
	#define	X68Sound_DmaPokeL(context,adrs,data)	(X68Sound_DmaPoke(context,(adrs),   (unsigned char)((unsigned int)(data)>>24)), \
													 X68Sound_DmaPoke(context,(adrs)+1, (unsigned char)((unsigned int)(data)>>16)), \
													 X68Sound_DmaPoke(context,(adrs)+2, (unsigned char)((unsigned int)(data)>>8)), \
													 X68Sound_DmaPoke(context,(adrs)+3, (unsigned char)((unsigned int)(data))))
	#define X68Sound_y(context,no,data)				(X68Sound_OpmReg(context,no),X68Sound_OpmPoke(context,data))
#endif


#if !MXDRV_ENABLE_PORTABLE_CODE
// 16bit値のバイトの並びを逆にして返す
unsigned short bswapw(unsigned short data) {
	__asm {
		mov ax,data
		ror ax,8
	}
}

// 32bit値のバイトの並びを逆にして返す
void *bswapl(void *adrs) {
	__asm {
		mov	eax,adrs
		bswap	eax
	}
}
#endif


#if MXDRV_ENABLE_PORTABLE_CODE
	/* MxdrvContextImpl に移動 */
#else
volatile unsigned char AdpcmStat=0; // $02:adpcmout $12:adpcmaot $22:adpcmlot $32:adpcmcot
volatile unsigned char OpmReg1B=0;  // OPM レジスタ $1B の内容
volatile unsigned char DmaErrCode = 0;

volatile unsigned char *Adpcmcot_adrs;
volatile int	Adpcmcot_len;
#endif

// OPMのBUSY待ち
#if MXDRV_ENABLE_PORTABLE_CODE
void OpmWait_(MxdrvContext *context) {
	while (X68Sound_OpmPeek(&context->m_impl->m_x68SoundContext) & 0x80);
}
#else
void OpmWait() {
	while (X68Sound_OpmPeek() & 0x80);
}
#endif

// IOCS _OPMSET ($68) の処理
// [引数]
//   int addr : OPMレジスタナンバー(0～255)
//   int data : データ(0～255)
#if MXDRV_ENABLE_PORTABLE_CODE
void _iocs_opmset(MxdrvContext *context, int addr, int data) {
#else
void _iocs_opmset(int addr, int data) {
#endif
	if (addr == 0x1B) {
		OpmReg1B = (OpmReg1B&0xC0)|(data&0x3F);
		data = OpmReg1B;
	}
#if MXDRV_ENABLE_PORTABLE_CODE
	OpmWait_(context);
	X68Sound_OpmReg(&context->m_impl->m_x68SoundContext, addr);
	OpmWait_(context);
	X68Sound_OpmPoke(&context->m_impl->m_x68SoundContext, data);
	context->m_impl->m_opmRegs[addr] = data&0xFF;
	context->m_impl->m_opmRegsUpdated[addr] = true;
	if (addr == 0x08) {
		context->m_impl->m_keyOnFlagsForFm[data & 7] = (data & 0x78) != 0;
		context->m_impl->m_logicalSumOfKeyOnFlagsForFm[data & 7] |= (data & 0x78) != 0;
	}
#else
	OpmWait();
	X68Sound_OpmReg(addr);
	OpmWait();
	X68Sound_OpmPoke(data);
#endif
}

// IOCS _OPMSNS ($69) の処理
// [戻り値]
//   bit 0 : タイマーAオーバーフローのとき1になる
//   bit 1 : タイマーBオーバーフローのとき1になる
//   bit 7 : 0ならばデータ書き込み可能
#if MXDRV_ENABLE_PORTABLE_CODE
int _iocs_opmsns(MxdrvContext *context) {
	return X68Sound_OpmPeek(&context->m_impl->m_x68SoundContext);
#else
int _iocs_opmsns() {
	return X68Sound_OpmPeek();
#endif
}

#if MXDRV_ENABLE_PORTABLE_CODE
	/* MxdrvContextImpl に移動 */
#else
void (CALLBACK *OpmIntProc)()=NULL;		// OPMのタイマー割り込み処理アドレス
#endif

// IOCS _OPMINTST ($6A) の処理
// [引数]
//   void *addr : 割り込み処理アドレス
//                0のときは割り込み禁止
// [戻り値]
//   割り込みが設定された場合は 0
//   既に割り込みが設定されている場合はその割り込み処理アドレスを返す
#if MXDRV_ENABLE_PORTABLE_CODE
intptr_t _iocs_opmintst(MxdrvContext *context, void *addr, void *arg) {
#else
int _iocs_opmintst(void *addr) {
#endif
	if (addr == 0) {				// 引数が0の時は割り込みを禁止する
		OpmIntProc = NULL;
#if MXDRV_ENABLE_PORTABLE_CODE
		OpmIntArg = NULL;
		X68Sound_OpmInt(&context->m_impl->m_x68SoundContext, OpmIntProc, OpmIntArg);
#else
		X68Sound_OpmInt(OpmIntProc);
#endif
		return 0;
	}
	if (OpmIntProc != NULL) {		// 既に設定されている場合は、その処理アドレスを返す
#if MXDRV_ENABLE_PORTABLE_CODE
		return (intptr_t)OpmIntProc;
#else
		return (int)OpmIntProc;
#endif
	}
#if MXDRV_ENABLE_PORTABLE_CODE
	OpmIntProc = (void (*)(void *))addr;
	OpmIntArg = arg;
	X68Sound_OpmInt(&context->m_impl->m_x68SoundContext, OpmIntProc, OpmIntArg);	// OPMの割り込み処理アドレスを設定
#else
	OpmIntProc = (void (CALLBACK *)())addr;
	X68Sound_OpmInt(OpmIntProc);	// OPMの割り込み処理アドレスを設定
#endif
	return 0;
}

// DMA転送終了割り込み処理ルーチン
#if MXDRV_ENABLE_PORTABLE_CODE
void DmaIntProc(void *arg) {
	MxdrvContext *context = (MxdrvContext *)arg;
#else
void CALLBACK DmaIntProc() {
#endif
#if MXDRV_ENABLE_PORTABLE_CODE
	if (AdpcmStat == 0x32 && (X68Sound_DmaPeek(&context->m_impl->m_x68SoundContext, 0x00)&0x40)!=0) {	// コンティニューモード時の処理
		X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x00, 0x40);	// BTCビットをクリア
#else
	if (AdpcmStat == 0x32 && (X68Sound_DmaPeek(0x00)&0x40)!=0) {	// コンティニューモード時の処理
		X68Sound_DmaPoke(0x00, 0x40);	// BTCビットをクリア
#endif
		if (Adpcmcot_len > 0) {
			int dmalen;
			dmalen = Adpcmcot_len;
			if (dmalen > 0xFF00) {	// 1度に転送できるバイト数は0xFF00
				dmalen = 0xFF00;
			}
#if MXDRV_ENABLE_PORTABLE_CODE
			X68Sound_DmaPokeL(&context->m_impl->m_x68SoundContext, 0x1C, MxdrvContext_ToOfs(context, Adpcmcot_adrs));	// BARに次のDMA転送アドレスをセット
			X68Sound_DmaPokeW(&context->m_impl->m_x68SoundContext, 0x1A, dmalen);	// BTCに次のDMA転送バイト数をセット
#else
			X68Sound_DmaPokeL(0x1C, Adpcmcot_adrs);	// BARに次のDMA転送アドレスをセット
			X68Sound_DmaPokeW(0x1A, dmalen);	// BTCに次のDMA転送バイト数をセット
#endif
			Adpcmcot_adrs += dmalen;
			Adpcmcot_len -= dmalen;

#if MXDRV_ENABLE_PORTABLE_CODE
			X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x07, 0x48);	// コンティニューオペレーション設定
#else
			X68Sound_DmaPoke(0x07, 0x48);	// コンティニューオペレーション設定
#endif
		}
		return;
	}
	if (!(AdpcmStat&0x80)) {
#if MXDRV_ENABLE_PORTABLE_CODE
		X68Sound_PpiCtrl(&context->m_impl->m_x68SoundContext, 0x01);	// ADPCM右出力OFF
		X68Sound_PpiCtrl(&context->m_impl->m_x68SoundContext, 0x03);	// ADPCM左出力OFF
		X68Sound_AdpcmPoke(&context->m_impl->m_x68SoundContext, 0x01);	// ADPCM再生動作停止
#else
		X68Sound_PpiCtrl(0x01);	// ADPCM右出力OFF
		X68Sound_PpiCtrl(0x03);	// ADPCM左出力OFF
		X68Sound_AdpcmPoke(0x01);	// ADPCM再生動作停止
#endif
	}
	AdpcmStat = 0;
#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x00, 0xFF);	// DMA CSR の全ビットをクリア
#else
	X68Sound_DmaPoke(0x00, 0xFF);	// DMA CSR の全ビットをクリア
#endif
}

// DMAエラー割り込み処理ルーチン
#if MXDRV_ENABLE_PORTABLE_CODE
void DmaErrIntProc(void *arg) {
	MxdrvContext *context = (MxdrvContext *)arg;
	DmaErrCode = X68Sound_DmaPeek(&context->m_impl->m_x68SoundContext, 0x01);	// エラーコードを DmaErrCode に保存

	X68Sound_PpiCtrl(&context->m_impl->m_x68SoundContext, 0x01);	// ADPCM右出力OFF
	X68Sound_PpiCtrl(&context->m_impl->m_x68SoundContext, 0x03);	// ADPCM左出力OFF
	X68Sound_AdpcmPoke(&context->m_impl->m_x68SoundContext, 0x01);	// ADPCM再生動作停止

	AdpcmStat = 0;
	X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x00, 0xFF);	// DMA CSR の全ビットをクリア
}
#else
void CALLBACK DmaErrIntProc() {
	DmaErrCode = X68Sound_DmaPeek(0x01);	// エラーコードを DmaErrCode に保存

	X68Sound_PpiCtrl(0x01);	// ADPCM右出力OFF
	X68Sound_PpiCtrl(0x03);	// ADPCM左出力OFF
	X68Sound_AdpcmPoke(0x01);	// ADPCM再生動作停止

	AdpcmStat = 0;
	X68Sound_DmaPoke(0x00, 0xFF);	// DMA CSR の全ビットをクリア
}
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
static const unsigned char	PANTBL[4] = {3, 1, 2, 0};
#else
unsigned char	PANTBL[4] = {3, 1, 2, 0};
#endif

// サンプリング周波数とPANを設定してDMA転送を開始するルーチン
// [引数]
//   unsigned short mode : サンプリング周波数*256+PAN
//   unsigned char ccr : DMA CCR に書き込むデータ
#if MXDRV_ENABLE_PORTABLE_CODE
void SetAdpcmMode(MxdrvContext *context, unsigned short mode, unsigned char ccr) {
#else
void SetAdpcmMode(unsigned short mode, unsigned char ccr) {
#endif
	if (mode >= 0x0200) {
		mode -= 0x0200;
		OpmReg1B &= 0x7F;	// ADPCMのクロックは8MHz
	} else {
		OpmReg1B |= 0x80;	// ADPCMのクロックは4MHz
	}
#if MXDRV_ENABLE_PORTABLE_CODE
	OpmWait_(context);
	X68Sound_OpmReg(&context->m_impl->m_x68SoundContext, 0x1B);
	OpmWait_(context);
	X68Sound_OpmPoke(&context->m_impl->m_x68SoundContext, OpmReg1B);	// ADPCMのクロック設定(8or4MHz)
#else
	OpmWait();
	X68Sound_OpmReg(0x1B);
	OpmWait();
	X68Sound_OpmPoke(OpmReg1B);	// ADPCMのクロック設定(8or4MHz)
#endif
	unsigned char ppireg;
	ppireg = ((mode>>6)&0x0C) | PANTBL[mode&3];
#if MXDRV_ENABLE_PORTABLE_CODE
	ppireg |= (X68Sound_PpiPeek(&context->m_impl->m_x68SoundContext)&0xF0);
	X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x07, ccr);	// DMA転送開始
	X68Sound_PpiPoke(&context->m_impl->m_x68SoundContext, ppireg);	// サンプリングレート＆PANをPPIに設定
#else
	ppireg |= (X68Sound_PpiPeek()&0xF0);
	X68Sound_DmaPoke(0x07, ccr);	// DMA転送開始
	X68Sound_PpiPoke(ppireg);	// サンプリングレート＆PANをPPIに設定
#endif
}

// _iocs_adpcmoutのメインルーチン
// [引数]
//   unsigned char stat : ADPCMを停止させずに続けてDMA転送を行う場合は$80
//                        DMA転送終了後ADPCMを停止させる場合は$00
//   unsigned short len : DMA転送バイト数
//   unsigned char *adrs : DMA転送アドレス
#if MXDRV_ENABLE_PORTABLE_CODE
void AdpcmoutMain(MxdrvContext *context, unsigned char stat, unsigned short mode, unsigned short len, unsigned char *adrs) {
#else
void AdpcmoutMain(unsigned char stat, unsigned short mode, unsigned short len, unsigned char *adrs) {
#endif
	while (AdpcmStat);	// DMA転送終了待ち
	AdpcmStat = stat+2;
#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x05, 0x32);	// DMA OCR をチェイン動作なしに設定

	X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x00, 0xFF);	// DMA CSR の全ビットをクリア
	X68Sound_DmaPokeL(&context->m_impl->m_x68SoundContext, 0x0C, MxdrvContext_ToOfs(context, adrs));	// DMA MAR にDMA転送アドレスをセット
	X68Sound_DmaPokeW(&context->m_impl->m_x68SoundContext, 0x0A, len);	// DMA MTC にDMA転送バイト数をセット
	SetAdpcmMode(context, mode, 0x88);	// サンプリング周波数とPANを設定してDMA転送開始

	X68Sound_AdpcmPoke(&context->m_impl->m_x68SoundContext, 0x02);	// ADPCM再生開始
#else
	X68Sound_DmaPoke(0x05, 0x32);	// DMA OCR をチェイン動作なしに設定

	X68Sound_DmaPoke(0x00, 0xFF);	// DMA CSR の全ビットをクリア
	X68Sound_DmaPokeL(0x0C, adrs);	// DMA MAR にDMA転送アドレスをセット
	X68Sound_DmaPokeW(0x0A, len);	// DMA MTC にDMA転送バイト数をセット
	SetAdpcmMode(mode, 0x88);	// サンプリング周波数とPANを設定してDMA転送開始

	X68Sound_AdpcmPoke(0x02);	// ADPCM再生開始
#endif
}

// IOCS _ADPCMOUT ($60) の処理
// [引数]
//   void *addr : ADPCMデータアドレス
//   int mode : サンプリング周波数(0～4)*256+PAN(0～3)
//   int len : ADPCMデータのバイト数
#if MXDRV_ENABLE_PORTABLE_CODE
void _iocs_adpcmout(MxdrvContext *context, void *addr, int mode, int len) {
#else
void _iocs_adpcmout(void *addr, int mode, int len) {
#endif
	int dmalen;
	unsigned char *dmaadrs = (unsigned char *)addr;
	while (AdpcmStat);	// DMA転送終了待ち
	while (len > 0x0000FF00) {	// ADPCMデータが0xFF00バイト以上の場合は
		dmalen = 0x0000FF00;	// 0xFF00バイトずつ複数回に分けてDMA転送を行う
#if MXDRV_ENABLE_PORTABLE_CODE
		AdpcmoutMain(context, 0x80,mode,dmalen,dmaadrs);
#else
		AdpcmoutMain(0x80,mode,dmalen,dmaadrs);
#endif
		dmaadrs += dmalen;
		len -= dmalen;
	}
#if MXDRV_ENABLE_PORTABLE_CODE
	AdpcmoutMain(context, 0x00,mode,len,dmaadrs);
#else
	AdpcmoutMain(0x00,mode,len,dmaadrs);
#endif
}

// IOCS _ADPCMAOT ($62) の処理
// [引数]
//   struct _chain *tbl : アレイチェインテーブルのアドレス
//   int mode : サンプリング周波数(0～4)*256+PAN(0～3)
//   int cnt : アレイチェインテーブルのブロック数
#if !MXDRV_ENABLE_PORTABLE_CODE
void _iocs_adpcmaot(struct _chain *tbl, int mode, int cnt) {
	while (AdpcmStat);	// DMA転送終了待ち

	AdpcmStat = 0x12;
	X68Sound_DmaPoke(0x05, 0x3A);	// DMA OCR をアレイチェイン動作に設定

	X68Sound_DmaPoke(0x00, 0xFF);	// DMA CSR の全ビットをクリア
	X68Sound_DmaPokeL(0x1C, tbl);	// DMA BAR にアレイチェインテーブルアドレスをセット
	X68Sound_DmaPokeW(0x1A, cnt);	// DMA BTC にアレイチェインテーブルの個数をセット
	SetAdpcmMode(mode, 0x88);	// サンプリング周波数とPANを設定してDMA転送開始

	X68Sound_AdpcmPoke(0x02);	// ADPCM再生開始
}
#endif

// IOCS _ADPCMAOT ($64) の処理
// [引数]
//   struct _chain2 *tbl : リンクアレイチェインテーブルのアドレス
//   int mode : サンプリング周波数(0～4)*256+PAN(0～3)
#if !MXDRV_ENABLE_PORTABLE_CODE
void _iocs_adpcmlot(struct _chain2 *tbl, int mode) {
	while (AdpcmStat);	// DMA転送終了待ち

	AdpcmStat = 0x22;
	X68Sound_DmaPoke(0x05, 0x3E);	// DMA OCR をリンクアレイチェイン動作に設定

	X68Sound_DmaPoke(0x00, 0xFF);	// DMA CSR の全ビットをクリア
	X68Sound_DmaPokeL(0x1C, tbl);	// DMA BAR にリンクアレイチェインテーブルアドレスをセット
	SetAdpcmMode(mode, 0x88);	// サンプリング周波数とPANを設定してDMA転送開始

	X68Sound_AdpcmPoke(0x02);	// ADPCM再生開始
}
#endif


// コンティニューモードを利用してADPCM出力を行うサンプル
// IOCS _ADPCMOUT と同じ処理を行うが、データバイト数が0xFF00バイト以上でも
// すぐにリターンする。
// [引数]
//   void *addr : ADPCMデータアドレス
//   int mode : サンプリング周波数(0～4)*256+PAN(0～3)
//   int len : ADPCMデータのバイト数
#if MXDRV_ENABLE_PORTABLE_CODE
void _iocs_adpcmcot(MxdrvContext *context, void *addr, int mode, int len) {
#else
void _iocs_adpcmcot(void *addr, int mode, int len) {
#endif
	int dmalen;
	Adpcmcot_adrs = (unsigned char *)addr;
	Adpcmcot_len = len;
	while (AdpcmStat);	// DMA転送終了待ち
	AdpcmStat = 0x32;

#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x05, 0x32);	// DMA OCR をチェイン動作なしに設定
#else
	X68Sound_DmaPoke(0x05, 0x32);	// DMA OCR をチェイン動作なしに設定
#endif

	dmalen = Adpcmcot_len;
	if (dmalen > 0xFF00) {	// ADPCMデータが0xFF00バイト以上の場合は
		dmalen = 0xFF00;	// 0xFF00バイトずつ複数回に分けてDMA転送を行う
	}

#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x00, 0xFF);	// DMA CSR の全ビットをクリア
	X68Sound_DmaPokeL(&context->m_impl->m_x68SoundContext, 0x0C, MxdrvContext_ToOfs(context, Adpcmcot_adrs));	// DMA MAR にDMA転送アドレスをセット
	X68Sound_DmaPokeW(&context->m_impl->m_x68SoundContext, 0x0A, dmalen);	// DMA MTC にDMA転送バイト数をセット
#else
	X68Sound_DmaPoke(0x00, 0xFF);	// DMA CSR の全ビットをクリア
	X68Sound_DmaPokeL(0x0C, Adpcmcot_adrs);	// DMA MAR にDMA転送アドレスをセット
	X68Sound_DmaPokeW(0x0A, dmalen);	// DMA MTC にDMA転送バイト数をセット
#endif
	Adpcmcot_adrs += dmalen;
	Adpcmcot_len -= dmalen;
	if (Adpcmcot_len <= 0) {
#if MXDRV_ENABLE_PORTABLE_CODE
		SetAdpcmMode(context, mode, 0x88);	// データバイト数が0xFF00以下の場合は通常転送
#else
		SetAdpcmMode(mode, 0x88);	// データバイト数が0xFF00以下の場合は通常転送
#endif
	} else {
		dmalen = Adpcmcot_len;
		if (dmalen > 0xFF00) {
			dmalen = 0xFF00;
		}
#if MXDRV_ENABLE_PORTABLE_CODE
		X68Sound_DmaPokeL(&context->m_impl->m_x68SoundContext, 0x1C, MxdrvContext_ToOfs(context, Adpcmcot_adrs));	// BARに次のDMA転送アドレスをセット
		X68Sound_DmaPokeW(&context->m_impl->m_x68SoundContext, 0x1A, dmalen);	// BTCに次のDMA転送バイト数をセット
#else
		X68Sound_DmaPokeL(0x1C, Adpcmcot_adrs);	// BARに次のDMA転送アドレスをセット
		X68Sound_DmaPokeW(0x1A, dmalen);	// BTCに次のDMA転送バイト数をセット
#endif
		Adpcmcot_adrs += dmalen;
		Adpcmcot_len -= dmalen;
#if MXDRV_ENABLE_PORTABLE_CODE
		SetAdpcmMode(context, mode, 0xC8);	// DMA CNTビットを1にしてDMA転送開始
#else
		SetAdpcmMode(mode, 0xC8);	// DMA CNTビットを1にしてDMA転送開始
#endif
	}

#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_AdpcmPoke(&context->m_impl->m_x68SoundContext, 0x02);	// ADPCM再生開始
#else
	X68Sound_AdpcmPoke(0x02);	// ADPCM再生開始
#endif
}

// IOCS _ADPCMSNS ($66) の処理
// [戻り値]
//   0 : 何もしていない
//   $02 : _iocs_adpcmout で出力中
//   $12 : _iocs_adpcmaot で出力中
//   $22 : _iocs_adpcmlot で出力中
//   $32 : _iocs_adpcmcot で出力中
#if MXDRV_ENABLE_PORTABLE_CODE
int _iocs_adpcmsns(MxdrvContext *context) {
#else
int _iocs_adpcmsns() {
#endif
	return (AdpcmStat&0x7F);
}

// IOCS _ADPCMMOD ($67) の処理
// [引数]
//   0 : ADPCM再生 終了
//   1 : ADPCM再生 一時停止
//   2 : ADPCM再生 再開
#if MXDRV_ENABLE_PORTABLE_CODE
void _iocs_adpcmmod(MxdrvContext *context, int mode) {
#else
void _iocs_adpcmmod(int mode) {
#endif
	switch (mode) {
	case 0:
		AdpcmStat = 0;
#if MXDRV_ENABLE_PORTABLE_CODE
		X68Sound_PpiCtrl(&context->m_impl->m_x68SoundContext, 0x01);	// ADPCM右出力OFF
		X68Sound_PpiCtrl(&context->m_impl->m_x68SoundContext, 0x03);	// ADPCM左出力OFF
		X68Sound_AdpcmPoke(&context->m_impl->m_x68SoundContext, 0x01);	// ADPCM再生動作停止
		X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x07, 0x10);	// DMA SAB=1 (ソフトウェアアボート)
#else
		X68Sound_PpiCtrl(0x01);	// ADPCM右出力OFF
		X68Sound_PpiCtrl(0x03);	// ADPCM左出力OFF
		X68Sound_AdpcmPoke(0x01);	// ADPCM再生動作停止
		X68Sound_DmaPoke(0x07, 0x10);	// DMA SAB=1 (ソフトウェアアボート)
#endif
		break;
	case 1:
#if MXDRV_ENABLE_PORTABLE_CODE
		X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x07, 0x20);	// DMA HLT=1 (ホルトオペレーション)
#else
		X68Sound_DmaPoke(0x07, 0x20);	// DMA HLT=1 (ホルトオペレーション)
#endif
		break;
	case 2:
#if MXDRV_ENABLE_PORTABLE_CODE
		X68Sound_DmaPoke(&context->m_impl->m_x68SoundContext, 0x07, 0x08);	// DMA HLT=0 (ホルトオペレーション解除)
#else
		X68Sound_DmaPoke(0x07, 0x08);	// DMA HLT=0 (ホルトオペレーション解除)
#endif
		break;
	}
}


// IOCSコールの初期化
// DMAの割り込みを設定する
#if MXDRV_ENABLE_PORTABLE_CODE
void sound_iocs_init(MxdrvContext *context) {
	X68Sound_DmaInt(&context->m_impl->m_x68SoundContext, DmaIntProc, context);
	X68Sound_DmaErrInt(&context->m_impl->m_x68SoundContext, DmaErrIntProc, context);
}
#else
void sound_iocs_init() {
	X68Sound_DmaInt(DmaIntProc);
	X68Sound_DmaErrInt(DmaErrIntProc);
}
#endif
