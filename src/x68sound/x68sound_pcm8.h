#ifndef __X68SOUND_PCM8_H__
#define __X68SOUND_PCM8_H__

#include "x68sound_config.h"

class Pcm8 {
#if X68SOUND_ENABLE_PORTABLE_CODE
public:
	struct tagX68SoundContextImpl *m_contextImpl;
private:
#endif
	int	Scale;		// 
	int Pcm;		// 16bit PCM Data
	int Pcm16Prev;	// 16bit,8bitPCMの1つ前のデータ
	int InpPcm,InpPcm_prev,OutPcm;		// HPF用 16bit PCM Data
	int OutInpPcm,OutInpPcm_prev;		// HPF用
	int	AdpcmRate;	// 187500(15625*12), 125000(10416.66*12), 93750(7812.5*12), 62500(5208.33*12), 46875(3906.25*12), ...
	int	RateCounter;
	int	N1Data;	// ADPCM 1サンプルのデータの保存
	int N1DataFlag;	// 0 or 1

	volatile int	Mode;
	volatile int	Volume;	// x/16
	volatile int	PcmKind;	// 0～4:ADPCM  5:16bitPCM  6:8bitPCM  7:謎

	void adpcm2pcm(unsigned char adpcm);
	void pcm16_2pcm(int pcm16);

public:
	int	DmaGetByte();
	unsigned char	DmaLastValue;
	unsigned char	AdpcmReg;

	volatile unsigned char *DmaMar;
	volatile unsigned int DmaMtc;
	volatile unsigned char *DmaBar;
	volatile unsigned int DmaBtc;
	volatile int	DmaOcr;				// 0:チェイン動作なし 0x08:アレイチェイン 0x0C:リンクアレイチェイン


	int DmaArrayChainSetNextMtcMar();
	int DmaLinkArrayChainSetNextMtcMar();

#if X68SOUND_ENABLE_PORTABLE_CODE
	Pcm8(struct tagX68SoundContextImpl *contextImpl);
#else
	Pcm8(void);
#endif
	~Pcm8() {};
	void	Init();
	void	InitSamprate();
	void	Reset();
	int		GetPcm();
	int		GetPcm62();

	int		Out(void *adrs, int mode, int len);
	int		Aot(void *tbl, int mode, int cnt);
	int		Lot(void *tbl, int mode);
	int		SetMode(int mode);
	int		GetRest();
	int		GetMode();


};


#endif //__X68SOUND_PCM8_H__
