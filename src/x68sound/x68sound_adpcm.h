#ifndef __X68SOUND_ADPCM_H__
#define __X68SOUND_ADPCM_H__

#include "x68sound_config.h"

class Adpcm {
#if X68SOUND_ENABLE_PORTABLE_CODE
public:
	struct tagX68SoundContextImpl *m_contextImpl;
private:
#endif
	int	Scale;		// 
	int Pcm;		// 16bit PCM Data
	int InpPcm,InpPcm_prev,OutPcm;		// HPF用 16bit PCM Data
	int OutInpPcm,OutInpPcm_prev;		// HPF用
	volatile int	AdpcmRate;	// 187500(15625*12), 125000(10416.66*12), 93750(7812.5*12), 62500(5208.33*12), 46875(3906.25*12), ...
	int	RateCounter;
	int	N1Data;	// ADPCM 1サンプルのデータの保存
	int N1DataFlag;	// 0 or 1

	void adpcm2pcm(unsigned char adpcm);

public:
#if X68SOUND_ENABLE_PORTABLE_CODE
	void (*IntProc)(void *);	// 割り込みアドレス
	void *IntArg;
	void (*ErrIntProc)(void *);	// 割り込みアドレス
	void *ErrIntArg;
#else
	void (CALLBACK *IntProc)();	// 割り込みアドレス
	void (CALLBACK *ErrIntProc)();	// 割り込みアドレス
#endif
//	int	AdpcmFlag;	// 0:非動作  1:再生中
//	int PpiReg;		// PPI レジスタの内容
//	int	DmaCsr;		// DMA CSR レジスタの内容
//	int	DmaCcr;		// DMA CCR レジスタの内容
//	int	DmaFlag;	// 0:DMA非動作  1:DMA動作中
	int	DmaGetByte();
	unsigned char	DmaLastValue;
	volatile unsigned char	AdpcmReg;
#if X68SOUND_ENABLE_PORTABLE_CODE
	volatile union {
		uint8_t asUint8[0x40];
		uint16_t asUint16[0x20];
		uint32_t asUint32[0x10];
	} DmaReg;
#else
	volatile unsigned char	DmaReg[0x40];
#endif
int FinishCounter;
	void DmaError(unsigned char errcode);
	void DmaFinish();
	int DmaContinueSetNextMtcMar();
	int DmaArrayChainSetNextMtcMar();
	int DmaLinkArrayChainSetNextMtcMar();

#if X68SOUND_ENABLE_PORTABLE_CODE
	Adpcm(struct tagX68SoundContextImpl *contextImpl);
#else
	Adpcm(void);
#endif
	~Adpcm() {};
	void	Init();
	void	InitSamprate();
	void Reset();
	int	GetPcm();
	int	GetPcm62();

	void	SetAdpcmRate(int rate);

};

#endif //__X68SOUND_ADPCM_H__
