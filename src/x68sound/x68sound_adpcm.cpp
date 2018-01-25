#include "x68sound_config.h"

#include "x68sound_global.h"
#include "x68sound_adpcm.h"
#include "x68sound_context.internal.h"

#if X68SOUND_ENABLE_PORTABLE_CODE
Adpcm::Adpcm(X68SoundContextImpl *contextImpl) {
	m_contextImpl = contextImpl;
#else
Adpcm::Adpcm(void) {
#endif
}

void Adpcm::SetAdpcmRate(int rate) {
	AdpcmRate = ADPCMRATEADDTBL[rate & 7];
}

#if X68SOUND_ENABLE_PORTABLE_CODE
static const unsigned char DmaRegInit[0x40] = {
#else
unsigned char DmaRegInit[0x40] = {
#endif
/*+00*/	0x00,0x00,	// CSR/CER
/*+02*/	0xFF,0xFF,
/*+04*/	0x80,0x32,	// DCR/OCR
/*+06*/	0x04,0x08,	// SCR/CCR
/*+08*/	0xFF,0xFF,
/*+0A*/	0x00,0x00,	// MTC
/*+0C*/	0x00,0x00,	// MAR
/*+0E*/	0x00,0x00,	// MAR
/*+10*/	0xFF,0xFF,
/*+12*/	0xFF,0xFF,
/*+14*/	0x00,0xE9,	// DAR
/*+16*/	0x20,0x03,	// DAR
/*+18*/	0xFF,0xFF,
/*+1A*/	0x00,0x00,	// BTC
/*+1C*/	0x00,0x00,	// BAR
/*+1E*/	0x00,0x00,	// BAR
/*+20*/	0xFF,0xFF,
/*+22*/	0xFF,0xFF,
/*+24*/	0xFF,0x6A,	// NIV
/*+26*/	0xFF,0x6B,	// EIV
/*+28*/	0xFF,0x05,	// MFC
/*+2A*/	0xFF,0xFF,
/*+2C*/	0xFF,0x01,	// CPR
/*+2E*/	0xFF,0xFF,
/*+30*/	0xFF,0x05,	// DFC
/*+32*/	0xFF,0xFF,
/*+34*/	0xFF,0xFF,
/*+36*/	0xFF,0xFF,
/*+38*/	0xFF,0x05,	// BFC
/*+3A*/	0xFF,0xFF,
/*+3C*/	0xFF,0xFF,
/*+3E*/	0xFF,0x00,	// GCR
};

void Adpcm::Init() {
	Scale = 0;
	Pcm = 0;
	InpPcm = InpPcm_prev = OutPcm = 0;
	OutInpPcm = OutInpPcm_prev = 0;
	AdpcmRate = 15625*12;
	RateCounter = 0;
	N1Data = 0;
	N1DataFlag = 0;
	IntProc = NULL;
#if X68SOUND_ENABLE_PORTABLE_CODE
	IntArg = NULL;
#endif
	ErrIntProc = NULL;
#if X68SOUND_ENABLE_PORTABLE_CODE
	ErrIntArg = NULL;
#endif
	DmaLastValue = 0;
	AdpcmReg = 0xC7;
	{
		int i;
		for (i=0; i<0x40; ++i) {
#if X68SOUND_ENABLE_PORTABLE_CODE
			DmaReg.asUint8[i] = DmaRegInit[i];
#else
			DmaReg[i] = DmaRegInit[i];
#endif
		}
	}
	FinishCounter = 3;
}
void Adpcm::InitSamprate() {
	RateCounter = 0;
}
void Adpcm::Reset() {	// ADPCM キーオン時の処理

	Scale = 0;

	Pcm = 0;
	InpPcm = InpPcm_prev = OutPcm = 0;
	OutInpPcm = OutInpPcm_prev = 0;

	N1Data = 0;
	N1DataFlag = 0;


}


void Adpcm::DmaError(unsigned char errcode) {
#if X68SOUND_ENABLE_PORTABLE_CODE
	DmaReg.asUint8[0x00] &= 0xF7;		// ACT=0
	DmaReg.asUint8[0x00] |= 0x90;		// COC=ERR=1
	DmaReg.asUint8[0x01] = errcode;		// CER=errorcode
	if (DmaReg.asUint8[0x07] & 0x08) {	// INT==1?
		if (ErrIntProc != NULL) {
			ErrIntProc(ErrIntArg);
#else
	DmaReg[0x00] &= 0xF7;		// ACT=0
	DmaReg[0x00] |= 0x90;		// COC=ERR=1
	DmaReg[0x01] = errcode;		// CER=errorcode
	if (DmaReg[0x07] & 0x08) {	// INT==1?
		if (ErrIntProc != NULL) {
			ErrIntProc();
#endif
		}
	}
}
void Adpcm::DmaFinish() {
#if X68SOUND_ENABLE_PORTABLE_CODE
	DmaReg.asUint8[0x00] &= 0xF7;		// ACT=0
	DmaReg.asUint8[0x00] |= 0x80;		// COC=1
	if (DmaReg.asUint8[0x07] & 0x08) {	// INT==1?
		if (IntProc != NULL) {
			IntProc(IntArg);
#else
	DmaReg[0x00] &= 0xF7;		// ACT=0
	DmaReg[0x00] |= 0x80;		// COC=1
	if (DmaReg[0x07] & 0x08) {	// INT==1?
		if (IntProc != NULL) {
			IntProc();
#endif
		}
	}
}

int Adpcm::DmaContinueSetNextMtcMar() {
#if X68SOUND_ENABLE_PORTABLE_CODE
	DmaReg.asUint8[0x07] &= (0xFF-0x40);	// CNT=0
	DmaReg.asUint16[0x0A/2] = DmaReg.asUint16[0x1A/2];	// BTC -> MTC
	DmaReg.asUint32[0x0C/4] = DmaReg.asUint32[0x1C/4];	// BAR -> MAR
	DmaReg.asUint8[0x29] = DmaReg.asUint8[0x39];	// BFC -> MFC
	if ( (DmaReg.asUint16[0x0A/2]) == 0 ) {	// MTC == 0 ?
		DmaError(0x0D);	// カウントエラー(メモリアドレス/メモリカウンタ)
		return 1;
	}
	DmaReg.asUint8[0x00] |= 0x40;		// BTC=1
	if (DmaReg.asUint8[0x07] & 0x08) {	// INT==1?
		if (IntProc != NULL) {
			IntProc(IntArg);
		}
	}
#else
	DmaReg[0x07] &= (0xFF-0x40);	// CNT=0
	*(unsigned short *)&DmaReg[0x0A] = *(unsigned short *)&DmaReg[0x1A];	// BTC -> MTC
	*(unsigned int *)&DmaReg[0x0C] = *(unsigned int *)&DmaReg[0x1C];	// BAR -> MAR
	DmaReg[0x29] = DmaReg[0x39];	// BFC -> MFC
	if ( (*(unsigned short *)&DmaReg[0x0A]) == 0 ) {	// MTC == 0 ?
		DmaError(0x0D);	// カウントエラー(メモリアドレス/メモリカウンタ)
		return 1;
	}
	DmaReg[0x00] |= 0x40;		// BTC=1
	if (DmaReg[0x07] & 0x08) {	// INT==1?
		if (IntProc != NULL) {
			IntProc();
		}
	}
#endif
	return 0;
}
int Adpcm::DmaArrayChainSetNextMtcMar() {
	unsigned short	Btc;
#if X68SOUND_ENABLE_PORTABLE_CODE
	Btc = bswapw(DmaReg.asUint16[0x1A/2]);
#else
	Btc = bswapw(*(unsigned short *)&DmaReg[0x1A]);
#endif
	if ( Btc == 0 ) {
		DmaFinish();
		FinishCounter = 0;
		return 1;
	}
	--Btc;
#if X68SOUND_ENABLE_PORTABLE_CODE
	DmaReg.asUint16[0x1A/2] = bswapw(Btc);
#else
	*(unsigned short *)&DmaReg[0x1A] = bswapw(Btc);
#endif

	unsigned char *Bar;
#if X68SOUND_ENABLE_PORTABLE_CODE
	Bar = TO_PTR(bswapl(DmaReg.asUint32[0x1C/4]));
#else
	Bar = bswapl(*(unsigned char **)&DmaReg[0x1C]);
#endif
	int	mem0,mem1,mem2,mem3,mem4,mem5;
	mem0 = MemRead(Bar++);
	mem1 = MemRead(Bar++);
	mem2 = MemRead(Bar++);
	mem3 = MemRead(Bar++);
	mem4 = MemRead(Bar++);
	mem5 = MemRead(Bar++);
	if ((mem0|mem1|mem2|mem3|mem4|mem5) == -1) {
		DmaError(0x0B);		// バスエラー(ベースアドレス/ベースカウンタ)
		return 1;
	} 
#if X68SOUND_ENABLE_PORTABLE_CODE
	DmaReg.asUint32[0x1C/4] = bswapl(TO_OFS(Bar));
	DmaReg.asUint8[0x0C] = (uint8_t)mem0;	// MAR
	DmaReg.asUint8[0x0D] = (uint8_t)mem1;
	DmaReg.asUint8[0x0E] = (uint8_t)mem2;
	DmaReg.asUint8[0x0F] = (uint8_t)mem3;
	DmaReg.asUint8[0x0A] = (uint8_t)mem4;	// MTC
	DmaReg.asUint8[0x0B] = (uint8_t)mem5;
#else
	*(unsigned char **)&DmaReg[0x1C] = bswapl(Bar);
	DmaReg[0x0C] = mem0;	// MAR
	DmaReg[0x0D] = mem1;
	DmaReg[0x0E] = mem2;
	DmaReg[0x0F] = mem3;
	DmaReg[0x0A] = mem4;	// MTC
	DmaReg[0x0B] = mem5;
#endif

#if X68SOUND_ENABLE_PORTABLE_CODE
	if ( (DmaReg.asUint16[0x0A/2]) == 0 ) {	// MTC == 0 ?
#else
	if ( (*(unsigned short *)&DmaReg[0x0A]) == 0 ) {	// MTC == 0 ?
#endif
		DmaError(0x0D);		// カウントエラー(メモリアドレス/メモリカウンタ)
		return 1;
	}
	return 0;
}
int Adpcm::DmaLinkArrayChainSetNextMtcMar() {
	unsigned char *Bar;
#if X68SOUND_ENABLE_PORTABLE_CODE
	Bar = TO_PTR(bswapl(DmaReg.asUint32[0x1C/4]));
#else
	Bar = bswapl(*(unsigned char **)&DmaReg[0x1C]);
#endif
	if (Bar == (unsigned char *)0) {
		DmaFinish();
		FinishCounter = 0;
		return 1;
	}

	int	mem0,mem1,mem2,mem3,mem4,mem5;
	int mem6,mem7,mem8,mem9;
	mem0 = MemRead(Bar++);
	mem1 = MemRead(Bar++);
	mem2 = MemRead(Bar++);
	mem3 = MemRead(Bar++);
	mem4 = MemRead(Bar++);
	mem5 = MemRead(Bar++);
	mem6 = MemRead(Bar++);
	mem7 = MemRead(Bar++);
	mem8 = MemRead(Bar++);
	mem9 = MemRead(Bar++);
	if ((mem0|mem1|mem2|mem3|mem4|mem5|mem6|mem7|mem8|mem9) == -1) {
		DmaError(0x0B);		// バスエラー(ベースアドレス/ベースカウンタ)
		return 1;
	} 
#if X68SOUND_ENABLE_PORTABLE_CODE
	DmaReg.asUint32[0x1C/4] = bswapl(TO_OFS(Bar));
	DmaReg.asUint8[0x0C] = (uint8_t)mem0;	// MAR
	DmaReg.asUint8[0x0D] = (uint8_t)mem1;
	DmaReg.asUint8[0x0E] = (uint8_t)mem2;
	DmaReg.asUint8[0x0F] = (uint8_t)mem3;
	DmaReg.asUint8[0x0A] = (uint8_t)mem4;	// MTC
	DmaReg.asUint8[0x0B] = (uint8_t)mem5;
	DmaReg.asUint8[0x1C] = (uint8_t)mem6;	// BAR
	DmaReg.asUint8[0x1D] = (uint8_t)mem7;
	DmaReg.asUint8[0x1E] = (uint8_t)mem8;
	DmaReg.asUint8[0x1F] = (uint8_t)mem9;
#else
	*(unsigned char **)&DmaReg[0x1C] = bswapl(Bar);
	DmaReg[0x0C] = mem0;	// MAR
	DmaReg[0x0D] = mem1;
	DmaReg[0x0E] = mem2;
	DmaReg[0x0F] = mem3;
	DmaReg[0x0A] = mem4;	// MTC
	DmaReg[0x0B] = mem5;
	DmaReg[0x1C] = mem6;	// BAR
	DmaReg[0x1D] = mem7;
	DmaReg[0x1E] = mem8;
	DmaReg[0x1F] = mem9;
#endif

#if X68SOUND_ENABLE_PORTABLE_CODE
	if ( DmaReg.asUint16[0x0A/2] == 0 ) {	// MTC == 0 ?
#else
	if ( (*(unsigned short *)&DmaReg[0x0A]) == 0 ) {	// MTC == 0 ?
#endif
		DmaError(0x0D);		// カウントエラー(メモリアドレス/メモリカウンタ)
		return 1;
	}
	return 0;
}

#if X68SOUND_ENABLE_PORTABLE_CODE
static const int MACTBL[4] = {0,1,-1,1};
#else
int	MACTBL[4] = {0,1,-1,1};
#endif

int	Adpcm::DmaGetByte() {
#if X68SOUND_ENABLE_PORTABLE_CODE
	if ((!(DmaReg.asUint8[0x00]&0x08)) || (DmaReg.asUint8[0x07]&0x20)) {	// ACT==0 || HLT==1 ?
#else
	if ((!(DmaReg[0x00]&0x08)) || (DmaReg[0x07]&0x20)) {	// ACT==0 || HLT==1 ?
#endif
		return 0x80000000;
	}
	unsigned short	Mtc;
#if X68SOUND_ENABLE_PORTABLE_CODE
	Mtc = bswapw(DmaReg.asUint16[0x0A/2]);
#else
	Mtc = bswapw(*(unsigned short *)&DmaReg[0x0A]);
#endif
	if (Mtc == 0) {
//		if (DmaReg[0x07] & 0x40) {	// Continue動作
//			if (DmaContinueSetNextMtcMar()) {
//				return 0x80000000;
//			}
//			Mtc = bswapw(*(unsigned short *)&DmaReg[0x0A]);
//		} else {
			return 0x80000000;
//		}
	}
	
	
	{
		unsigned char *Mar;
#if X68SOUND_ENABLE_PORTABLE_CODE
		Mar = TO_PTR(bswapl(DmaReg.asUint32[0x0C/4]));
#else
		Mar = bswapl(*(unsigned char **)&DmaReg[0x0C]);
#endif
		int mem;
		mem = MemRead(Mar);
		if (mem == -1) {
			DmaError(0x09);	// バスエラー(メモリアドレス/メモリカウンタ)
			return 0x80000000;
		}
		DmaLastValue = mem;
#if X68SOUND_ENABLE_PORTABLE_CODE
		Mar += MACTBL[(DmaReg.asUint8[0x06]>>2)&3];
		DmaReg.asUint32[0x0C/4] = bswapl(TO_OFS(Mar));
#else
		Mar += MACTBL[(DmaReg[0x06]>>2)&3];
		*(unsigned char **)&DmaReg[0x0C] = bswapl(Mar);
#endif
	}

	--Mtc;
#if X68SOUND_ENABLE_PORTABLE_CODE
	DmaReg.asUint16[0x0A/2] = bswapw(Mtc);
#else
	*(unsigned short *)&DmaReg[0x0A] = bswapw(Mtc);
#endif

#if X68SOUND_ENABLE_PORTABLE_CODE
	do {
		if (Mtc == 0) {
			if (DmaReg.asUint8[0x07] & 0x40) {	// Continue動作
				if (DmaContinueSetNextMtcMar()) {
					break;
				}
			} else if (DmaReg.asUint8[0x05] & 0x08) {	// チェイニング動作
				if (!(DmaReg.asUint8[0x05] & 0x04)) {	// アレイチェイン
					if (DmaArrayChainSetNextMtcMar()) {
						break;
					}
				} else {						// リンクアレイチェイン
					if (DmaLinkArrayChainSetNextMtcMar()) {
						break;
					}
				}
			} else {	// ノーマル転送終了
//				if (!(DmaReg[0x00] & 0x40)) {		// BTC=1 ?
//					if (DmaContinueSetNextMtcMar()) {
//						break;
//					}
//				} else {
					DmaFinish();
					FinishCounter = 0;
//				}
			}
		}
	} while(0);
#else
	try {
	if (Mtc == 0) {
		if (DmaReg[0x07] & 0x40) {	// Continue動作
			if (DmaContinueSetNextMtcMar()) {
				throw "";
			}
		} else if (DmaReg[0x05] & 0x08) {	// チェイニング動作
			if (!(DmaReg[0x05] & 0x04)) {	// アレイチェイン
				if (DmaArrayChainSetNextMtcMar()) {
					throw "";
				}
			} else {						// リンクアレイチェイン
				if (DmaLinkArrayChainSetNextMtcMar()) {
					throw "";
				}
			}
		} else {	// ノーマル転送終了
//			if (!(DmaReg[0x00] & 0x40)) {		// BTC=1 ?
//				if (DmaContinueSetNextMtcMar()) {
//					throw "";
//				}
//			} else {
				DmaFinish();
				FinishCounter = 0;
//			}
		}
	}
	} catch (void *) {
	}
#endif
	
	return DmaLastValue;
}








#define	MAXPCMVAL	(2047)

// adpcmを入力して InpPcm の値を変化させる
// -2047<<(4+4) <= InpPcm <= +2047<<(4+4)
void	Adpcm::adpcm2pcm(unsigned char adpcm) {

	
	int	dltL;
	dltL = dltLTBL[Scale];
	dltL = (dltL&(adpcm&4?-1:0)) + ((dltL>>1)&(adpcm&2?-1:0)) + ((dltL>>2)&(adpcm&1?-1:0)) + (dltL>>3);
	int sign = adpcm&8?-1:0;
	dltL = (dltL^sign)+(sign&1);
	Pcm += dltL;
	

	if ((unsigned int)(Pcm+MAXPCMVAL) > (unsigned int)(MAXPCMVAL*2)) {
		if ((int)(Pcm+MAXPCMVAL) >= (int)(MAXPCMVAL*2)) {
			Pcm = MAXPCMVAL;
		} else {
			Pcm = -MAXPCMVAL;
		}
	}

	InpPcm = (Pcm&(int)0xFFFFFFFC)<<(4+4);

	Scale += DCT[adpcm];
	if ((unsigned int)Scale > (unsigned int)48) {
		if ((int)Scale >= (int)48) {
			Scale = 48;
		} else {
			Scale = 0;
		}
	}
}

// -32768<<4 <= retval <= +32768<<4
int Adpcm::GetPcm() {
	if (AdpcmReg & 0x80) {		// ADPCM 停止中
		return 0x80000000;
	}
	RateCounter -= AdpcmRate;
	while (RateCounter < 0) {
		int	N10Data;	// (N1Data << 4) | N0Data
		if (N1DataFlag == 0) {		// 次のADPCMデータが内部にない場合
			N10Data = DmaGetByte();	// DMA転送(1バイト)
#if X68SOUND_ENABLE_PORTABLE_CODE
			if (N10Data == (int)0x80000000) {
#else
			if (N10Data == 0x80000000) {
#endif
				RateCounter = 0;
				return 0x80000000;
			}
			adpcm2pcm(N10Data & 0x0F);	// InpPcm に値が入る
			N1Data = (N10Data >> 4) & 0x0F;
			N1DataFlag = 1;
		} else {
			adpcm2pcm(N1Data);			// InpPcm に値が入る
			N1DataFlag = 0;
		}
		RateCounter += 15625*12;
	}
	OutPcm = ((InpPcm<<9) - (InpPcm_prev<<9) + 459*OutPcm) >> 9;
	InpPcm_prev = InpPcm;

	return (OutPcm*TotalVolume)>>8;
}

// -32768<<4 <= retval <= +32768<<4
int Adpcm::GetPcm62() {
	if (AdpcmReg & 0x80) {		// ADPCM 停止中
		return 0x80000000;
	}
	RateCounter -= AdpcmRate;
	while (RateCounter < 0) {
		int	N10Data;	// (N1Data << 4) | N0Data
		if (N1DataFlag == 0) {		// 次のADPCMデータが内部にない場合
			N10Data = DmaGetByte();	// DMA転送(1バイト)
#if X68SOUND_ENABLE_PORTABLE_CODE
			if (N10Data == (int)0x80000000) {
#else
			if (N10Data == 0x80000000) {
#endif
				RateCounter = 0;
				return 0x80000000;
			}
			adpcm2pcm(N10Data & 0x0F);	// InpPcm に値が入る
			N1Data = (N10Data >> 4) & 0x0F;
			N1DataFlag = 1;
		} else {
			adpcm2pcm(N1Data);			// InpPcm に値が入る
			N1DataFlag = 0;
		}
		RateCounter += 15625*12*4;

	}
	OutInpPcm = (InpPcm<<9) - (InpPcm_prev<<9) +  OutInpPcm-(OutInpPcm>>5)-(OutInpPcm>>10);
	InpPcm_prev = InpPcm;
	OutPcm = OutInpPcm - OutInpPcm_prev + OutPcm-(OutPcm>>8)-(OutPcm>>9)-(OutPcm>>12);
	OutInpPcm_prev = OutInpPcm;
	return (OutPcm>>9);
}

