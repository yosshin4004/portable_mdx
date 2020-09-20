// Copyright (C) 2000 GORRY.
/* Copyright (C) 2018 Yosshin(@yosshin4004) */

#ifndef __MXDRV_CONTEXT_INTERNAL_H__
#define __MXDRV_CONTEXT_INTERNAL_H__

#include <mutex>
#include <mxdrv.h>
#include <mxdrv_context.h>
#include <x68sound_context.h>
#include "mxdrv_config.h"
#include "mxdrv_depend.h"

typedef struct tagMxdrvContextImpl {
	ULONG m_D0;
	ULONG m_D1;
	ULONG m_D2;
	ULONG m_D3;
	ULONG m_D4;
	ULONG m_D5;
	ULONG m_D6;
	ULONG m_D7;
	ULONG m_A0;
	ULONG m_A1;
	ULONG m_A2;
	ULONG m_A3;
	ULONG m_A4;
	ULONG m_A5;
	ULONG m_A6;
	ULONG m_A7;

	volatile MXWORK_CH m_MXWORK_CHBUF_FM[9];
	volatile MXWORK_CH m_MXWORK_CHBUF_PCM[7];
	volatile MXWORK_GLOBAL m_MXWORK_GLOBALBUF;
	volatile MXWORK_KEY m_MXWORK_KEYBUF;
	volatile MXWORK_OPM m_MXWORK_OPMBUF;
	volatile UBYTE m_MXWORK_PCM8;
	UBYTE m_FAKEA6S0004[256];
	UBYTE m_DisposeStack_L00122e;

	void(*m_OPMINT_FUNC)( MxdrvContext *context );
	void (*m_MXCALLBACK_OPMINT)( MxdrvContext *context );

	BOOL volatile m_MeasurePlayTime;
	BOOL volatile m_TerminatePlay;
	int m_LoopCount;
	int m_LoopLimit;
	BOOL m_FadeoutStart;
	BOOL m_ReqFadeout;
	UWORD m_L001190;
	UBYTE m_L0019b2[3];
	UBYTE m_L000e7eVolume[16];

	/* sound_iocs.h 由来のメンバ */
	volatile unsigned char m_AdpcmStat; // $02:adpcmout $12:adpcmaot $22:adpcmlot $32:adpcmcot
	volatile unsigned char m_OpmReg1B;  // OPM レジスタ $1B の内容
	volatile unsigned char m_DmaErrCode;

	volatile unsigned char *m_Adpcmcot_adrs;
	volatile int m_Adpcmcot_len;

	void (*m_OpmIntProc)(void *);	// OPMのタイマー割り込み処理アドレス
	void *m_OpmIntArg;

	/* x68sound 由来のメンバ */
	X68SoundContext m_x68SoundContext;

	/* メモリプール上に確保した mdx pdx のコピーのサイズ */
	uint32_t m_mdxReservedMemoryPoolSize;
	uint32_t m_pdxReservedMemoryPoolSize;

	/* OPM レジスタに書き込んだ値のコピー */
	uint8_t m_opmRegs[0x100];

	/* OPM レジスタに更新があったか？ */
	bool m_opmRegsUpdated[0x100];

	/* KeyOn の状態 */
	bool m_keyOnFlagsForFm[8];
	bool m_logicalSumOfKeyOnFlagsForFm[8];
	bool m_logicalSumOfKeyOnFlagsForPcm[8];

	/* 再入防止用 */
	std::mutex m_mtx;	/* 配置 new delete が必要 */

	/*
		メモリプール領域は、MxdrvContextImpl 構造体から相対アドレス 32bit の
		範囲に置く必要がある。以下のように要素数省略配列として保持する必要がある。
		ポインタに置き換えてはいけない。
	*/
	uint32_t m_memoryPoolSizeInBytes;
	uint8_t *m_memoryPoolReserved;
	uint8_t m_memoryPool[1];		/* 要素数を省略したいが VC で警告されるので */
	/*
		!!! 注意 !!!
		ここ以降にメンバを追加してはならない。
	*/
} MxdrvContextImpl;

void MxdrvContextImpl_ResetMemoryPool(
	MxdrvContextImpl *impl
);

uint8_t *MxdrvContextImpl_ReserveMemory(
	MxdrvContextImpl *impl,
	uint32_t sizeInBytes
);

bool MxdrvContextImpl_ReleaseMemory(
	MxdrvContextImpl *impl,
	uint32_t sizeInBytes
);

uint32_t MxdrvContextImpl_GetReservedMemoryPoolSize(
	MxdrvContextImpl *impl
);

void MxdrvContextImpl_EnterCriticalSection(
	MxdrvContextImpl *impl
);

void MxdrvContextImpl_LeaveCriticalSection(
	MxdrvContextImpl *impl
);

#define TO_OFS( ptr ) MxdrvContext_ToOfs( context , ptr )
#define TO_PTR( ofs ) MxdrvContext_ToPtr( context , ofs )

#define D0						(context->m_impl->m_D0)
#define D1						(context->m_impl->m_D1)
#define D2						(context->m_impl->m_D2)
#define D3						(context->m_impl->m_D3)
#define D4						(context->m_impl->m_D4)
#define D5						(context->m_impl->m_D5)
#define D6						(context->m_impl->m_D6)
#define D7						(context->m_impl->m_D7)
#define A0						(context->m_impl->m_A0)
#define A1						(context->m_impl->m_A1)
#define A2						(context->m_impl->m_A2)
#define A3						(context->m_impl->m_A3)
#define A4						(context->m_impl->m_A4)
#define A5						(context->m_impl->m_A5)
#define A6						(context->m_impl->m_A6)
#define A7						(context->m_impl->m_A7)
#define MXWORK_CHBUF_FM			(context->m_impl->m_MXWORK_CHBUF_FM)
#define MXWORK_CHBUF_PCM		(context->m_impl->m_MXWORK_CHBUF_PCM)
#define MXWORK_GLOBALBUF		(context->m_impl->m_MXWORK_GLOBALBUF)
#define G						(context->m_impl->m_MXWORK_GLOBALBUF)
#define MXWORK_KEYBUF			(context->m_impl->m_MXWORK_KEYBUF)
#define KEY						(context->m_impl->m_MXWORK_KEYBUF)
#define MXWORK_OPMBUF			(context->m_impl->m_MXWORK_OPMBUF)
#define OPMBUF					(context->m_impl->m_MXWORK_OPMBUF)
#define MXWORK_PCM8				(context->m_impl->m_MXWORK_PCM8)
#define PCM8					(context->m_impl->m_MXWORK_PCM8)
#define FAKEA6S0004				(context->m_impl->m_FAKEA6S0004)
#define DisposeStack_L00122e	(context->m_impl->m_DisposeStack_L00122e)
#define OPMINT_FUNC				(context->m_impl->m_OPMINT_FUNC)
#define MXCALLBACK_OPMINT		(context->m_impl->m_MXCALLBACK_OPMINT)
#define MeasurePlayTime			(context->m_impl->m_MeasurePlayTime)
#define TerminatePlay			(context->m_impl->m_TerminatePlay)
#define LoopCount				(context->m_impl->m_LoopCount)
#define LoopLimit				(context->m_impl->m_LoopLimit)
#define FadeoutStart			(context->m_impl->m_FadeoutStart)
#define ReqFadeout				(context->m_impl->m_ReqFadeout)
#define L001190					(context->m_impl->m_L001190)
#define L0019b2					(context->m_impl->m_L0019b2)
#define L000e7eVolume			(context->m_impl->m_L000e7eVolume)
#define OpmReg1B				(context->m_impl->m_OpmReg1B)

#endif //__MXDRV_CONTEXT_INTERNAL_H__
