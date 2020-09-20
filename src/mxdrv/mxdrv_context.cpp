/* Copyright (C) 2018 Yosshin(@yosshin4004) */

#include <string.h>		/* for memset */
#include <mxdrv.h>
#include <mxdrv_context.h>
#include "mxdrv_config.h"
#include "mxdrv_context.internal.h"

void MxdrvContextImpl_ResetMemoryPool(
	MxdrvContextImpl *impl
){
	impl->m_memoryPoolReserved = &impl->m_memoryPool[0];
}

uint8_t *MxdrvContextImpl_ReserveMemory(
	MxdrvContextImpl *impl,
	uint32_t sizeInBytes
){
	uint8_t *p = impl->m_memoryPoolReserved;
	if (impl->m_memoryPoolReserved + sizeInBytes > &impl->m_memoryPool[impl->m_memoryPoolSizeInBytes]) {
		return NULL;
	}
	impl->m_memoryPoolReserved += sizeInBytes;
	return p;
}

bool MxdrvContextImpl_ReleaseMemory(
	MxdrvContextImpl *impl,
	uint32_t sizeInBytes
){
	if (impl->m_memoryPoolReserved < &impl->m_memoryPool[sizeInBytes]) return false;
	impl->m_memoryPoolReserved -= sizeInBytes;
	return true;
}

uint32_t MxdrvContextImpl_GetReservedMemoryPoolSize(
	MxdrvContextImpl *impl
){
	uintptr_t sizeInBytes = (uintptr_t)impl->m_memoryPoolReserved - (uintptr_t)impl->m_memoryPool;
	assert(sizeInBytes < 0x100000000LL);
	return (uint32_t)sizeInBytes;
}

void MxdrvContextImpl_EnterCriticalSection(
	MxdrvContextImpl *impl
){
	impl->m_mtx.lock();
}

void MxdrvContextImpl_LeaveCriticalSection(
	MxdrvContextImpl *impl
){
	impl->m_mtx.unlock();
}

static bool MxdrvContextImpl_Initialize(
	MxdrvContextImpl *impl,
	uint32_t allocSizeInBytes
){
	memset(impl, 0, sizeof(*impl));

	impl->m_L001190 = 0x1234;
	impl->m_L0019b2[0] = 0x7f;
	impl->m_L0019b2[1] = 0xf1;
	impl->m_L0019b2[2] = 0x00;
	{
		static const UBYTE Volume[] = {
			0x2a,0x28,0x25,0x22,0x20,0x1d,0x1a,0x18,
			0x15,0x12,0x10,0x0d,0x0a,0x08,0x05,0x02,
		};
		memcpy(impl->m_L000e7eVolume, Volume, sizeof(Volume));
	}
	impl->m_AdpcmStat=0; // $02:adpcmout $12:adpcmaot $22:adpcmlot $32:adpcmcot
	impl->m_OpmReg1B=0;  // OPM レジスタ $1B の内容
	impl->m_DmaErrCode = 0;
	impl->m_Adpcmcot_adrs = NULL;
	impl->m_Adpcmcot_len = 0;
	impl->m_OpmIntProc = NULL;
	impl->m_OpmIntArg = NULL;
	impl->m_mdxReservedMemoryPoolSize = 0;
	impl->m_pdxReservedMemoryPoolSize = 0;
	impl->m_memoryPoolSizeInBytes = allocSizeInBytes - sizeof(MxdrvContextImpl);
	impl->m_memoryPoolReserved = NULL;
	new(&impl->m_mtx) std::mutex();

	if (X68SoundContext_Initialize(&impl->m_x68SoundContext, impl) == false) return false;

	return true;
}

static bool MxdrvContextImpl_Terminate(
	MxdrvContextImpl *impl
){
	if (X68SoundContext_Terminate(&impl->m_x68SoundContext) == false) return false;
	impl->m_mtx.~mutex();

	return true;
}

bool MxdrvContext_GetOpmReg(
	MxdrvContext *context,
	uint8_t regIndex,
	uint8_t *regVal,
	bool *updated
){
	if (context->m_impl == NULL) return false;
	if (regVal != NULL) *regVal = context->m_impl->m_opmRegs[regIndex];
	if (updated != NULL) {
		*updated = context->m_impl->m_opmRegsUpdated[regIndex];
		context->m_impl->m_opmRegsUpdated[regIndex] = false;
	}
	return true;
}

bool MxdrvContext_GetFmKeyOn(
	MxdrvContext *context,
	uint8_t channelIndex,
	bool *currentKeyOn,
	bool *logicalSumOfKeyOn
){
	if (context->m_impl == NULL) return false;
	if (channelIndex > 7) return false;
	if (currentKeyOn != NULL) *currentKeyOn = context->m_impl->m_keyOnFlagsForFm[channelIndex];
	if (logicalSumOfKeyOn != NULL) {
		*logicalSumOfKeyOn = context->m_impl->m_logicalSumOfKeyOnFlagsForFm[channelIndex];
		context->m_impl->m_logicalSumOfKeyOnFlagsForFm[channelIndex] = false;
	}
	return true;
}

bool MxdrvContext_GetPcmKeyOn(
	MxdrvContext *context,
	uint8_t channelIndex,
	bool *logicalSumOfKeyOn
){
	if (context->m_impl == NULL) return false;
	if (channelIndex > 7) return false;
	if (logicalSumOfKeyOn != NULL) {
		*logicalSumOfKeyOn = context->m_impl->m_logicalSumOfKeyOnFlagsForPcm[channelIndex];
		context->m_impl->m_logicalSumOfKeyOnFlagsForPcm[channelIndex] = false;
	}
	return true;
}

bool MxdrvContext_Initialize(
	MxdrvContext *context,
	int memoryPoolSizeInBytes
){
	memset(context, 0, sizeof(*context));

	int allocSizeInBytes = sizeof(MxdrvContextImpl) + memoryPoolSizeInBytes;
	context->m_impl = (MxdrvContextImpl *)malloc(allocSizeInBytes);
	if (context->m_impl == NULL) return false;

	if (MxdrvContextImpl_Initialize(context->m_impl, allocSizeInBytes) == false) {
		free(context->m_impl);
		context->m_impl = NULL;
		return false;
	}

	return true;
}

bool MxdrvContext_Terminate(
	MxdrvContext *context
){
	if (context->m_impl == NULL) return false;
	if (MxdrvContextImpl_Terminate(context->m_impl) == false) return false;
	free(context->m_impl);
	context->m_impl = NULL;

	return true;
}
