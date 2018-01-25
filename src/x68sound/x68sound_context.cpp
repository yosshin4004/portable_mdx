// Copyright (C) m_puusan
/* Copyright (C) 2018 Yosshin(@yosshin4004) */

#include <new>			/* for placement new/delete */
#include <string.h>		/* for memset */
#include <x68sound_context.h>
#include "x68sound_config.h"
#include "x68sound_global.h"
#include "x68sound_opm.h"
#include "x68sound_context.internal.h"

static bool X68SoundContextImpl_Initialize(
	X68SoundContextImpl *impl,
	void *dmaBase
){
	memset(impl, 0, sizeof(*impl));

	impl->m_dmaBase = dmaBase;

	impl->m_OpmFir = OpmFir_Normal;

	impl->m_DebugValue = 0;
	impl->m_ErrorCode = 0;
	impl->m_Samprate = 22050;
	impl->m_WaveOutSamp = 22050;
	impl->m_OpmWait = 240;	// 24.0μｓ
	impl->m_OpmRate = 62500;	// 入力クロック÷64

	impl->m_MemRead = MemReadDefault;
	impl->m_Semapho = 0;
	impl->m_TimerSemapho = 0;
	impl->m_OPMLPF_ROW = OPMLPF_ROW_44;
	impl->m_OPMLOWPASS = OPMLOWPASS_44;

	impl->m_TimerResolution = 1;

	impl->m_N_waveblk = 4;
	impl->m_waveblk = 0;
	impl->m_playingblk = 0;
	impl->m_playingblk_next = 1;
	impl->m_setPcmBufPtr = -1;

	impl->m_RandSeed = 1;

	new(&impl->m_opm) Opm(impl);

	return true;
}

static bool X68SoundContextImpl_Terminate(
	X68SoundContextImpl *impl
){
	impl->m_opm.~Opm();

	return true;
}

bool X68SoundContext_Initialize(
	X68SoundContext *context,
	void *dmaBase
){
	memset(context, 0, sizeof(*context));

	context->m_impl = (X68SoundContextImpl *)malloc(sizeof(X68SoundContextImpl));
	if (context->m_impl == NULL) return false;

	if (X68SoundContextImpl_Initialize(context->m_impl, dmaBase) == false) {
		free(context->m_impl);
		context->m_impl = NULL;
		return false;
	}

	return true;
}

bool X68SoundContext_Terminate(X68SoundContext *context){
	if (context->m_impl == NULL) return false;
	if (X68SoundContextImpl_Terminate(context->m_impl) == false) return false;
	free(context->m_impl);
	context->m_impl = NULL;

	return true;
}

