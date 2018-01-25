/* Copyright (C) 2018 Yosshin(@yosshin4004) */

#ifndef __X68SOUND_CONTEXT_INTERNAL_H__
#define __X68SOUND_CONTEXT_INTERNAL_H__

#include <x68sound_context.h>
#include "x68sound_config.h"
#include "x68sound_opm.h"

typedef struct tagX68SoundContextImpl {
	void *m_dmaBase;

	void (*m_OpmFir)(const short *, const short *, const short *, int *);

	int m_DebugValue;
	int m_ErrorCode;
	int m_Samprate;
	int m_WaveOutSamp;
	int m_OpmWait;
	int m_OpmRate;

	int m_STEPTBL[11*12*64];
	#define ALPHAZERO	(SIZEALPHATBL*3)
	unsigned short m_ALPHATBL[ALPHAZERO+SIZEALPHATBL+1];
	short m_SINTBL[SIZESINTBL];

	int m_D1LTBL[16];
	int m_DT1TBL[128+4];

	unsigned short	m_NOISEALPHATBL[ALPHAZERO+SIZEALPHATBL+1];

	int (*m_MemRead)(unsigned char *adrs);

	int m_TotalVolume;	// 音量 x/256

	volatile int m_Semapho;
	volatile int m_TimerSemapho;

	int m_OPMLPF_ROW;
	const short (*m_OPMLOWPASS)[OPMLPF_COL];

	int m_Betw_Time;		// 5 ms
	int m_Late_Time;		// (200+Bet_time) ms
	int m_Late_Samples;	// (44100*Late_Time/1000)
	int m_Blk_Samples;	// 44100/N_waveblk
	int m_Betw_Samples_Slower;	// floor(44100.0*5/1000.0-rev)
	int m_Betw_Samples_Faster;	// ceil(44100.0*5/1000.0+rev)
	int m_Betw_Samples_VerySlower;	// floor(44100.0*5/1000.0-rev)/4.0
	int m_Slower_Limit,m_Faster_Limit;
	unsigned int	m_TimerResolution;
	//unsigned int	m_SamplesCounter=0;
	//unsigned int	m_SamplesCounterRev=0;
	unsigned int	m_nSamples;

	//const int m_N_waveblk=8;
	int m_N_waveblk;
	int m_waveblk;
	int m_playingblk, m_playingblk_next;
	volatile int m_setPcmBufPtr;

	int m_RandSeed;

	Opm m_opm;	/* 配置 new delete が必要 */
} X68SoundContextImpl;

static inline uint32_t X68SoundContextImpl_ToOfs(
	const X68SoundContextImpl *contextImpl,
	const volatile void *ptr
){
	uintptr_t ofs = (uintptr_t)ptr - (uintptr_t)contextImpl->m_dmaBase;
	assert(ofs < 0x100000000LL);
	return (ptr ? (uint32_t)ofs : 0);
}

static inline uint8_t *X68SoundContextImpl_ToPtr(
	const X68SoundContextImpl *contextImpl,
	uint32_t ofs
){
	return (ofs ? ((uint8_t *)(((uintptr_t)contextImpl->m_dmaBase) + (ofs))) : NULL);
}

static inline unsigned int irnd(X68SoundContextImpl *contextImpl){
	contextImpl->m_RandSeed = contextImpl->m_RandSeed * 1566083941UL + 1;
	return contextImpl->m_RandSeed;
}

#define TO_OFS( ptr ) X68SoundContextImpl_ToOfs( m_contextImpl , ptr )
#define TO_PTR( ofs ) X68SoundContextImpl_ToPtr( m_contextImpl , ofs )

#define OpmFir					(m_contextImpl->m_OpmFir)
#define DebugValue				(m_contextImpl->m_DebugValue)
#define ErrorCode				(m_contextImpl->m_ErrorCode)
#define Samprate				(m_contextImpl->m_Samprate)
#define WaveOutSamp				(m_contextImpl->m_WaveOutSamp)
#define OpmWait					(m_contextImpl->m_OpmWait)
#define OpmRate					(m_contextImpl->m_OpmRate)
#define STEPTBL					(m_contextImpl->m_STEPTBL)
#define ALPHATBL				(m_contextImpl->m_ALPHATBL)
#define SINTBL					(m_contextImpl->m_SINTBL)
#define D1LTBL					(m_contextImpl->m_D1LTBL)
#define DT1TBL					(m_contextImpl->m_DT1TBL)
#define NOISEALPHATBL			(m_contextImpl->m_NOISEALPHATBL)
#define MemRead					(m_contextImpl->m_MemRead)
#define TotalVolume				(m_contextImpl->m_TotalVolume)
#define Semapho					(m_contextImpl->m_Semapho)
#define TimerSemapho			(m_contextImpl->m_TimerSemapho)
#define OPMLPF_ROW				(m_contextImpl->m_OPMLPF_ROW)
#define OPMLOWPASS				(m_contextImpl->m_OPMLOWPASS)
#define Betw_Time				(m_contextImpl->m_Betw_Time)
#define Late_Time				(m_contextImpl->m_Late_Time)
#define Late_Samples			(m_contextImpl->m_Late_Samples)
#define Blk_Samples				(m_contextImpl->m_Blk_Samples)
#define Betw_Samples_Slower		(m_contextImpl->m_Betw_Samples_Slower)
#define Betw_Samples_Faster		(m_contextImpl->m_Betw_Samples_Faster)
#define Betw_Samples_VerySlower	(m_contextImpl->m_Betw_Samples_VerySlower)
#define Slower_Limit			(m_contextImpl->m_Slower_Limit)
#define Faster_Limit			(m_contextImpl->m_Faster_Limit)
#define TimerResolution			(m_contextImpl->m_TimerResolution)
#define nSamples				(m_contextImpl->m_nSamples)
#define N_waveblk				(m_contextImpl->m_N_waveblk)
#define waveblk					(m_contextImpl->m_waveblk)
#define playingblk				(m_contextImpl->m_playingblk)
#define playingblk_next			(m_contextImpl->m_playingblk_next)
#define setPcmBufPtr			(m_contextImpl->m_setPcmBufPtr)

#endif //__X68SOUND_CONTEXT_INTERNAL_H__
