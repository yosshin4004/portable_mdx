#ifndef __X68SOUND_OPM_H__
#define __X68SOUND_OPM_H__

#include "x68sound_config.h"

#if X68SOUND_ENABLE_PORTABLE_CODE
	#include "x68sound_global.h"
	#include "x68sound_op.h"
	#include "x68sound_lfo.h"
	#include "x68sound_adpcm.h"
	#include "x68sound_pcm8.h"
#endif


#define	CMNDBUFSIZE	65535
#define	FlagBufSize	7

//#define	RES	(20)
//#define	NDATA	(44100/5)
#define	PCMBUFSIZE	65536
//#define	DELAY	(1000/5)

class Opm {
#if X68SOUND_ENABLE_PORTABLE_CODE
public:
	struct tagX68SoundContextImpl *m_contextImpl;
private:
#endif
#if X68SOUND_ENABLE_PORTABLE_CODE
	const char *Author;
#else
	char *Author;
#endif

	Op	op[8][4];	// オペレータ0～31
	int	EnvCounter1;	// エンベロープ用カウンタ1 (0,1,2,3,4,5,6,...)
	int	EnvCounter2;	// エンベロープ用カウンタ2 (3,2,1,3,2,1,3,2,...)
//	int	con[N_CH];	// アルゴリズム 0～7
	int	pan[2][N_CH];	// 0:無音 -1:出力
//	int pms[N_CH];	// 0, 1, 2, 4, 10, 20, 80, 140
//	int	ams[N_CH];	// 右シフト回数 31(0), 2(1), 1(2), 0(3)
//	int	pmd;
//	int	amd;
//	int	pmspmd[N_CH];	// pms[]*pmd
	Lfo	lfo;
	int	SLOTTBL[8*4];

	unsigned char	CmndBuf[CMNDBUFSIZE+1][2];
	volatile int	NumCmnd;
	int	CmndReadIdx,CmndWriteIdx;
	int CmndRate;

#if X68SOUND_ENABLE_PORTABLE_CODE
	int RateForExecuteCmnd;
	int RateForPcmset62;
	int RateForPcmset22;
	int Rate2ForPcmset22;
#endif
	
	//	short	PcmBuf[PCMBUFSIZE][2];
	short	(*PcmBuf)[2];
public:
	unsigned int PcmBufSize;
	volatile unsigned int PcmBufPtr;
private:
#if !X68SOUND_ENABLE_PORTABLE_CODE
	unsigned int	TimerID;
#endif

//	int	LfoOverTime;	// LFO tのオーバーフロー値
//	int	LfoTime;	// LFO専用 t
//	int LfoRndTime;	// LFOランダム波専用t
//	int 
//	int	Lfrq;		// LFO周波数設定値 LFRQ
//	int	LfoWaveForm;	// LFO wave form
//	inline void	CulcLfoStep();
	void SetConnection(int ch, int alg);
	volatile int	OpOut[8];
	int	OpOutDummy;

	int	TimerAreg10;	// OPMreg$10の値
	int	TimerAreg11;	// OPMreg$11の値
	int	TimerA;			// タイマーAのオーバーフロー設定値
	int	TimerAcounter;	// タイマーAのカウンター値
	int	TimerB;			// タイマーBのオーバーフロー設定値
	int	TimerBcounter;	// タイマーBのカウンター値
	volatile int	TimerReg;		// タイマー制御レジスタ (OPMreg$14の下位4ビット)
	volatile int	StatReg;		// OPMステータスレジスタ ($E90003の下位2ビット)
#if X68SOUND_ENABLE_PORTABLE_CODE
	void (*OpmIntProc)(void *);		// OPM割り込みコールバック関数
	void *OpmIntArg;
#else
	void (CALLBACK *OpmIntProc)();	// OPM割り込みコールバック関数
#endif

	double inpopmbuf_dummy;
	short InpOpmBuf0[OPMLPF_COL*2],InpOpmBuf1[OPMLPF_COL*2];
	int InpOpm_idx;
#if X68SOUND_ENABLE_PORTABLE_CODE
	int OpmLPFidx; const short *OpmLPFp;
#else
	int OpmLPFidx; short *OpmLPFp;
#endif
	double inpadpcmbuf_dummy;
//	short InpAdpcmBuf0[ADPCMLPF_COL*2],InpAdpcmBuf1[ADPCMLPF_COL*2];
//	int InpAdpcm_idx;
//	int	AdpcmLPFidx; short *AdpcmLPFp;
	int	OutOpm[2];
	int InpInpOpm[2],InpOpm[2];
	int	InpInpOpm_prev[2],InpOpm_prev[2];
	int	InpInpOpm_prev2[2],InpOpm_prev2[2];
	int OpmHpfInp[2], OpmHpfInp_prev[2], OpmHpfOut[2];
	int OutInpAdpcm[2],OutInpAdpcm_prev[2],OutInpAdpcm_prev2[2],
		OutOutAdpcm[2],OutOutAdpcm_prev[2],OutOutAdpcm_prev2[2];	// 高音フィルター２用バッファ
	int OutInpOutAdpcm[2],OutInpOutAdpcm_prev[2],OutInpOutAdpcm_prev2[2],
		OutOutInpAdpcm[2],OutOutInpAdpcm_prev[2];			// 高音フィルター３用バッファ
	
	volatile unsigned char	PpiReg;
	unsigned char	AdpcmBaseClock;	// ADPCMクロック切り替え(0:8MHz 1:4Mhz)
	void SetAdpcmRate();

	unsigned char	OpmRegNo;		// 現在指定されているOPMレジスタ番号
	unsigned char	OpmRegNo_backup;		// バックアップ用OPMレジスタ番号
#if X68SOUND_ENABLE_PORTABLE_CODE
	void (*BetwIntProc)(void *);	// マルチメディアタイマー割り込み
	void *BetwIntArg;
	int (*WaveFunc)(void *);		// WaveFunc
	void *WaveFuncArg;
#else
	void (CALLBACK *BetwIntProc)();	// マルチメディアタイマー割り込み
	int (CALLBACK *WaveFunc)();		// WaveFunc
#endif

	int	UseOpmFlag;		// OPMを利用するかどうかのフラグ
	int	UseAdpcmFlag;	// ADPCMを利用するかどうかのフラグ
	int _betw;
	int _pcmbuf;
	int _late;
	int _rev;

	int Dousa_mode;		// 0:非動作 1:X68Sound_Start中  2:X68Sound_PcmStart中


//public:
	Adpcm	adpcm;
//private:
	Pcm8	pcm8[PCM8_NCH];

//	int	TotalVolume;	// 音量 x/256


public:
#if X68SOUND_ENABLE_PORTABLE_CODE
	Opm(struct tagX68SoundContextImpl *contextImpl);
#else
	Opm(void);
#endif
	~Opm();
	void pcmset62(int ndata);
	void pcmset22(int ndata);

	int GetPcm(void *buf, int ndata);

	void timer();
	void betwint();

	void MakeTable();
	int Start(int samprate, int opmflag, int adpcmflag,
				int betw, int pcmbuf, int late, double rev);
	int StartPcm(int samprate, int opmflag, int adpcmflag, int pcmbuf);
	int SetSamprate(int samprate);
	int SetOpmClock(int clock);
	int WaveAndTimerStart();
	int SetOpmWait(int wait);
	void CulcCmndRate();
	void Reset();
	void ResetSamprate();
	void Free();
#if X68SOUND_ENABLE_PORTABLE_CODE
	void BetwInt(void (*proc)(void *), void *arg);
#else
	void BetwInt(void (CALLBACK *proc)());
#endif

	unsigned char OpmPeek();
	void OpmReg(unsigned char no);
	void OpmPoke(unsigned char data);
	void ExecuteCmnd();
#if X68SOUND_ENABLE_PORTABLE_CODE
	void OpmInt(void (*proc)(void *), void *arg);
#else
	void OpmInt(void (CALLBACK *proc)());
#endif

	unsigned char AdpcmPeek();
	void AdpcmPoke(unsigned char data);
	unsigned char PpiPeek();
	void PpiPoke(unsigned char data);
	void PpiCtrl(unsigned char data);

	unsigned char DmaPeek(unsigned char adrs);
	void DmaPoke(unsigned char adrs, unsigned char data);
#if X68SOUND_ENABLE_PORTABLE_CODE
	void DmaInt(void (*proc)(void *), void *arg);
	void DmaErrInt(void (*proc)(void *), void *arg);
	void MemReadFunc(int (*func)(unsigned char *));
#else
	void DmaInt(void (CALLBACK *proc)());
	void DmaErrInt(void (CALLBACK *proc)());
	void MemReadFunc(int (CALLBACK *func)(unsigned char *));
#endif

#if X68SOUND_ENABLE_PORTABLE_CODE
	void SetWaveFunc(int (*func)(void *), void *arg);
#else
	void SetWaveFunc(int (CALLBACK *func)());
#endif

	int Pcm8_Out(int ch, void *adrs, int mode, int len);
	int Pcm8_Aot(int ch, void *tbl, int mode, int cnt);
	int Pcm8_Lot(int ch, void *tbl, int mode);
	int Pcm8_SetMode(int ch, int mode);
	int Pcm8_GetRest(int ch);
	int Pcm8_GetMode(int ch);
	int Pcm8_Abort();

	int SetTotalVolume(int v);
	int GetTotalVolume(void);

	void PushRegs();
	void PopRegs();

};


#endif //__X68SOUND_OPM_H__
