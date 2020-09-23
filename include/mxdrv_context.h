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

/* クリティカルセクションに入る */
void MxdrvContext_EnterCriticalSection(
	MxdrvContext *context
);

/* クリティカルセクションから出る */
void MxdrvContext_LeaveCriticalSection(
	MxdrvContext *context
);

#ifdef __cplusplus
}
#endif

#endif //__MXDRV_CONTEXT_H__
