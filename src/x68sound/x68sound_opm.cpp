#include <math.h>
#include <x68sound_context.h>
#include "x68sound_config.h"
#include "x68sound_global.h"
#include "x68sound_opm.h"
#include "x68sound.h"
#include "x68sound_context.internal.h"


void Opm::SetAdpcmRate() {
	adpcm.SetAdpcmRate(ADPCMRATETBL[AdpcmBaseClock][(PpiReg>>2)&3]);
}




void Opm::SetConnection(int ch, int alg) {
	switch (alg) {
	case 0:
		op[ch][0].out = &op[ch][1].inp;
		op[ch][0].out2 = &OpOutDummy;
		op[ch][0].out3 = &OpOutDummy;
		op[ch][1].out = &op[ch][2].inp;
		op[ch][2].out = &op[ch][3].inp;
		op[ch][3].out = &OpOut[ch];
		break;
	case 1:
		op[ch][0].out = &op[ch][2].inp;
		op[ch][0].out2 = &OpOutDummy;
		op[ch][0].out3 = &OpOutDummy;
		op[ch][1].out = &op[ch][2].inp;
		op[ch][2].out = &op[ch][3].inp;
		op[ch][3].out = &OpOut[ch];
		break;
	case 2:
		op[ch][0].out = &op[ch][3].inp;
		op[ch][0].out2 = &OpOutDummy;
		op[ch][0].out3 = &OpOutDummy;
		op[ch][1].out = &op[ch][2].inp;
		op[ch][2].out = &op[ch][3].inp;
		op[ch][3].out = &OpOut[ch];
		break;
	case 3:
		op[ch][0].out = &op[ch][1].inp;
		op[ch][0].out2 = &OpOutDummy;
		op[ch][0].out3 = &OpOutDummy;
		op[ch][1].out = &op[ch][3].inp;
		op[ch][2].out = &op[ch][3].inp;
		op[ch][3].out = &OpOut[ch];
		break;
	case 4:
		op[ch][0].out = &op[ch][1].inp;
		op[ch][0].out2 = &OpOutDummy;
		op[ch][0].out3 = &OpOutDummy;
		op[ch][1].out = &OpOut[ch];
		op[ch][2].out = &op[ch][3].inp;
		op[ch][3].out = &OpOut[ch];
		break;
	case 5:
		op[ch][0].out = &op[ch][1].inp;
		op[ch][0].out2 = &op[ch][2].inp;
		op[ch][0].out3 = &op[ch][3].inp;
		op[ch][1].out = &OpOut[ch];
		op[ch][2].out = &OpOut[ch];
		op[ch][3].out = &OpOut[ch];
		break;
	case 6:
		op[ch][0].out = &op[ch][1].inp;
		op[ch][0].out2 = &OpOutDummy;
		op[ch][0].out3 = &OpOutDummy;
		op[ch][1].out = &OpOut[ch];
		op[ch][2].out = &OpOut[ch];
		op[ch][3].out = &OpOut[ch];
		break;
	case 7:
		op[ch][0].out = &OpOut[ch];
		op[ch][0].out2 = &OpOutDummy;
		op[ch][0].out3 = &OpOutDummy;
		op[ch][1].out = &OpOut[ch];
		op[ch][2].out = &OpOut[ch];
		op[ch][3].out = &OpOut[ch];
		break;
	}
}


int Opm::SetOpmWait(int wait) {
	if (wait != -1) {
		OpmWait = wait;
		CulcCmndRate();
	}
	return OpmWait;
}
void Opm::CulcCmndRate() {
	if (OpmWait != 0) {
		CmndRate = (4096*160/OpmWait);
		if (CmndRate == 0) {
			CmndRate = 1;
		}
	} else {
		CmndRate = 4096*CMNDBUFSIZE;
	}
}

void Opm::Reset() {
	// OPMコマンドバッファを初期化
	NumCmnd = 0;
	CmndReadIdx = CmndWriteIdx = 0;

	CulcCmndRate();
	
	// 高音フィルター用バッファをクリア
	InpInpOpm[0] = InpInpOpm[1] =
	InpInpOpm_prev[0] = InpInpOpm_prev[1] = 0;
	InpInpOpm_prev2[0] = InpInpOpm_prev2[1] = 0;
	InpOpm[0] = InpOpm[1] =
	InpOpm_prev[0] = InpOpm_prev[1] =
	InpOpm_prev2[0] = InpOpm_prev2[1] =
	OutOpm[0] = OutOpm[1] = 0;
	{
#if X68SOUND_ENABLE_PORTABLE_CODE
		int i;
#else
		int i,j;
#endif
		for (i=0; i<OPMLPF_COL*2; ++i) {
			InpOpmBuf0[i]=InpOpmBuf1[i]=0;
		}
		InpOpm_idx = 0;
		OpmLPFidx = 0;
		OpmLPFp = OPMLOWPASS[0];
	}
	OpmHpfInp[0] = OpmHpfInp[1] =
	OpmHpfInp_prev[0] = OpmHpfInp_prev[1] =
	OpmHpfOut[0] = OpmHpfOut[1] = 0;
/*	{
		int i,j;
		for (i=0; i<ADPCMLPF_COL*2; ++i) {
			InpAdpcmBuf0[i]=InpAdpcmBuf1[i]=0;
		}
		InpAdpcm_idx = 0;
		AdpcmLPFidx = 0;
		AdpcmLPFp = ADPCMLOWPASS[0];
	}
*/
	OutInpAdpcm[0] = OutInpAdpcm[1] =
	OutInpAdpcm_prev[0] = OutInpAdpcm_prev[1] =
	OutInpAdpcm_prev2[0] = OutInpAdpcm_prev2[1] =
	OutOutAdpcm[0] = OutOutAdpcm[1] =
	OutOutAdpcm_prev[0] = OutOutAdpcm_prev[1] =
	OutOutAdpcm_prev2[0] = OutOutAdpcm_prev2[1] =
	0;
	OutInpOutAdpcm[0] = OutInpOutAdpcm[1] =
	OutInpOutAdpcm_prev[0] = OutInpOutAdpcm_prev[1] =
	OutInpOutAdpcm_prev2[0] = OutInpOutAdpcm_prev2[1] =
	OutOutInpAdpcm[0] = OutOutInpAdpcm[1] =
	OutOutInpAdpcm_prev[0] = OutOutInpAdpcm_prev[1] =
	0;

	// 全オペレータを初期化
	{
		int	ch;
		for (ch=0; ch<N_CH; ++ch) {
			op[ch][0].Init();
			op[ch][1].Init();
			op[ch][2].Init();
			op[ch][3].Init();
//			con[ch] = 0;
			SetConnection(ch,0);
			pan[0][ch] = pan[1][ch] = 0;
		}
	}
	
	// エンベロープ用カウンタを初期化
	{
		EnvCounter1 = 0;
		EnvCounter2 = 3;
	}


	// LFO初期化
	lfo.Init();

	// PcmBufポインターをリセット
	PcmBufPtr=0;
//	PcmBufSize = PCMBUFSIZE;


	// タイマー関係の初期化
	TimerAreg10 = 0;
	TimerAreg11 = 0;
	TimerA = 1024-0;
	TimerAcounter = 0;
	TimerB = (256-0) << (10-6);
	TimerBcounter = 0;
	TimerReg = 0;
	StatReg = 0;
	OpmIntProc = NULL;
#if X68SOUND_ENABLE_PORTABLE_CODE
	OpmIntArg = NULL;
#endif

	PpiReg = 0x0B;
	AdpcmBaseClock = 0;


	adpcm.Init();

	{
		int	i;
		for (i=0; i<PCM8_NCH; ++i) {
			pcm8[i].Init();
		}
	}

	TotalVolume = 256;
//	TotalVolume = 192;


	OpmRegNo = 0;
	BetwIntProc = NULL;
#if X68SOUND_ENABLE_PORTABLE_CODE
	BetwIntArg = NULL;
#endif
	WaveFunc = NULL;
#if X68SOUND_ENABLE_PORTABLE_CODE
	WaveFuncArg = NULL;
#endif

	MemRead = MemReadDefault;


//	UseOpmFlag = 0;
//	UseAdpcmFlag = 0;
}
void Opm::ResetSamprate() {
	CulcCmndRate();

	// 高音フィルター用バッファをクリア
	InpInpOpm[0] = InpInpOpm[1] =
	InpInpOpm_prev[0] = InpInpOpm_prev[1] = 0;
	InpInpOpm_prev2[0] = InpInpOpm_prev2[1] = 0;
	InpOpm[0] = InpOpm[1] =
	InpOpm_prev[0] = InpOpm_prev[1] =
	InpOpm_prev2[0] = InpOpm_prev2[1] =
	OutOpm[0] = OutOpm[1] = 0;
	{
#if X68SOUND_ENABLE_PORTABLE_CODE
		int i;
#else
		int i,j;
#endif
		for (i=0; i<OPMLPF_COL*2; ++i) {
			InpOpmBuf0[i]=InpOpmBuf1[i]=0;
		}
		InpOpm_idx = 0;
		OpmLPFidx = 0;
		OpmLPFp = OPMLOWPASS[0];
	}
	OpmHpfInp[0] = OpmHpfInp[1] =
	OpmHpfInp_prev[0] = OpmHpfInp_prev[1] =
	OpmHpfOut[0] = OpmHpfOut[1] = 0;
/*	{
		int i,j;
		for (i=0; i<ADPCMLPF_COL*2; ++i) {
			InpAdpcmBuf0[i]=InpAdpcmBuf1[i]=0;
		}
		InpAdpcm_idx = 0;
		AdpcmLPFidx = 0;
		AdpcmLPFp = ADPCMLOWPASS[0];
	}
*/
	OutInpAdpcm[0] = OutInpAdpcm[1] =
	OutInpAdpcm_prev[0] = OutInpAdpcm_prev[1] =
	OutInpAdpcm_prev2[0] = OutInpAdpcm_prev2[1] =
	OutOutAdpcm[0] = OutOutAdpcm[1] =
	OutOutAdpcm_prev[0] = OutOutAdpcm_prev[1] =
	OutOutAdpcm_prev2[0] = OutOutAdpcm_prev2[1] =
	0;
	OutInpOutAdpcm[0] = OutInpOutAdpcm[1] =
	OutInpOutAdpcm_prev[0] = OutInpOutAdpcm_prev[1] =
	OutInpOutAdpcm_prev2[0] = OutInpOutAdpcm_prev2[1] =
	OutOutInpAdpcm[0] = OutOutInpAdpcm[1] =
	OutOutInpAdpcm_prev[0] = OutOutInpAdpcm_prev[1] =
	0;

	// 全オペレータを初期化
	{
		int	ch;
		for (ch=0; ch<N_CH; ++ch) {
			op[ch][0].InitSamprate();
			op[ch][1].InitSamprate();
			op[ch][2].InitSamprate();
			op[ch][3].InitSamprate();
		}
	}
	
	// LFOを初期化
	lfo.InitSamprate();

	// PcmBufポインターをリセット
	PcmBufPtr=0;
//	PcmBufSize = PCMBUFSIZE;

//	PpiReg = 0x0B;
//	AdpcmBaseClock = 0;

	adpcm.InitSamprate();
}

#if X68SOUND_ENABLE_PORTABLE_CODE
Opm::Opm(X68SoundContextImpl *contextImpl)
:	/* 要 c++11 */
	op{
		{contextImpl, contextImpl, contextImpl, contextImpl},
		{contextImpl, contextImpl, contextImpl, contextImpl},
		{contextImpl, contextImpl, contextImpl, contextImpl},
		{contextImpl, contextImpl, contextImpl, contextImpl},
		{contextImpl, contextImpl, contextImpl, contextImpl},
		{contextImpl, contextImpl, contextImpl, contextImpl},
		{contextImpl, contextImpl, contextImpl, contextImpl},
		{contextImpl, contextImpl, contextImpl, contextImpl}
	}
,	lfo(contextImpl)
,	adpcm(contextImpl)
,	pcm8{
		contextImpl, contextImpl, contextImpl, contextImpl,
		contextImpl, contextImpl, contextImpl, contextImpl
	}
{
	m_contextImpl = contextImpl;
#else
Opm::Opm(void) {
#endif
	Author = "m_puusan";

#if X68SOUND_ENABLE_PORTABLE_CODE
	RateForExecuteCmnd = 0;
	RateForPcmset62 = 0;
	RateForPcmset22 = 0;
	Rate2ForPcmset22 = 0;
#endif

#if !X68SOUND_ENABLE_PORTABLE_CODE
	hwo = NULL;
#endif
	PcmBuf = NULL;
#if !X68SOUND_ENABLE_PORTABLE_CODE
	TimerID = NULL;
#endif

#if X68SOUND_ENABLE_PORTABLE_CODE
	/* clang の未使用メンバ警告を抑制するため */
	inpopmbuf_dummy = 0;
	inpadpcmbuf_dummy = 0;
#endif

	Dousa_mode = 0;

}

void Opm::MakeTable() {


	// sinテーブルを作成
	{
		int	i;
		for (i=0; i<SIZESINTBL; ++i) {
#if X68SOUND_ENABLE_PORTABLE_CODE
			SINTBL[i] = (short)(sin(2.0*PI*(i+0.0)/SIZESINTBL)*(MAXSINVAL) + 0.5);
#else
			SINTBL[i] = sin(2.0*PI*(i+0.0)/SIZESINTBL)*(MAXSINVAL) + 0.5;
#endif
		}
//		for (i=0; i<SIZESINTBL/4; ++i) {
//			short s = sin(2.0*PI*i/(SIZESINTBL-0))*(MAXSINVAL+0.0) +0.9;
//			SINTBL[i] = s;
//			SINTBL[SIZESINTBL/2-1-i] = s;
//			SINTBL[i+SIZESINTBL/2] = -s;
//			SINTBL[SIZESINTBL-1-i] = -s;
//		}

	}


	// エンベロープ値 → α 変換テーブルを作成
	{
		int	i;
		for (i=0; i<=ALPHAZERO+SIZEALPHATBL; ++i) {
			ALPHATBL[i] = 0;
		}
		for (i=17; i<=SIZEALPHATBL; ++i) {
#if X68SOUND_ENABLE_PORTABLE_CODE
			ALPHATBL[ALPHAZERO+i] = (unsigned short)floor(
#else
			ALPHATBL[ALPHAZERO+i] = floor(
#endif
				pow(2.0, -((SIZEALPHATBL)-i)*(128.0/8.0)/(SIZEALPHATBL))
				*1.0*1.0*PRECISION +0.0);
		}
	}
	// エンベロープ値 → Noiseα 変換テーブルを作成
	{
		int	i;
		for (i=0; i<=ALPHAZERO+SIZEALPHATBL; ++i) {
			NOISEALPHATBL[i] = 0;
		}
		for (i=17; i<=SIZEALPHATBL; ++i) {
#if X68SOUND_ENABLE_PORTABLE_CODE
			NOISEALPHATBL[ALPHAZERO+i] = (unsigned short)floor(
#else
			NOISEALPHATBL[ALPHAZERO+i] = floor(
#endif
				i*1.0/(SIZEALPHATBL)
				*1.0*0.25*PRECISION +0.0); // Noise音量はOpの1/4
		}
	}

	// D1L → D1l 変換テーブルを作成
	{
		int i;
		for (i=0; i<15; ++i) {
			D1LTBL[i] = i*2;
		}
		D1LTBL[15] = (15+16)*2;
	}


	// C1 <-> M2 入れ替えテーブルを作成
	{
		int	slot;
		for (slot=0; slot<8; ++slot) {
			SLOTTBL[slot] = slot*4;
			SLOTTBL[slot+8] = slot*4+2;
			SLOTTBL[slot+16] = slot*4+1;
			SLOTTBL[slot+24] = slot*4+3;
		}
	}
	
	// Pitch→Δt変換テーブルを作成
	{
		int	oct,notekf,step;

		for (oct=0; oct<=10; ++oct) {
			for (notekf=0; notekf<12*64; ++notekf) {
				if (oct >= 3) {
					step = STEPTBL_O2[notekf] << (oct-3);
				} else {
					step = STEPTBL_O2[notekf] >> (3-oct);
				}
#if X68SOUND_ENABLE_PORTABLE_CODE
				STEPTBL[oct*12*64+notekf] = step * 64 * (int64_t)(OpmRate)/Samprate;
#else
				STEPTBL[oct*12*64+notekf] = step * 64 * (__int64)(OpmRate)/Samprate;
#endif
			}
		}
//		for (notekf=0; notekf<11*12*64; ++notekf) {
//			STEPTBL3[notekf] = STEPTBL[notekf]/3.0+0.5;
//		}
	}


	{
		int i;
		for (i=0; i<=128+4-1; ++i) {
#if X68SOUND_ENABLE_PORTABLE_CODE
			DT1TBL[i] = DT1TBL_org[i] * 64 * (int64_t)(OpmRate)/Samprate;
#else
			DT1TBL[i] = DT1TBL_org[i] * 64 * (__int64)(OpmRate)/Samprate;
#endif
		}

	}


}

unsigned char Opm::OpmPeek() {
	return (unsigned char)StatReg;
}
void Opm::OpmReg(unsigned char no) {
	OpmRegNo = no;
}
void Opm::OpmPoke(unsigned char data) {
	if (NumCmnd < CMNDBUFSIZE) {
		CmndBuf[CmndWriteIdx][0] = OpmRegNo;
		CmndBuf[CmndWriteIdx][1] = data;
		++CmndWriteIdx; CmndWriteIdx&=CMNDBUFSIZE;
#if X68SOUND_ENABLE_PORTABLE_CODE
		++NumCmnd;
#else
		// ++NumCmnd;
		__asm {
			mov ebx,this
			lock inc [ebx].NumCmnd
		}
#endif
	}

	switch (OpmRegNo) {
	case 0x10: case 0x11:
	// TimerA
		{
			if (OpmRegNo == 0x10) {
				TimerAreg10 = data;
			} else {
				TimerAreg11 = data & 3;
			}
			TimerA = 1024-((TimerAreg10<<2)+TimerAreg11);
		}
		break;

	case 0x12:
	// TimerB
		{
			TimerB = (256-(int)data) << (10-6);
		}
		break;

	case 0x14:
	// タイマー制御レジスタ
		{
#if X68SOUND_ENABLE_PORTABLE_CODE
			for(;;){
				int eax = 0;
				if(	TimerSemapho == eax ){
					int ecx = 1;
					TimerSemapho = ecx;
					break;
				} else {
					eax = TimerSemapho;
				}
			}
#else
			__asm {
				lp1:
					xor		eax,eax
					mov		ecx,1
					lock cmpxchg	TimerSemapho,ecx
				jnz		lp1
			}
#endif

			TimerReg = data & 0x0F;
			StatReg &= 0xFF-((data>>4)&3);

			TimerSemapho = 0;
		}
		break;

	case 0x1B:
	// WaveForm
		{
			AdpcmBaseClock = data >> 7;
			SetAdpcmRate();
		}
		break;
	}
}
void Opm::ExecuteCmnd() {

#if X68SOUND_ENABLE_PORTABLE_CODE
	RateForExecuteCmnd -= CmndRate;
	while (RateForExecuteCmnd < 0) {
	RateForExecuteCmnd += 4096;
#else
	static int	rate=0;
	rate -= CmndRate;
	while (rate < 0) {
	rate += 4096;
#endif
	
	
	if (NumCmnd != 0) {

	unsigned char	regno,data;
	regno = CmndBuf[CmndReadIdx][0];
	data = CmndBuf[CmndReadIdx][1];
	++CmndReadIdx; CmndReadIdx&=CMNDBUFSIZE;
#if X68SOUND_ENABLE_PORTABLE_CODE
	--NumCmnd;
#else
	__asm {
		mov ebx,this
		lock dec [ebx].NumCmnd
	}
#endif

	switch (regno) {
	case 0x01:
	// LFO RESET
		{
			if (data & 0x02) {
				lfo.LfoReset();
			} else {
				lfo.LfoStart();
			}
		}
		break;
	
	case 0x08:
	// KON
		{
			int	ch,s,bit;
			ch = data & 7;
			for (s=0,bit=8; s<4; ++s,bit+=bit) {
				if (data & bit) {
					op[ch][s].KeyON();
				} else {
					op[ch][s].KeyOFF();
				}
			}
		}
		break;

	case 0x0F:
	// NE,NFRQ
		{
			op[7][3].SetNFRQ(data&0xFF);
		}
		break;


	case 0x18:
	// LFRQ
		{
			lfo.SetLFRQ(data&0xFF);
		}
		break;
	case 0x19:
	// PMD/AMD
		{
			lfo.SetPMDAMD(data&0xFF);
		}
		break;
	case 0x1B:
	// WaveForm
		{
			lfo.SetWaveForm(data&0xFF);
		}
		break;

	case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
	// PAN/FL/CON
		{
			int ch = regno-0x20;
//			con[ch] = data & 7;
			SetConnection(ch, data&7);
//			pan[ch] = data>>6;
			pan[0][ch] = ((data&0x40) ? -1 : 0);
			pan[1][ch] = ((data&0x80) ? -1 : 0);
			op[ch][0].SetFL(data);
		}
		break;

	case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E: case 0x2F:
	// KC
		{
			int ch = regno-0x28;
			op[ch][0].SetKC(data);
			op[ch][1].SetKC(data);
			op[ch][2].SetKC(data);
			op[ch][3].SetKC(data);
		}
		break;

	case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
	// KF
		{
			int ch = regno-0x30;
			op[ch][0].SetKF(data);
			op[ch][1].SetKF(data);
			op[ch][2].SetKF(data);
			op[ch][3].SetKF(data);
		}
		break;

	case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3E: case 0x3F:
	// PMS/AMS
		{
			int ch = regno-0x38;
			lfo.SetPMSAMS(ch, data&0xFF);
		}
		break;

	case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
	case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
	case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
	case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F:
	// DT1/MUL
		{
			int slot = regno-0x40;
			op[0][SLOTTBL[slot]].SetDT1MUL(data);
		}
		break;

	case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
	case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E: case 0x6F:
	case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
	case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7E: case 0x7F:
	// TL
		{
			int slot = regno-0x60;
			op[0][SLOTTBL[slot]].SetTL(data);
		}
		break;

	case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
	case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F:
	case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
	case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F:
	// KS/AR
		{
			int slot = regno-0x80;
			op[0][SLOTTBL[slot]].SetKSAR(data);
		}
		break;

	case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA6: case 0xA7:
	case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAE: case 0xAF:
	case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7:
	case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF:
	// AME/D1R
		{
			int slot = regno-0xA0;
			op[0][SLOTTBL[slot]].SetAMED1R(data);
		}
		break;

	case 0xC0: case 0xC1: case 0xC2: case 0xC3: case 0xC4: case 0xC5: case 0xC6: case 0xC7:
	case 0xC8: case 0xC9: case 0xCA: case 0xCB: case 0xCC: case 0xCD: case 0xCE: case 0xCF:
	case 0xD0: case 0xD1: case 0xD2: case 0xD3: case 0xD4: case 0xD5: case 0xD6: case 0xD7:
	case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD: case 0xDE: case 0xDF:
	// DT2/D2R
		{
			int slot = regno-0xC0;
			op[0][SLOTTBL[slot]].SetDT2D2R(data);
		}
		break;

	case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5: case 0xE6: case 0xE7:
	case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEE: case 0xEF:
	case 0xF0: case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF6: case 0xF7:
	case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC: case 0xFD: case 0xFE: case 0xFF:
	// D1L/RR
		{
			int slot = regno-0xE0;
			op[0][SLOTTBL[slot]].SetD1LRR(data);
		}
		break;
	
	}

	}
	}

}

void Opm::pcmset62(int ndata) {

#if !X68SOUND_ENABLE_PORTABLE_CODE
	DetectMMX();
#endif

	int	i;
	for (i=0; i<ndata; ++i) {
		int	Out[2];
		Out[0] = Out[1] = 0;
		
		OpmLPFidx += Samprate;
		while (OpmLPFidx >= WaveOutSamp) {
			OpmLPFidx -= WaveOutSamp;

			int OutInpOpm[2];
			OutInpOpm[0] = OutInpOpm[1] = 0;
			if (UseOpmFlag) {
#if X68SOUND_ENABLE_PORTABLE_CODE
				RateForPcmset62 -= OpmRate;
				while (RateForPcmset62 < 0) {
					RateForPcmset62 += 62500;
#else
				static int rate=0;
				rate -= OpmRate;
				while (rate < 0) {
					rate += 62500;
#endif

					timer();
					ExecuteCmnd();
					if ((--EnvCounter2) == 0) {
						EnvCounter2 = 3;
						++EnvCounter1;
						int slot;
						for (slot=0; slot<32; ++slot) {
#if X68SOUND_ENABLE_PORTABLE_CODE
							op[slot&7][slot>>3].Envelope(EnvCounter1);
#else
							op[0][slot].Envelope(EnvCounter1);
#endif
						}
					}
				}

					{
						lfo.Update();

						int lfopitch[8];
						int lfolevel[8];
						int	ch;
						for (ch=0; ch<8; ++ch) {
							op[ch][1].inp=op[ch][2].inp=op[ch][3].inp=OpOut[ch]=0;
	
							lfopitch[ch] = lfo.GetPmValue(ch);
							lfolevel[ch] = lfo.GetAmValue(ch);
						}
						for (ch=0; ch<8; ++ch) {
							op[ch][0].Output0(lfopitch[ch], lfolevel[ch]);
						}
						for (ch=0; ch<8; ++ch) {
							op[ch][1].Output(lfopitch[ch], lfolevel[ch]);
						}
						for (ch=0; ch<8; ++ch) {
							op[ch][2].Output(lfopitch[ch], lfolevel[ch]);
						}
						for (ch=0; ch<7; ++ch) {
							op[ch][3].Output(lfopitch[ch], lfolevel[ch]);
						}
						op[7][3].Output32(lfopitch[7], lfolevel[7]);
					}

					// OpmHpfInp[] に OPM の出力PCMをステレオ加算
					OpmHpfInp[0] =    (OpOut[0] & pan[0][0])
									+ (OpOut[1] & pan[0][1])
									+ (OpOut[2] & pan[0][2])
									+ (OpOut[3] & pan[0][3])
									+ (OpOut[4] & pan[0][4])
									+ (OpOut[5] & pan[0][5])
									+ (OpOut[6] & pan[0][6])
									+ (OpOut[7] & pan[0][7]);
					OpmHpfInp[1] =    (OpOut[0] & pan[1][0])
									+ (OpOut[1] & pan[1][1])
									+ (OpOut[2] & pan[1][2])
									+ (OpOut[3] & pan[1][3])
									+ (OpOut[4] & pan[1][4])
									+ (OpOut[5] & pan[1][5])
									+ (OpOut[6] & pan[1][6])
									+ (OpOut[7] & pan[1][7]);

					OpmHpfInp[0] = (OpmHpfInp[0]&(int)0xFFFFFC00) << 4;
					OpmHpfInp[1] = (OpmHpfInp[1]&(int)0xFFFFFC00) << 4;

					OpmHpfOut[0] = OpmHpfInp[0]-OpmHpfInp_prev[0]
									+OpmHpfOut[0]-(OpmHpfOut[0]>>10)-(OpmHpfOut[0]>>12);
					OpmHpfOut[1] = OpmHpfInp[1]-OpmHpfInp_prev[1]
									+OpmHpfOut[1]-(OpmHpfOut[1]>>10)-(OpmHpfOut[1]>>12);
					OpmHpfInp_prev[0] = OpmHpfInp[0];
					OpmHpfInp_prev[1] = OpmHpfInp[1];

					InpInpOpm[0] = OpmHpfOut[0] >> (4+5);
					InpInpOpm[1] = OpmHpfOut[1] >> (4+5);
					
//					InpInpOpm[0] = (InpInpOpm[0]&(int)0xFFFFFC00)
//									>> ((SIZESINTBL_BITS+PRECISION_BITS)-10-5); // 8*-2^17 ～ 8*+2^17
//					InpInpOpm[1] = (InpInpOpm[1]&(int)0xFFFFFC00)
//									>> ((SIZESINTBL_BITS+PRECISION_BITS)-10-5); // 8*-2^17 ～ 8*+2^17

					InpInpOpm[0] = InpInpOpm[0]*29;
					InpInpOpm[1] = InpInpOpm[1]*29;
					InpOpm[0] = (InpInpOpm[0] + InpInpOpm_prev[0]
						+ InpOpm[0]*70) >> 7;
					InpOpm[1] = (InpInpOpm[1] + InpInpOpm_prev[1]
						+ InpOpm[1]*70) >> 7;
					InpInpOpm_prev[0] = InpInpOpm[0];
					InpInpOpm_prev[1] = InpInpOpm[1];

					OutInpOpm[0] = InpOpm[0] >> 5; // 8*-2^12 ～ 8*+2^12
					OutInpOpm[1] = InpOpm[1] >> 5; // 8*-2^12 ～ 8*+2^12
//					OutInpOpm[0] = (InpOpm[0]*521) >> (5+9); // 8*-2^12 ～ 8*+2^12
//					OutInpOpm[1] = (InpOpm[1]*521) >> (5+9); // OPMとADPCMの音量バランス調整
			}	// UseOpmFlag

			if (UseAdpcmFlag) {
				OutInpAdpcm[0] = OutInpAdpcm[1] = 0;
				// OutInpAdpcm[] に Adpcm の出力PCMを加算
				{
					int	o;
					o = adpcm.GetPcm62();
#if X68SOUND_ENABLE_PORTABLE_CODE
					if (o != (int)0x80000000) {
#else
					if (o != 0x80000000) {
#endif
							OutInpAdpcm[0] += ((((int)(PpiReg)>>1)&1)-1) & o;
							OutInpAdpcm[1] += (((int)(PpiReg)&1)-1) & o;
					}
				}

				// OutInpAdpcm[] に Pcm8 の出力PCMを加算
				{
					int ch;
					for (ch=0; ch<PCM8_NCH; ++ch) {
						int pan;
						pan = pcm8[ch].GetMode();
						int	o;
						o = pcm8[ch].GetPcm62();
#if X68SOUND_ENABLE_PORTABLE_CODE
						if (o != (int)0x80000000) {
#else
						if (o != 0x80000000) {
#endif
								OutInpAdpcm[0] += (-(pan&1)) & o;
								OutInpAdpcm[1] += (-((pan>>1)&1)) & o;
						}
					}
				}


//				OutInpAdpcm[0] >>= 4;
//				OutInpAdpcm[1] >>= 4;
					
				// 音割れ防止
#if X68SOUND_ENABLE_PORTABLE_CODE
				#undef LIMITS
#endif
				#define	LIMITS	((1<<(15+4))-1)
				if ((unsigned int)(OutInpAdpcm[0]+LIMITS) > (unsigned int)(LIMITS*2)) {
					if ((int)(OutInpAdpcm[0]+LIMITS) >= (int)(LIMITS*2)) {
						OutInpAdpcm[0] = LIMITS;
					} else {
						OutInpAdpcm[0] = -LIMITS;
					}
				}
				if ((unsigned int)(OutInpAdpcm[1]+LIMITS) > (unsigned int)(LIMITS*2)) {
					if ((int)(OutInpAdpcm[1]+LIMITS) >= (int)(LIMITS*2)) {
						OutInpAdpcm[1] = LIMITS;
					} else {
						OutInpAdpcm[1] = -LIMITS;
					}
				}

				OutInpAdpcm[0] *= 26;
				OutInpAdpcm[1] *= 26;
				OutInpOutAdpcm[0] = (OutInpAdpcm[0] + OutInpAdpcm_prev[0]+OutInpAdpcm_prev[0] + OutInpAdpcm_prev2[0]
					- OutInpOutAdpcm_prev[0]*(-1537) - OutInpOutAdpcm_prev2[0]*617) >> 10;
				OutInpOutAdpcm[1] = (OutInpAdpcm[1] + OutInpAdpcm_prev[1]+OutInpAdpcm_prev[1] + OutInpAdpcm_prev2[1]
					- OutInpOutAdpcm_prev[1]*(-1537) - OutInpOutAdpcm_prev2[1]*617) >> 10;

				OutInpAdpcm_prev2[0] = OutInpAdpcm_prev[0];
				OutInpAdpcm_prev2[1] = OutInpAdpcm_prev[1];
				OutInpAdpcm_prev[0] = OutInpAdpcm[0];
				OutInpAdpcm_prev[1] = OutInpAdpcm[1];
				OutInpOutAdpcm_prev2[0] = OutInpOutAdpcm_prev[0];
				OutInpOutAdpcm_prev2[1] = OutInpOutAdpcm_prev[1];
				OutInpOutAdpcm_prev[0] = OutInpOutAdpcm[0];
				OutInpOutAdpcm_prev[1] = OutInpOutAdpcm[1];

				OutOutInpAdpcm[0] = OutInpOutAdpcm[0] * (356);
				OutOutInpAdpcm[1] = OutInpOutAdpcm[1] * (356);
				OutOutAdpcm[0] = (OutOutInpAdpcm[0] + OutOutInpAdpcm_prev[0]
					- OutOutAdpcm_prev[0]*(-312)) >> 10;
				OutOutAdpcm[1] = (OutOutInpAdpcm[1] + OutOutInpAdpcm_prev[1]
					- OutOutAdpcm_prev[1]*(-312)) >> 10;

				OutOutInpAdpcm_prev[0] = OutOutInpAdpcm[0];
				OutOutInpAdpcm_prev[1] = OutOutInpAdpcm[1];
				OutOutAdpcm_prev[0] = OutOutAdpcm[0];
				OutOutAdpcm_prev[1] = OutOutAdpcm[1];

//				OutInpOpm[0] += OutOutAdpcm[0] >> 4;	// -2048*16～+2048*16
//				OutInpOpm[1] += OutOutAdpcm[1] >> 4;	// -2048*16～+2048*16
				OutInpOpm[0] += (OutOutAdpcm[0]*506) >> (4+9);	// -2048*16～+2048*16
				OutInpOpm[1] += (OutOutAdpcm[1]*506) >> (4+9);	// OPMとADPCMの音量バランス調整
			}	// UseAdpcmFlag
			


			// 音割れ防止
#if X68SOUND_ENABLE_PORTABLE_CODE
			#undef LIMITS
#endif
			#define	LIMITS	((1<<15)-1)
			if ((unsigned int)(OutInpOpm[0]+LIMITS) > (unsigned int)(LIMITS*2)) {
				if ((int)(OutInpOpm[0]+LIMITS) >= (int)(LIMITS*2)) {
					OutInpOpm[0] = LIMITS;
				} else {
					OutInpOpm[0] = -LIMITS;
				}
			}
			if ((unsigned int)(OutInpOpm[1]+LIMITS) > (unsigned int)(LIMITS*2)) {
				if ((int)(OutInpOpm[1]+LIMITS) >= (int)(LIMITS*2)) {
					OutInpOpm[1] = LIMITS;
				} else {
					OutInpOpm[1] = -LIMITS;
				}
			}

			--InpOpm_idx;
			if (InpOpm_idx < 0) InpOpm_idx=OPMLPF_COL-1;
			InpOpmBuf0[InpOpm_idx] =
			InpOpmBuf0[InpOpm_idx+OPMLPF_COL] = (short)OutInpOpm[0];
			InpOpmBuf1[InpOpm_idx] =
			InpOpmBuf1[InpOpm_idx+OPMLPF_COL] = (short)OutInpOpm[1];
		}

		OpmFir(OpmLPFp, &InpOpmBuf0[InpOpm_idx], &InpOpmBuf1[InpOpm_idx], OutOpm);

		OpmLPFp += OPMLPF_COL;
		if (OpmLPFp >= OPMLOWPASS[OPMLPF_ROW]) {
			OpmLPFp = OPMLOWPASS[0];
		}

		// 全体の音量を調整
		OutOpm[0] = (OutOpm[0]*TotalVolume) >> 8;
		OutOpm[1] = (OutOpm[1]*TotalVolume) >> 8;

		Out[0] -= OutOpm[0];	// -4096 ～ +4096
		Out[1] -= OutOpm[1];


		// WaveFunc()の出力値を加算
		if (WaveFunc != NULL) {
			int	ret;
#if X68SOUND_ENABLE_PORTABLE_CODE
			ret = WaveFunc(WaveFuncArg);
#else
			ret = WaveFunc();
#endif
			Out[0] += (int)(short)ret;
			Out[1] += (ret>>16);
		}


		// 音割れ防止
		if ((unsigned int)(Out[0]+32767) > (unsigned int)(32767*2)) {
			if ((int)(Out[0]+32767) >= (int)(32767*2)) {
				Out[0] = 32767;
			} else {
				Out[0] = -32767;
			}
		}
		if ((unsigned int)(Out[1]+32767) > (unsigned int)(32767*2)) {
			if ((int)(Out[1]+32767) >= (int)(32767*2)) {
				Out[1] = 32767;
			} else {
				Out[1] = -32767;
			}
		}

		PcmBuf[PcmBufPtr][0] = Out[0];
		PcmBuf[PcmBufPtr][1] = Out[1];

		++PcmBufPtr;
		if (PcmBufPtr >= PcmBufSize) {
			PcmBufPtr = 0;
		}
	}

}

void Opm::pcmset22(int ndata) {

	int	i;
	for (i=0; i<ndata; ++i) {
		int	Out[2];
		Out[0] = Out[1] = 0;
		
		if (UseOpmFlag) {
#if X68SOUND_ENABLE_PORTABLE_CODE
			RateForPcmset22-=OpmRate;
			while (RateForPcmset22<0) {
				RateForPcmset22+=22050;
#else
			static int rate=0,rate2=0;

//			rate-=62500;
			rate-=OpmRate;
			while (rate<0) {
				rate+=22050;
#endif

				timer();
				ExecuteCmnd();
				if ((--EnvCounter2) == 0) {
					EnvCounter2 = 3;
					++EnvCounter1;
					int slot;
					for (slot=0; slot<32; ++slot) {
#if X68SOUND_ENABLE_PORTABLE_CODE
						op[slot&7][slot>>3].Envelope(EnvCounter1);
#else
						op[0][slot].Envelope(EnvCounter1);
#endif
					}
				}
			}

					{
						lfo.Update();

						int lfopitch[8];
						int lfolevel[8];
						int	ch;
						for (ch=0; ch<8; ++ch) {
							op[ch][1].inp=op[ch][2].inp=op[ch][3].inp=OpOut[ch]=0;
	
							lfopitch[ch] = lfo.GetPmValue(ch);
							lfolevel[ch] = lfo.GetAmValue(ch);
						}
						for (ch=0; ch<8; ++ch) {
							op[ch][0].Output0(lfopitch[ch], lfolevel[ch]);
						}
						for (ch=0; ch<8; ++ch) {
							op[ch][1].Output(lfopitch[ch], lfolevel[ch]);
						}
						for (ch=0; ch<8; ++ch) {
							op[ch][2].Output(lfopitch[ch], lfolevel[ch]);
						}
						for (ch=0; ch<7; ++ch) {
							op[ch][3].Output(lfopitch[ch], lfolevel[ch]);
						}
						op[7][3].Output32(lfopitch[7], lfolevel[7]);
					}


				// InpInpOpm[] に OPM の出力PCMをステレオ加算
				InpInpOpm[0] =    (OpOut[0] & pan[0][0])
								+ (OpOut[1] & pan[0][1])
								+ (OpOut[2] & pan[0][2])
								+ (OpOut[3] & pan[0][3])
								+ (OpOut[4] & pan[0][4])
								+ (OpOut[5] & pan[0][5])
								+ (OpOut[6] & pan[0][6])
								+ (OpOut[7] & pan[0][7]);
				InpInpOpm[1] =    (OpOut[0] & pan[1][0])
								+ (OpOut[1] & pan[1][1])
								+ (OpOut[2] & pan[1][2])
								+ (OpOut[3] & pan[1][3])
								+ (OpOut[4] & pan[1][4])
								+ (OpOut[5] & pan[1][5])
								+ (OpOut[6] & pan[1][6])
								+ (OpOut[7] & pan[1][7]);
					
				{

					InpInpOpm[0] = (InpInpOpm[0]&(int)0xFFFFFC00)
									>> ((SIZESINTBL_BITS+PRECISION_BITS)-10-5); // 8*-2^17 ～ 8*+2^17
					InpInpOpm[1] = (InpInpOpm[1]&(int)0xFFFFFC00)
									>> ((SIZESINTBL_BITS+PRECISION_BITS)-10-5); // 8*-2^17 ～ 8*+2^17
				}

				InpInpOpm[0] += (InpInpOpm[0]<<4)+InpInpOpm[0];	// * 18
				InpInpOpm[1] += (InpInpOpm[1]<<4)+InpInpOpm[1];	// * 18
				InpOpm[0] = (InpInpOpm[0] + InpInpOpm_prev[0]+InpInpOpm_prev[0] + InpInpOpm_prev2[0]
					+ InpOpm_prev[0]+InpOpm_prev[0]+InpOpm_prev[0] - InpOpm_prev2[0]*11) >> 6;
				InpOpm[1] = (InpInpOpm[1] + InpInpOpm_prev[1]+InpInpOpm_prev[1] + InpInpOpm_prev2[1]
					+ InpOpm_prev[1]+InpOpm_prev[1]+InpOpm_prev[1] - InpOpm_prev2[1]*11) >> 6;

				InpInpOpm_prev2[0] = InpInpOpm_prev[0];
				InpInpOpm_prev2[1] = InpInpOpm_prev[1];
				InpInpOpm_prev[0] = InpInpOpm[0];
				InpInpOpm_prev[1] = InpInpOpm[1];
				InpOpm_prev2[0] = InpOpm_prev[0];
				InpOpm_prev2[1] = InpOpm_prev[1];
				InpOpm_prev[0] = InpOpm[0];
				InpOpm_prev[1] = InpOpm[1];

//			}

			// 全体の音量を調整
			OutOpm[0] = (InpOpm[0]*TotalVolume) >> 8;
			OutOpm[1] = (InpOpm[1]*TotalVolume) >> 8;

			Out[0] -= OutOpm[0]>>(5);	// -4096 ～ +4096
			Out[1] -= OutOpm[1]>>(5);
		}

		if (UseAdpcmFlag) {
#if X68SOUND_ENABLE_PORTABLE_CODE
				Rate2ForPcmset22 -= 15625;
				if (Rate2ForPcmset22 < 0) {
					Rate2ForPcmset22 += 22050;
#else
			static int rate=0,rate2=0;

				rate2 -= 15625;
				if (rate2 < 0) {
					rate2 += 22050;
#endif

					OutInpAdpcm[0] = OutInpAdpcm[1] = 0;
					// OutInpAdpcm[] に Adpcm の出力PCMを加算
					{
						int	o;
						o = adpcm.GetPcm();
#if X68SOUND_ENABLE_PORTABLE_CODE
						if (o != (int)0x80000000) {
#else
						if (o != 0x80000000) {
#endif
							OutInpAdpcm[0] += ((((int)(PpiReg)>>1)&1)-1) & o;
							OutInpAdpcm[1] += (((int)(PpiReg)&1)-1) & o;
						}
					}

					// OutInpAdpcm[] に Pcm8 の出力PCMを加算
					{
						int ch;
						for (ch=0; ch<PCM8_NCH; ++ch) {
							int pan;
							pan = pcm8[ch].GetMode();
							int	o;
							o = pcm8[ch].GetPcm();
#if X68SOUND_ENABLE_PORTABLE_CODE
							if (o != (int)0x80000000) {
#else
							if (o != 0x80000000) {
#endif
								OutInpAdpcm[0] += (-(pan&1)) & o;
								OutInpAdpcm[1] += (-((pan>>1)&1)) & o;
							}
						}
					}

					// 全体の音量を調整
//					OutInpAdpcm[0] = (OutInpAdpcm[0]*TotalVolume) >> 8;
//					OutInpAdpcm[1] = (OutInpAdpcm[1]*TotalVolume) >> 8;

					// 音割れ防止
#if X68SOUND_ENABLE_PORTABLE_CODE
					#undef LIMITS
#endif
					#define	LIMITS	((1<<19)-1)
					if ((unsigned int)(OutInpAdpcm[0]+LIMITS) > (unsigned int)(LIMITS*2)) {
						if ((int)(OutInpAdpcm[0]+LIMITS) >= (int)(LIMITS*2)) {
							OutInpAdpcm[0] = LIMITS;
						} else {
							OutInpAdpcm[0] = -LIMITS;
						}
					}
					if ((unsigned int)(OutInpAdpcm[1]+LIMITS) > (unsigned int)(LIMITS*2)) {
						if ((int)(OutInpAdpcm[1]+LIMITS) >= (int)(LIMITS*2)) {
							OutInpAdpcm[1] = LIMITS;
						} else {
							OutInpAdpcm[1] = -LIMITS;
						}
					}

					OutInpAdpcm[0] *= 40;
					OutInpAdpcm[1] *= 40;
				}

				OutOutAdpcm[0] = (OutInpAdpcm[0] + OutInpAdpcm_prev[0]+OutInpAdpcm_prev[0] + OutInpAdpcm_prev2[0]
								- OutOutAdpcm_prev[0]*(-157) - OutOutAdpcm_prev2[0]*(61)) >> 8;
				OutOutAdpcm[1] = (OutInpAdpcm[1] + OutInpAdpcm_prev[1]+OutInpAdpcm_prev[1] + OutInpAdpcm_prev2[1]
								- OutOutAdpcm_prev[1]*(-157) - OutOutAdpcm_prev2[1]*(61)) >> 8;

				OutInpAdpcm_prev2[0] = OutInpAdpcm_prev[0];
				OutInpAdpcm_prev2[1] = OutInpAdpcm_prev[1];
				OutInpAdpcm_prev[0] = OutInpAdpcm[0];
				OutInpAdpcm_prev[1] = OutInpAdpcm[1];
				OutOutAdpcm_prev2[0] = OutOutAdpcm_prev[0];
				OutOutAdpcm_prev2[1] = OutOutAdpcm_prev[1];
				OutOutAdpcm_prev[0] = OutOutAdpcm[0];
				OutOutAdpcm_prev[1] = OutOutAdpcm[1];



//			Out[0] += OutAdpcm[0] >> 4;	// -2048*16～+2048*16
//			Out[1] += OutAdpcm[1] >> 4;
			Out[0] -= OutOutAdpcm[0] >> 4;	// -2048*16～+2048*16
			Out[1] -= OutOutAdpcm[1] >> 4;
		}

//		// 全体の音量を調整
//		Out[0] = (Out[0]*TotalVolume) >> 8;
//		Out[1] = (Out[1]*TotalVolume) >> 8;


		// WaveFunc()の出力値を加算
		if (WaveFunc != NULL) {
			int	ret;
#if X68SOUND_ENABLE_PORTABLE_CODE
			ret = WaveFunc(WaveFuncArg);
#else
			ret = WaveFunc();
#endif
			Out[0] += (int)(short)ret;
			Out[1] += (ret>>16);
		}


		// 音割れ防止
		if ((unsigned int)(Out[0]+32767) > (unsigned int)(32767*2)) {
			if ((int)(Out[0]+32767) >= (int)(32767*2)) {
				Out[0] = 32767;
			} else {
				Out[0] = -32767;
			}
		}
		if ((unsigned int)(Out[1]+32767) > (unsigned int)(32767*2)) {
			if ((int)(Out[1]+32767) >= (int)(32767*2)) {
				Out[1] = 32767;
			} else {
				Out[1] = -32767;
			}
		}

		PcmBuf[PcmBufPtr][0] = Out[0];
		PcmBuf[PcmBufPtr][1] = Out[1];

		++PcmBufPtr;
		if (PcmBufPtr >= PcmBufSize) {
			PcmBufPtr = 0;
		}
	}
}

int Opm::GetPcm(void *buf, int ndata) {
	if (Dousa_mode != 2) {
		return X68SNDERR_NOTACTIVE;
	}
	PcmBuf = (short (*)[2])buf;
	PcmBufPtr=0;
	if (Samprate != 22050) {
		pcmset62(ndata);
	} else {
		pcmset22(ndata);
	}
	PcmBuf = NULL;
	return 0;
}

void Opm::timer() {
	char	res;
#if X68SOUND_ENABLE_PORTABLE_CODE
	{
		int eax = 0;
		int ecx = 1;
		if(	TimerSemapho == eax ){
			TimerSemapho = ecx;
			res = 0;
		} else {
			eax = TimerSemapho;
			res = 1;
		}
	}
#else
	__asm {
		xor		eax,eax
		mov		ecx,1
		lock cmpxchg	TimerSemapho,ecx
		setnz	res
	}
#endif
	if (res) {
		return;
	}

	int	prev_stat = StatReg;
	int flag_set = 0;
	if (TimerReg & 0x01) {	// TimerA 動作中
		++TimerAcounter;
		if (TimerAcounter >= TimerA) {
			flag_set |= ((TimerReg>>2) & 0x01);
			TimerAcounter = 0;
		}
	}
	if (TimerReg & 0x02) {	// TimerB 動作中
		++TimerBcounter;
		if (TimerBcounter >= TimerB) {
			flag_set |= ((TimerReg>>2) & 0x02);
			TimerBcounter = 0;
		}
	}

//	int	next_stat = StatReg;

	StatReg |= flag_set;

	TimerSemapho = 0;

	if (flag_set != 0) {
		if (
			prev_stat == 0) {
			if (OpmIntProc != NULL) {
#if X68SOUND_ENABLE_PORTABLE_CODE
				OpmIntProc(OpmIntArg);
#else
				OpmIntProc();
#endif
			}
		}
	}

}


int Opm::Start(int samprate, int opmflag, int adpcmflag,
						int betw, int pcmbuf, int late, double rev) {
	if (Dousa_mode != 0) {
		return X68SNDERR_ALREADYACTIVE;
	}
	Dousa_mode = 1;

	if (rev < 0.1) rev = 0.1;	
	
	UseOpmFlag = opmflag;
	UseAdpcmFlag = adpcmflag;
	_betw = betw;
	_pcmbuf = pcmbuf;
	_late = late;
#if X68SOUND_ENABLE_PORTABLE_CODE
	_rev = (int)rev;
#else
	_rev = rev;
#endif
	
	if (samprate == 44100) {
		Samprate = 62500;
		WaveOutSamp = 44100;
		OPMLPF_ROW = OPMLPF_ROW_44;
		OPMLOWPASS = OPMLOWPASS_44;
	} else if (samprate == 48000) {
		Samprate = 62500;
		WaveOutSamp = 48000;
		OPMLPF_ROW = OPMLPF_ROW_48;
		OPMLOWPASS = OPMLOWPASS_48;
	} else {
		Samprate = 22050;
		WaveOutSamp = 22050;
	}

	MakeTable();
	Reset();

	return WaveAndTimerStart();
}

int Opm::StartPcm(int samprate, int opmflag, int adpcmflag, int pcmbuf) {
	if (Dousa_mode != 0) {
		return X68SNDERR_ALREADYACTIVE;
	}
	Dousa_mode = 2;

	UseOpmFlag = opmflag;
	UseAdpcmFlag = adpcmflag;
	_betw = 5;
	_pcmbuf = pcmbuf;
	_late = 200;
#if X68SOUND_ENABLE_PORTABLE_CODE
	_rev = 1;
#else
	_rev = 1.0;
#endif

	if (samprate == 44100) {
		Samprate = 62500;
		WaveOutSamp = 44100;
		OPMLPF_ROW = OPMLPF_ROW_44;
		OPMLOWPASS = OPMLOWPASS_44;
	} else if (samprate == 48000) {
		Samprate = 62500;
		WaveOutSamp = 48000;
		OPMLPF_ROW = OPMLPF_ROW_48;
		OPMLOWPASS = OPMLOWPASS_48;
	} else {
		Samprate = 22050;
		WaveOutSamp = 22050;
	}

	MakeTable();
	Reset();

	PcmBufSize = 0xFFFFFFFF;

	return WaveAndTimerStart();
}

int Opm::SetSamprate(int samprate) {
	if (Dousa_mode == 0) {
		return X68SNDERR_NOTACTIVE;
	}
	int dousa_mode_bak = Dousa_mode;

	Free();
	
	if (samprate == 44100) {
		Samprate = 62500;
		WaveOutSamp = 44100;
		OPMLPF_ROW = OPMLPF_ROW_44;
		OPMLOWPASS = OPMLOWPASS_44;
	} else if (samprate == 48000) {
		Samprate = 62500;
		WaveOutSamp = 48000;
		OPMLPF_ROW = OPMLPF_ROW_48;
		OPMLOWPASS = OPMLOWPASS_48;
	} else {
		Samprate = 22050;
		WaveOutSamp = 22050;
	}

	MakeTable();
	ResetSamprate();

	Dousa_mode = dousa_mode_bak;
	return WaveAndTimerStart();
}

int Opm::SetOpmClock(int clock) {
	int rate = clock >> 6;
	if (rate <= 0) {
		return X68SNDERR_BADARG;
	}
	if (Dousa_mode == 0) {
		OpmRate = rate;
		return 0;
	}
	int dousa_mode_bak = Dousa_mode;

	Free();
	
	OpmRate = rate;
	
	MakeTable();
	ResetSamprate();

	Dousa_mode = dousa_mode_bak;
	return WaveAndTimerStart();
}

int Opm::WaveAndTimerStart() {
	
	Betw_Time = _betw;
	TimerResolution = _betw;
	Late_Time = _late+_betw;
#if X68SOUND_ENABLE_PORTABLE_CODE
	Betw_Samples_Slower = (int)floor((double)(WaveOutSamp)*_betw/1000.0-_rev);
	Betw_Samples_Faster = (int)ceil((double)(WaveOutSamp)*_betw/1000.0+_rev);
	Betw_Samples_VerySlower = (int)(floor((double)(WaveOutSamp)*_betw/1000.0-_rev)/8.0);
#else
	Betw_Samples_Slower = floor((double)(WaveOutSamp)*_betw/1000.0-_rev);
	Betw_Samples_Faster = ceil((double)(WaveOutSamp)*_betw/1000.0+_rev);
	Betw_Samples_VerySlower = floor((double)(WaveOutSamp)*_betw/1000.0-_rev)/8.0;
#endif
	Late_Samples = WaveOutSamp*Late_Time/1000;

//	Blk_Samples = WaveOutSamp/N_waveblk;
	Blk_Samples = Late_Samples;

//	Faster_Limit = Late_Samples;
//	Faster_Limit = WaveOutSamp*50/1000;
	if (Late_Samples >= WaveOutSamp*175/1000) {
		Faster_Limit = Late_Samples - WaveOutSamp*125/1000;
	} else {
		Faster_Limit = WaveOutSamp*50/1000;
	}
	if (Faster_Limit > Late_Samples) Faster_Limit = Late_Samples;
	Slower_Limit = Faster_Limit;
//	Slower_Limit = WaveOutSamp*100/1000;
//	Slower_Limit = Late_Samples;
	if (Slower_Limit > Late_Samples) Slower_Limit = Late_Samples;
	
	if (Dousa_mode != 1) {
		return 0;
	}
	

	
	PcmBufSize = Blk_Samples * N_waveblk;
	nSamples = (unsigned int)(Betw_Samples_Faster);

	
#if !X68SOUND_ENABLE_PORTABLE_CODE
	thread_handle = CreateThread(NULL, 0, waveOutThread, NULL, 0, &thread_id);
	if (thread_handle == NULL) {
		Free();
		ErrorCode = 5;
		return X68SNDERR_TIMER;
	}
//	SetThreadPriority(thread_handle, THREAD_PRIORITY_ABOVE_NORMAL );
//	SetThreadPriority(thread_handle, THREAD_PRIORITY_BELOW_NORMAL );
	SetThreadPriority(thread_handle, THREAD_PRIORITY_HIGHEST );
	while (!thread_flag) Sleep(100);




	MMRESULT	ret;

	
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = WaveOutSamp;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.nChannels*(wfx.wBitsPerSample/8);
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec*wfx.nBlockAlign;
	wfx.cbSize = 0;

	timer_start_flag = 0;
	if ((ret=waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD)waveOutProc, NULL, CALLBACK_FUNCTION ))
		!= MMSYSERR_NOERROR) {
		hwo = NULL;
		Free();
		ErrorCode = 0x10000000+ret;
		return X68SNDERR_PCMOUT;
	}
//	if (waveOutReset(hwo) != MMSYSERR_NOERROR) {
//		waveOutClose(hwo);
//		hwo = NULL;
//		return X68SNDERR_PCMOUT;
//	}


	PcmBuf = (short (*)[2])GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE, PcmBufSize*2*2 );
	if (!PcmBuf) {
		Free();
		ErrorCode = 2;
		return X68SNDERR_MEMORY;
	}
	lpwh = (LPWAVEHDR)GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE,
                  (DWORD) sizeof(WAVEHDR)*N_waveblk);
	if (!lpwh) {
		Free();
		ErrorCode = 3;
		return X68SNDERR_MEMORY;
	}

//	pcmset(Late_Samples);
	{
		unsigned int i;
		for (i=0; i<PcmBufSize; ++i) {
			PcmBuf[i][0] = PcmBuf[i][1] = 0;
		}
	}

	{
		int i;
		N_wavehdr = 0;
		for (i=0; i<N_waveblk; ++i) {
			(lpwh+i)->lpData = (LPSTR)PcmBuf[Blk_Samples*i];
			(lpwh+i)->dwBufferLength = Blk_Samples*2*2;
			(lpwh+i)->dwFlags = 0;

			if ((ret=waveOutPrepareHeader(hwo, lpwh+i, sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
				Free();
				ErrorCode = 0x20000000+ret;
				return X68SNDERR_PCMOUT;
			}
			++N_wavehdr;
		}
	}

	PcmBufPtr = Blk_Samples + Late_Samples + Betw_Samples_Faster;
	while (PcmBufPtr >= PcmBufSize) PcmBufPtr -= PcmBufSize;
	waveblk = 0;
	playingblk = 0;
//	playingblk_next = playingblk+1;
	{
		int i;
		for (i=0; i<N_waveblk; ++i) {
			PostThreadMessage(thread_id, THREADMES_WAVEOUTDONE, 0, 0);
		}
	}

	timeBeginPeriod(TimerResolution);
	TimerID=timeSetEvent(Betw_Time, TimerResolution, OpmTimeProc, NULL, TIME_PERIODIC);
	if (TimerID == NULL) {
		Free();
		ErrorCode = 4;
		return X68SNDERR_TIMER;
	}

	while (!timer_start_flag) Sleep(200);	// マルチメディアタイマーの処理が開始されるまで待つ
#endif

	return 0;
}


void Opm::Free() {
#if !X68SOUND_ENABLE_PORTABLE_CODE
	if (TimerID != NULL) {	// マルチメディアタイマー停止
		timeKillEvent(TimerID);
		TimerID = NULL;
		timeEndPeriod(TimerResolution);
	}
	if (thread_handle) {	// スレッド停止
		PostThreadMessage(thread_id, THREADMES_KILL, 0, 0);
		WaitForSingleObject(thread_handle, INFINITE);
		CloseHandle(thread_handle);
		thread_handle = NULL;
	}
	timer_start_flag = 0;	// マルチメディアタイマーの処理を停止
	if (hwo != NULL) {		// ウェーブ再生停止
//		waveOutReset(hwo);
		if (lpwh) {
			int i;
			for (i=0; i<N_wavehdr; ++i) {
				waveOutUnprepareHeader(hwo, lpwh+i, sizeof(WAVEHDR));
			}
			N_wavehdr = 0;
			GlobalFreePtr( lpwh );
			lpwh=NULL;
		}
		waveOutClose(hwo);
		if (PcmBuf) { GlobalFreePtr( PcmBuf ); PcmBuf=NULL; }
		hwo = NULL;
	}
#endif


	Dousa_mode = 0;
}

Opm::~Opm() {
	Free();
}


#if X68SOUND_ENABLE_PORTABLE_CODE
void Opm::OpmInt(void (*proc)(void *), void *arg) {
	OpmIntProc = proc;
	OpmIntArg = arg;
#else
void Opm::OpmInt(void (CALLBACK *proc)()) {
	OpmIntProc = proc;
#endif
}

unsigned char Opm::AdpcmPeek() {
	return adpcm.AdpcmReg;
}
void Opm::AdpcmPoke(unsigned char data) {
//#ifdef ADPCM_POLY
#if 0
	// PCM8テスト
	if (data & 0x02) {	// ADPCM再生開始
		static int pcm8_pantbl[4]={3,1,2,0};
		int minch=0,ch;
		unsigned int minlen=0xFFFFFFFF;
		for (ch=0; ch<PCM8_NCH; ++ch) {
			if ((unsigned int)pcm8[ch].GetRest() < minlen) {
				minlen = pcm8[ch].GetRest();
				minch = ch;
			}
		}
		if (adpcm.DmaReg[0x05] & 0x08) {	// チェイニング動作
			if (!(adpcm.DmaReg[0x05] & 0x04)) {	// アレイチェイン
				pcm8[minch].Aot(bswapl(*(unsigned char **)&(adpcm.DmaReg[0x1C])),
					(8<<16)+(ADPCMRATETBL[AdpcmBaseClock][(PpiReg>>2)&3]<<8)+pcm8_pantbl[PpiReg&3],
					bswapw(*(unsigned short *)&(adpcm.DmaReg[0x1A])));
			} else {						// リンクアレイチェイン
				pcm8[minch].Lot(bswapl(*(unsigned char **)&(adpcm.DmaReg[0x1C])),
					(8<<16)+(ADPCMRATETBL[AdpcmBaseClock][(PpiReg>>2)&3]<<8)+pcm8_pantbl[PpiReg&3]);
			}
		} else {	// ノーマル転送
			pcm8[minch].Out(bswapl(*(unsigned char **)&(adpcm.DmaReg[0x0C])),
				(8<<16)+(ADPCMRATETBL[AdpcmBaseClock][(PpiReg>>2)&3]<<8)+pcm8_pantbl[PpiReg&3],
				bswapw(*(unsigned short *)&(adpcm.DmaReg[0x0A])));
		}
		if (adpcm.IntProc != NULL) {
			adpcm.IntProc();
		}
	} else if (data & 0x01) {	// 再生動作停止
	}
	return;
#endif
	
	// オリジナル
	if (data & 0x02) {	// ADPCM再生開始
		adpcm.AdpcmReg &= 0x7F;
	} else if (data & 0x01) {	// 再生動作停止
		adpcm.AdpcmReg |= 0x80;
		adpcm.Reset();
	}
}
unsigned char Opm::PpiPeek() {
	return PpiReg;
}
void Opm::PpiPoke(unsigned char data) {
	PpiReg = data;
	SetAdpcmRate();
}
void Opm::PpiCtrl(unsigned char data) {
	if (!(data&0x80)) {
		if (data&0x01) {
			PpiReg |= 1<<((data>>1)&7);
		} else {
			PpiReg &= 0xFF^(1<<((data>>1)&7));
		}
		SetAdpcmRate();
	}
}
unsigned char Opm::DmaPeek(unsigned char adrs) {
	if (adrs >= 0x40) return 0;
	if (adrs == 0x00) {
		if ((adpcm.AdpcmReg & 0x80) == 0) {	// ADPCM 再生中
#if X68SOUND_ENABLE_PORTABLE_CODE
			adpcm.DmaReg.asUint8[0x00] |= 0x02;
			return adpcm.DmaReg.asUint8[0x00] | 0x01;
#else
			adpcm.DmaReg[0x00] |= 0x02;
			return adpcm.DmaReg[0x00] | 0x01;
#endif
		}
	}
#if X68SOUND_ENABLE_PORTABLE_CODE
	return adpcm.DmaReg.asUint8[adrs];
#else
	return adpcm.DmaReg[adrs];
#endif
}
void Opm::DmaPoke(unsigned char adrs, unsigned char data) {
	if (adrs >= 0x40) return;
	switch (adrs) {
	case 0x00:	// CSR
		data &= 0xF6;					// ACTとPCSはクリアしない
#if X68SOUND_ENABLE_PORTABLE_CODE
		adpcm.DmaReg.asUint8[adrs] &= ~data;
#else
		adpcm.DmaReg[adrs] &= ~data;
#endif
		if (data & 0x10) {
#if X68SOUND_ENABLE_PORTABLE_CODE
			adpcm.DmaReg.asUint8[0x01] = 0;
#else
			adpcm.DmaReg[0x01] = 0;
#endif
		}
		return;
		break;
	case 0x01:	// CER
		return;
		break;
	case 0x04: // DCR
	case 0x05: // OCR
	case 0x06: // SCR
	case 0x0A: // MTC
	case 0x0B: // MTC
	case 0x0C: // MAR
	case 0x0D: // MAR
	case 0x0E: // MAR
	case 0x0F: // MAR
	case 0x14: // DAR
	case 0x15: // DAR
	case 0x16: // DAR
	case 0x17: // DAR
	case 0x29: // MFC
	case 0x31: // DFC
#if X68SOUND_ENABLE_PORTABLE_CODE
		if (adpcm.DmaReg.asUint8[0x00] & 0x08) {	// ACT==1 ?
#else
		if (adpcm.DmaReg[0x00] & 0x08) {	// ACT==1 ?
#endif
			adpcm.DmaError(0x02);	// 動作タイミングエラー
			break;
		}
	case 0x1A:	// BTC
	case 0x1B:	// BTC
	case 0x1C:	// BAR
	case 0x1D:	// BAR
	case 0x1E:	// BAR
	case 0x1F:	// BAR
	case 0x25:	// NIV
	case 0x27:	// EIV
	case 0x2D:	// CPR
	case 0x39:	// BFC
	case 0x3F:	// GCR
#if X68SOUND_ENABLE_PORTABLE_CODE
		adpcm.DmaReg.asUint8[adrs] = data;
#else
		adpcm.DmaReg[adrs] = data;
#endif
		break;

	case 0x07:
#if X68SOUND_ENABLE_PORTABLE_CODE
		adpcm.DmaReg.asUint8[0x07] = data & 0x78;
#else
		adpcm.DmaReg[0x07] = data & 0x78;
#endif
		if (data&0x80) {		// STR == 1 ?

#if X68SOUND_ENABLE_PORTABLE_CODE
			if (adpcm.DmaReg.asUint8[0x00] & 0xF8) {	// COC|BTC|NDT|ERR|ACT == 1 ?
				adpcm.DmaError(0x02);	// 動作タイミングエラー
				adpcm.DmaReg.asUint8[0x07] = data & 0x28;
#else
			if (adpcm.DmaReg[0x00] & 0xF8) {	// COC|BTC|NDT|ERR|ACT == 1 ?
				adpcm.DmaError(0x02);	// 動作タイミングエラー
				adpcm.DmaReg[0x07] = data & 0x28;
#endif
				break;
			}
#if X68SOUND_ENABLE_PORTABLE_CODE
			adpcm.DmaReg.asUint8[0x00] |= 0x08;	// ACT=1
#else
			adpcm.DmaReg[0x00] |= 0x08;	// ACT=1
#endif
//adpcm.FinishFlag=0;
#if X68SOUND_ENABLE_PORTABLE_CODE
			if (  adpcm.DmaReg.asUint8[0x04] & 0x08		// DPS != 0 ?
				||adpcm.DmaReg.asUint8[0x06] & 0x03		// DAC != 00 ?
				||bswapl(adpcm.DmaReg.asUint32[0x14/4]) != 0x00E92003) {
				adpcm.DmaError(0x0A);	// バスエラー(デバイスアドレス)
				adpcm.DmaReg.asUint8[0x07] = data & 0x28;
#else
			if (  adpcm.DmaReg[0x04] & 0x08		// DPS != 0 ?
				||adpcm.DmaReg[0x06] & 0x03		// DAC != 00 ?
				||bswapl(*(unsigned char **)&adpcm.DmaReg[0x14]) != (unsigned char *)0x00E92003) {
				adpcm.DmaError(0x0A);	// バスエラー(デバイスアドレス)
				adpcm.DmaReg[0x07] = data & 0x28;
#endif
				break;
			}
			unsigned char ocr;
#if X68SOUND_ENABLE_PORTABLE_CODE
			ocr = adpcm.DmaReg.asUint8[0x05] & 0xB0;
#else
			ocr = adpcm.DmaReg[0x05] & 0xB0;
#endif
			if (ocr != 0x00 && ocr != 0x30) {	// DIR==1 || SIZE!=00&&SIZE!=11 ?
				adpcm.DmaError(0x01);	// コンフィグレーションエラー
#if X68SOUND_ENABLE_PORTABLE_CODE
				adpcm.DmaReg.asUint8[0x07] = data & 0x28;
#else
				adpcm.DmaReg[0x07] = data & 0x28;
#endif
				break;
			}
			
		}
		if (data&0x40) {	// CNT == 1 ?
#if X68SOUND_ENABLE_PORTABLE_CODE
			if ( (adpcm.DmaReg.asUint8[0x00] & 0x48) != 0x08 ) {	// !(BTC==0&&ACT==1) ?
				adpcm.DmaError(0x02);	// 動作タイミングエラー
				adpcm.DmaReg.asUint8[0x07] = data & 0x28;
#else
			if ( (adpcm.DmaReg[0x00] & 0x48) != 0x08 ) {	// !(BTC==0&&ACT==1) ?
				adpcm.DmaError(0x02);	// 動作タイミングエラー
				adpcm.DmaReg[0x07] = data & 0x28;
#endif
				break;
			}

#if X68SOUND_ENABLE_PORTABLE_CODE
			if (adpcm.DmaReg.asUint8[0x05] & 0x08) {	// CHAIN == 10 or 11 ?
				adpcm.DmaError(0x01);	// コンフィグレーションエラー
				adpcm.DmaReg.asUint8[0x07] = data & 0x28;
#else
			if (adpcm.DmaReg[0x05] & 0x08) {	// CHAIN == 10 or 11 ?
				adpcm.DmaError(0x01);	// コンフィグレーションエラー
				adpcm.DmaReg[0x07] = data & 0x28;
#endif
				break;
			}

		}
		if (data&0x10) {	// SAB == 1 ?
#if X68SOUND_ENABLE_PORTABLE_CODE
			if (adpcm.DmaReg.asUint8[0x00] & 0x08) {	// ACT == 1 ?
				adpcm.DmaError(0x11);	// ソフトウェア強制停止
				adpcm.DmaReg.asUint8[0x07] = data & 0x28;
#else
			if (adpcm.DmaReg[0x00] & 0x08) {	// ACT == 1 ?
				adpcm.DmaError(0x11);	// ソフトウェア強制停止
				adpcm.DmaReg[0x07] = data & 0x28;
#endif
				break;
			}
		}
		if (data&0x80) {	// STR == 1 ?
			data &= 0x7F;

#if X68SOUND_ENABLE_PORTABLE_CODE
			if (adpcm.DmaReg.asUint8[0x05] & 0x08) {	// チェイニング動作
				if (!(adpcm.DmaReg.asUint8[0x05] & 0x04)) {	// アレイチェイン
					if (adpcm.DmaArrayChainSetNextMtcMar()) {
						adpcm.DmaReg.asUint8[0x07] = data & 0x28;
#else
			if (adpcm.DmaReg[0x05] & 0x08) {	// チェイニング動作
				if (!(adpcm.DmaReg[0x05] & 0x04)) {	// アレイチェイン
					if (adpcm.DmaArrayChainSetNextMtcMar()) {
						adpcm.DmaReg[0x07] = data & 0x28;
#endif
						break;
					}
				} else {						// リンクアレイチェイン
					if (adpcm.DmaLinkArrayChainSetNextMtcMar()) {
#if X68SOUND_ENABLE_PORTABLE_CODE
						adpcm.DmaReg.asUint8[0x07] = data & 0x28;
#else
						adpcm.DmaReg[0x07] = data & 0x28;
#endif
						break;
					}
				}
			}
			
#if X68SOUND_ENABLE_PORTABLE_CODE
			if ( adpcm.DmaReg.asUint16[0x0A/2] == 0 ) {	// MTC == 0 ?
#else
			if ( (*(unsigned short *)&adpcm.DmaReg[0x0A]) == 0 ) {	// MTC == 0 ?
#endif
				adpcm.DmaError(0x0D);	// カウントエラー(メモリアドレス/メモリカウンタ)
				data &= 0x28;
				break;
			}
			
		}
		break;
	}
}
#if X68SOUND_ENABLE_PORTABLE_CODE
void Opm::DmaInt(void (*proc)(void *), void *arg) {
	adpcm.IntProc = proc;
	adpcm.IntArg = arg;
#else
void Opm::DmaInt(void (CALLBACK *proc)()) {
	adpcm.IntProc = proc;
#endif
}
#if X68SOUND_ENABLE_PORTABLE_CODE
void Opm::DmaErrInt(void (*proc)(void *), void *arg) {
	adpcm.ErrIntProc = proc;
	adpcm.ErrIntArg = arg;
#else
void Opm::DmaErrInt(void (CALLBACK *proc)()) {
	adpcm.ErrIntProc = proc;
#endif
}


int Opm::Pcm8_Out(int ch, void *adrs, int mode, int len) {
	return pcm8[ch & (PCM8_NCH-1)].Out(adrs, mode, len);
}
int Opm::Pcm8_Aot(int ch, void *tbl, int mode, int cnt) {
	return pcm8[ch & (PCM8_NCH-1)].Aot(tbl, mode, cnt);
}
int Opm::Pcm8_Lot(int ch, void *tbl, int mode) {
	return pcm8[ch & (PCM8_NCH-1)].Lot(tbl, mode);
}
int Opm::Pcm8_SetMode(int ch, int mode) {
	return pcm8[ch & (PCM8_NCH-1)].SetMode(mode);
}
int Opm::Pcm8_GetRest(int ch) {
	return pcm8[ch & (PCM8_NCH-1)].GetRest();
}
int Opm::Pcm8_GetMode(int ch) {
	return pcm8[ch & (PCM8_NCH-1)].GetMode();
}
int Opm::Pcm8_Abort() {
	int ch;
	for (ch=0; ch<PCM8_NCH; ++ch) {
		pcm8[ch].Init();
	}
	return 0;
}

int Opm::SetTotalVolume(int v) {
	if ((unsigned int)v <= 65535) {
		TotalVolume = v;
	}
	return TotalVolume;
}

int Opm::GetTotalVolume(void) {
	return TotalVolume;
}

#if X68SOUND_ENABLE_PORTABLE_CODE
void Opm::BetwInt(void (*proc)(void *), void *arg) {
	BetwIntProc = proc;
	BetwIntArg = arg;
#else
void Opm::BetwInt(void (CALLBACK *proc)()) {
	BetwIntProc = proc;
#endif
}
void Opm::betwint() {
	if (BetwIntProc != NULL) {
#if X68SOUND_ENABLE_PORTABLE_CODE
		BetwIntProc(BetwIntArg);
#endif
	}
}

#if X68SOUND_ENABLE_PORTABLE_CODE
void Opm::SetWaveFunc(int (*func)(void *), void *arg) {
	WaveFunc = func;
	WaveFuncArg = arg;
#else
void Opm::SetWaveFunc(int (CALLBACK *func)()) {
	WaveFunc = func;
#endif
}


void Opm::PushRegs() {
	OpmRegNo_backup = OpmRegNo;
}
void Opm::PopRegs() {
	OpmRegNo = OpmRegNo_backup;
}



#if X68SOUND_ENABLE_PORTABLE_CODE
void Opm::MemReadFunc(int (*func)(unsigned char *)) {
#else
void Opm::MemReadFunc(int (CALLBACK *func)(unsigned char *)) {
#endif
	if (func == NULL) {
		MemRead = MemReadDefault;
	} else {
		MemRead = func;
	}
}

