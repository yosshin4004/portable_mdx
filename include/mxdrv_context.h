/* Copyright (C) 2018 Yosshin(@yosshin4004) */

#ifndef __MXDRV_CONTEXT_H__
#define __MXDRV_CONTEXT_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagMxdrvContext {
	struct tagMxdrvContextImpl *m_impl;
} MxdrvContext;

/* コンテキストの初期化 */
bool MxdrvContext_Initialize(
	MxdrvContext *context,
	int memoryPoolSizeInBytes
);

/* コンテキストの終了 */
bool MxdrvContext_Terminate(
	MxdrvContext *context
);

/* OPM レジスタ値の取得 */
bool MxdrvContext_GetOpmReg(
	MxdrvContext *context,
	uint8_t regIndex,
	uint8_t *regVal,
	bool *updated
);

/* FM チャンネルのキーオンを取得 */
bool MxdrvContext_GetFmKeyOn(
	MxdrvContext *context,
	uint8_t channelIndex,	/* 0~7 */
	bool *currentKeyOn,
	bool *logicalSumOfKeyOn
);

/* PCM チャンネルのキーオンを取得 */
bool MxdrvContext_GetPcmKeyOn(
	MxdrvContext *context,
	uint8_t channelIndex,	/* 0~7 */
	bool *logicalSumOfKeyOn
);

/* ポインタを 32 bit オフセットに変換 */
static inline uint32_t MxdrvContext_ToOfs(
	const MxdrvContext *context,
	const volatile void *ptr
){
	uintptr_t ofs = (uintptr_t)ptr - (uintptr_t)context->m_impl;
	assert(ofs < 0x100000000LL);
	return (ptr ? (uint32_t)ofs : 0);
}

/* 32 bit オフセットをポインタに変換 */
static inline uint8_t *MxdrvContext_ToPtr(
	const MxdrvContext *context,
	uint32_t ofs
){
	return (ofs ? ((uint8_t *)(((uintptr_t)context->m_impl) + (ofs))) : NULL);
}

#ifdef __cplusplus
}
#endif

#endif //__MXDRV_CONTEXT_H__
