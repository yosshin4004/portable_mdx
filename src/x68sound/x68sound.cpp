#include <x68sound_context.h>
#include "x68sound_config.h"

#include "x68sound_global.h"
#include "x68sound.h"
#include "x68sound_op.h"
#include "x68sound_lfo.h"
#include "x68sound_adpcm.h"
#include "x68sound_pcm8.h"
#include "x68sound_opm.h"

#if X68SOUND_ENABLE_PORTABLE_CODE
	#include "x68sound_context.internal.h"
#endif






#if X68SOUND_ENABLE_PORTABLE_CODE
	/* X68SoundContext に移動 */
#else
Opm	opm;
#endif
//MMTIME	mmt;

#if !X68SOUND_ENABLE_PORTABLE_CODE
void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, 
						  DWORD dwParam1, DWORD dwParam2) {
	if (uMsg == WOM_DONE && thread_flag) {
		timer_start_flag = 1;	// マルチメディアタイマーの処理を開始


		playingblk = (playingblk+1) & (N_waveblk-1);
		int playptr = playingblk * Blk_Samples;
		
		int genptr = opm.PcmBufPtr;
		if (genptr < playptr) {
			genptr += opm.PcmBufSize;
		}
		genptr -= playptr;
		if (genptr <= Late_Samples) {
			if (Late_Samples-Faster_Limit <= genptr) {
				// 音生成が遅れている
				nSamples = Betw_Samples_Faster;
			} else {
				// 音生成が進みすぎている
//				nSamples = Betw_Samples_VerySlower;
				// 音生成が遅れすぎている
//				setPcmBufPtr = ((playingblk+1)&(N_waveblk-1)) * Blk_Samples;
				unsigned int ptr = playptr + Late_Samples + Betw_Samples_Faster;
				while (ptr >= opm.PcmBufSize) ptr -= opm.PcmBufSize;
				setPcmBufPtr = ptr;
			}
		} else {
			if (genptr <= Late_Samples+Slower_Limit) {
				// 音生成が進んでいる
				nSamples = Betw_Samples_Slower;
			} else {
				// 音生成が進みすぎている
//				nSamples = Betw_Samples_VerySlower;
//				setPcmBufPtr = ((playingblk+1)&(N_waveblk-1)) * Blk_Samples;
				unsigned int ptr = playptr + Late_Samples + Betw_Samples_Faster;
				while (ptr >= opm.PcmBufSize) ptr -= opm.PcmBufSize;
				setPcmBufPtr = ptr;
			}
		}

		PostThreadMessage(thread_id, THREADMES_WAVEOUTDONE, 0, 0);
	}
}
#endif

#if !X68SOUND_ENABLE_PORTABLE_CODE
DWORD WINAPI waveOutThread( LPVOID ) {
	MSG Msg;

	thread_flag = 1;

	while (GetMessage( &Msg, NULL, 0, 0)) {
		if (Msg.message == THREADMES_WAVEOUTDONE) {
			
			waveOutWrite(hwo, lpwh+waveblk, sizeof(WAVEHDR));

			++waveblk;
			if (waveblk >= N_waveblk) {
				waveblk = 0;
			}

		} else if (Msg.message == THREADMES_KILL) {
			waveOutReset(hwo);
			break;
		}
	}

	thread_flag = 0;
	return 0;
}
#endif


#if !X68SOUND_ENABLE_PORTABLE_CODE
// マルチメディアタイマー
void CALLBACK OpmTimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) {
		if (!timer_start_flag) return;

//if (opm.PcmBufPtr/Blk_Samples == ((playingblk-1)&(N_waveblk-1))) return;
		if (setPcmBufPtr != -1) {
			opm.PcmBufPtr = setPcmBufPtr;
			setPcmBufPtr = -1;
		}
		
		opm.PushRegs();

		if (Samprate != 22050) {
			opm.pcmset62(nSamples);
		} else {
			opm.pcmset22(nSamples);
		}

//		opm.timer();
		opm.betwint();


		opm.PopRegs();


/*
		if (opm.adpcm.DmaReg[0x00] & 0x10) {
			if (opm.adpcm.DmaReg[0x07] & 0x08) {	// INT==1?
				if (opm.adpcm.ErrIntProc != NULL) {
					opm.adpcm.ErrIntProc();
				}
			}
		} else if (opm.adpcm.DmaReg[0x00] & 0x10) {
			if (opm.adpcm.DmaReg[0x07] & 0x08) {	// INT==1?
				if (opm.adpcm.IntProc != NULL) {
					opm.adpcm.IntProc();
				}
			}
		}
*/

}
#endif




#if X68SOUND_ENABLE_PORTABLE_CODE
int X68Sound_Start(X68SoundContext *context, int samprate, int opmflag, int adpcmflag,
				  int betw, int pcmbuf, int late, double rev) {
	return context->m_impl->m_opm.Start(samprate, opmflag, adpcmflag, betw, pcmbuf, late, rev);
}
int X68Sound_Samprate(X68SoundContext *context, int samprate) {
	return context->m_impl->m_opm.SetSamprate(samprate);
}
int X68Sound_OpmClock(X68SoundContext *context, int clock) {
	return context->m_impl->m_opm.SetOpmClock(clock);
}
void X68Sound_Reset(X68SoundContext *context) {
	context->m_impl->m_opm.Reset();
}
void X68Sound_Free(X68SoundContext *context) {
	context->m_impl->m_opm.Free();
}
void X68Sound_BetwInt(X68SoundContext *context, void (*proc)(void *), void *arg) {
	context->m_impl->m_opm.BetwInt(proc, arg);
}

int X68Sound_StartPcm(X68SoundContext *context, int samprate, int opmflag, int adpcmflag, int pcmbuf) {
	return context->m_impl->m_opm.StartPcm(samprate, opmflag, adpcmflag, pcmbuf);
}
int X68Sound_GetPcm(X68SoundContext *context, void *buf, int len) {
	return context->m_impl->m_opm.GetPcm(buf, len);
}

unsigned char X68Sound_OpmPeek(X68SoundContext *context) {
	return context->m_impl->m_opm.OpmPeek();
}
void X68Sound_OpmReg(X68SoundContext *context, unsigned char no) {
	context->m_impl->m_opm.OpmReg(no);
}
void X68Sound_OpmPoke(X68SoundContext *context, unsigned char data) {
	context->m_impl->m_opm.OpmPoke(data);
}
void X68Sound_OpmInt(X68SoundContext *context, void (*proc)(void *), void *arg) {
	context->m_impl->m_opm.OpmInt(proc, arg);
}
int X68Sound_OpmWait(X68SoundContext *context, int wait) {
	return context->m_impl->m_opm.SetOpmWait(wait);
}

unsigned char X68Sound_AdpcmPeek(X68SoundContext *context) {
	return context->m_impl->m_opm.AdpcmPeek();
}
void X68Sound_AdpcmPoke(X68SoundContext *context, unsigned char data) {
	context->m_impl->m_opm.AdpcmPoke(data);
}
unsigned char X68Sound_PpiPeek(X68SoundContext *context) {
	return context->m_impl->m_opm.PpiPeek();
}
void X68Sound_PpiPoke(X68SoundContext *context, unsigned char data) {
	context->m_impl->m_opm.PpiPoke(data);
}
void X68Sound_PpiCtrl(X68SoundContext *context, unsigned char data) {
	context->m_impl->m_opm.PpiCtrl(data);
}
unsigned char X68Sound_DmaPeek(X68SoundContext *context, unsigned char adrs) {
	return context->m_impl->m_opm.DmaPeek(adrs);
}
void X68Sound_DmaPoke(X68SoundContext *context, unsigned char adrs, unsigned char data) {
	context->m_impl->m_opm.DmaPoke(adrs, data);
}
void X68Sound_DmaInt(X68SoundContext *context, void (*proc)(void *), void *arg) {
	context->m_impl->m_opm.DmaInt(proc, arg);
}
void X68Sound_DmaErrInt(X68SoundContext *context, void (*proc)(void *), void *arg) {
	context->m_impl->m_opm.DmaErrInt(proc, arg);
}
void X68Sound_MemReadFunc(X68SoundContext *context, int (*func)(unsigned char *)) {
	context->m_impl->m_opm.MemReadFunc(func);
}

void X68Sound_WaveFunc(X68SoundContext *context, int (*func)(void *), void *arg) {
	context->m_impl->m_opm.SetWaveFunc(func, arg);
}

int X68Sound_Pcm8_Out(X68SoundContext *context, int ch, void *adrs, int mode, int len) {
	return context->m_impl->m_opm.Pcm8_Out(ch, adrs, mode, len);
}
int X68Sound_Pcm8_Aot(X68SoundContext *context, int ch, void *tbl, int mode, int cnt) {
	return context->m_impl->m_opm.Pcm8_Aot(ch, tbl, mode, cnt);
}
int X68Sound_Pcm8_Lot(X68SoundContext *context, int ch, void *tbl, int mode) {
	return context->m_impl->m_opm.Pcm8_Lot(ch, tbl, mode);
}
int X68Sound_Pcm8_SetMode(X68SoundContext *context, int ch, int mode) {
	return context->m_impl->m_opm.Pcm8_SetMode(ch, mode);
}
int X68Sound_Pcm8_GetRest(X68SoundContext *context, int ch) {
	return context->m_impl->m_opm.Pcm8_GetRest(ch);
}
int X68Sound_Pcm8_GetMode(X68SoundContext *context, int ch) {
	return context->m_impl->m_opm.Pcm8_GetMode(ch);
}
int X68Sound_Pcm8_Abort(X68SoundContext *context) {
	return context->m_impl->m_opm.Pcm8_Abort();
}


int X68Sound_TotalVolume(X68SoundContext *context, int v) {
	return context->m_impl->m_opm.SetTotalVolume(v);
}


int X68Sound_ErrorCode(X68SoundContext *context) {
	return context->m_impl->m_ErrorCode;
}
int X68Sound_DebugValue(X68SoundContext *context) {
	return context->m_impl->m_DebugValue;
}
#else
extern "C" int X68Sound_Start(int samprate, int opmflag, int adpcmflag,
				  int betw, int pcmbuf, int late, double rev) {
	return opm.Start(samprate, opmflag, adpcmflag, betw, pcmbuf, late, rev);
}
extern "C" int X68Sound_Samprate(int samprate) {
	return opm.SetSamprate(samprate);
}
extern "C" int X68Sound_OpmClock(int clock) {
	return opm.SetOpmClock(clock);
}
extern "C" void X68Sound_Reset() {
	opm.Reset();
}
extern "C" void X68Sound_Free() {
	opm.Free();
}
extern "C" void X68Sound_BetwInt(void (CALLBACK *proc)()) {
	opm.BetwInt(proc);
}

extern "C" int X68Sound_StartPcm(int samprate, int opmflag, int adpcmflag, int pcmbuf) {
	return opm.StartPcm(samprate, opmflag, adpcmflag, pcmbuf);
}
extern "C" int X68Sound_GetPcm(void *buf, int len) {
	return opm.GetPcm(buf, len);
}

extern "C" unsigned char X68Sound_OpmPeek() {
	return opm.OpmPeek();
}
extern "C" void X68Sound_OpmReg(unsigned char no) {
	opm.OpmReg(no);
}
extern "C" void X68Sound_OpmPoke(unsigned char data) {
	opm.OpmPoke(data);
}
extern "C" void X68Sound_OpmInt(void (CALLBACK *proc)()) {
	opm.OpmInt(proc);
}
extern "C" int X68Sound_OpmWait(int wait) {
	return opm.SetOpmWait(wait);
}

extern "C" unsigned char X68Sound_AdpcmPeek() {
	return opm.AdpcmPeek();
}
extern "C" void X68Sound_AdpcmPoke(unsigned char data) {
	opm.AdpcmPoke(data);
}
extern "C" unsigned char X68Sound_PpiPeek() {
	return opm.PpiPeek();
}
extern "C" void X68Sound_PpiPoke(unsigned char data) {
	opm.PpiPoke(data);
}
extern "C" void X68Sound_PpiCtrl(unsigned char data) {
	opm.PpiCtrl(data);
}
extern "C" unsigned char X68Sound_DmaPeek(unsigned char adrs) {
	return opm.DmaPeek(adrs);
}
extern "C" void X68Sound_DmaPoke(unsigned char adrs, unsigned char data) {
	opm.DmaPoke(adrs, data);
}
extern "C" void X68Sound_DmaInt(void (CALLBACK *proc)()) {
	opm.DmaInt(proc);
}
extern "C" void X68Sound_DmaErrInt(void (CALLBACK *proc)()) {
	opm.DmaErrInt(proc);
}
extern "C" void X68Sound_MemReadFunc(int (CALLBACK *func)(unsigned char *)) {
	opm.MemReadFunc(func);
}

extern "C" void X68Sound_WaveFunc(int (CALLBACK *func)()) {
	opm.SetWaveFunc(func);
}

extern "C" int X68Sound_Pcm8_Out(int ch, void *adrs, int mode, int len) {
	return opm.Pcm8_Out(ch, adrs, mode, len);
}
extern "C" int X68Sound_Pcm8_Aot(int ch, void *tbl, int mode, int cnt) {
	return opm.Pcm8_Aot(ch, tbl, mode, cnt);
}
extern "C" int X68Sound_Pcm8_Lot(int ch, void *tbl, int mode) {
	return opm.Pcm8_Lot(ch, tbl, mode);
}
extern "C" int X68Sound_Pcm8_SetMode(int ch, int mode) {
	return opm.Pcm8_SetMode(ch, mode);
}
extern "C" int X68Sound_Pcm8_GetRest(int ch) {
	return opm.Pcm8_GetRest(ch);
}
extern "C" int X68Sound_Pcm8_GetMode(int ch) {
	return opm.Pcm8_GetMode(ch);
}
extern "C" int X68Sound_Pcm8_Abort() {
	return opm.Pcm8_Abort();
}


extern "C" int X68Sound_TotalVolume(int v) {
	return opm.SetTotalVolume(v);
}








extern "C" int X68Sound_ErrorCode() {
	return ErrorCode;
}
extern "C" int X68Sound_DebugValue() {
	return DebugValue;
}
#endif
