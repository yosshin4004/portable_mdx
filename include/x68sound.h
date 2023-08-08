#ifndef __X68SOUND_H__
#define __X68SOUND_H__

#ifdef __cplusplus
extern "C" {
#endif

#define X68SNDERR_PCMOUT		(-1)
#define X68SNDERR_TIMER			(-2)
#define X68SNDERR_MEMORY		(-3)
#define X68SNDERR_NOTACTIVE		(-4)
#define X68SNDERR_ALREADYACTIVE	(-5)
#define X68SNDERR_BADARG		(-6)

#define X68SNDERR_DLL			(-1)
#define X68SNDERR_FUNC			(-2)

int X68Sound_Start(struct tagX68SoundContext *context, int samprate/*=44100*/, int opmflag/*=1*/, int adpcmflag/*=1*/,
		  int betw/*=5*/, int pcmbuf/*=5*/, int late/*=200*/, double rev/*=1.0*/);
int X68Sound_Samprate(struct tagX68SoundContext *context, int samprate/*=44100*/);
void X68Sound_Reset(struct tagX68SoundContext *context);
void X68Sound_Free(struct tagX68SoundContext *context);
void X68Sound_BetwInt(struct tagX68SoundContext *context, void (*proc)(void *), void *arg);

int X68Sound_StartPcm(struct tagX68SoundContext *context, int samprate/*=44100*/, int opmflag/*=1*/, int adpcmflag/*=1*/, int pcmbuf/*=5*/);
int X68Sound_GetPcm(struct tagX68SoundContext *context, void *buf, int len);

unsigned char X68Sound_OpmPeek(struct tagX68SoundContext *context);
void X68Sound_OpmReg(struct tagX68SoundContext *context, unsigned char no);
void X68Sound_OpmPoke(struct tagX68SoundContext *context, unsigned char data);
void X68Sound_OpmInt(struct tagX68SoundContext *context, void (*proc)(void *), void *arg);
int X68Sound_OpmWait(struct tagX68SoundContext *context, int wait/*=240*/);
int X68Sound_OpmClock(struct tagX68SoundContext *context, int clock/*=4000000*/);

unsigned char X68Sound_AdpcmPeek(struct tagX68SoundContext *context);
void X68Sound_AdpcmPoke(struct tagX68SoundContext *context, unsigned char data);
unsigned char X68Sound_PpiPeek(struct tagX68SoundContext *context);
void X68Sound_PpiPoke(struct tagX68SoundContext *context, unsigned char data);
void X68Sound_PpiCtrl(struct tagX68SoundContext *context, unsigned char data);
unsigned char X68Sound_DmaPeek(struct tagX68SoundContext *context, unsigned char adrs);
void X68Sound_DmaPoke(struct tagX68SoundContext *context, unsigned char adrs, unsigned char data);
void X68Sound_DmaInt(struct tagX68SoundContext *context, void (*proc)(void *), void *arg);
void X68Sound_DmaErrInt(struct tagX68SoundContext *context, void (*Proc)(void *), void *arg);
void X68Sound_MemReadFunc(struct tagX68SoundContext *context, int (*func)(unsigned char *));

void X68Sound_WaveFunc(struct tagX68SoundContext *context, int (*func)(void *));

int X68Sound_Pcm8_Out(struct tagX68SoundContext *context, int ch, void *adrs, int mode, int len);
int X68Sound_Pcm8_Aot(struct tagX68SoundContext *context, int ch, void *tbl, int mode, int cnt);
int X68Sound_Pcm8_Lot(struct tagX68SoundContext *context, int ch, void *tbl, int mode);
int X68Sound_Pcm8_SetMode(struct tagX68SoundContext *context, int ch, int mode);
int X68Sound_Pcm8_GetRest(struct tagX68SoundContext *context, int ch);
int X68Sound_Pcm8_GetMode(struct tagX68SoundContext *context, int ch);
int X68Sound_Pcm8_Abort(struct tagX68SoundContext *context);

int X68Sound_TotalVolume(struct tagX68SoundContext *context, int v);
int X68Sound_GetTotalVolume(struct tagX68SoundContext *context);

int X68Sound_ErrorCode(struct tagX68SoundContext *context);
int X68Sound_DebugValue(struct tagX68SoundContext *context);

#ifdef __cplusplus
}
#endif

#endif //__X68SOUND_H__
