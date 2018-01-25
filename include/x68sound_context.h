/* Copyright (C) 2018 Yosshin(@yosshin4004) */

#ifndef __X68SOUND_CONTEXT_H__
#define __X68SOUND_CONTEXT_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagX68SoundContext {
	struct tagX68SoundContextImpl *m_impl;
} X68SoundContext;

bool X68SoundContext_Initialize(
	X68SoundContext *context,
	void *dmaBase
);
bool X68SoundContext_Terminate(
	X68SoundContext *context
);

#ifdef __cplusplus
}
#endif

#endif //__X68SOUND_CONTEXT_H__
