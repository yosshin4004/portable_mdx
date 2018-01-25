// MXDRV.DLL header
// Copyright (C) 2000 GORRY.

#ifndef __MXDRV_H__
#define __MXDRV_H__

#include <stdint.h>
#include <stdbool.h>
#include "mxdrv_context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagX68REG {
	uint32_t d0;
	uint32_t d1;
	uint32_t d2;
	uint32_t d3;
	uint32_t d4;
	uint32_t d5;
	uint32_t d6;
	uint32_t d7;
	uint32_t a0;
	uint32_t a1;
	uint32_t a2;
	uint32_t a3;
	uint32_t a4;
	uint32_t a5;
	uint32_t a6;
	uint32_t a7;
} X68REG;

typedef struct tagMXWORK_CH {
	uint32_t	S0000;		// Ptr
	uint8_t		S0004_b;	// PCM bank
	uint32_t	S0004;		// voice ptr
	uint32_t	S0008;		// bend delta
	uint32_t	S000c;		// bend offset
	uint16_t	S0010;		// D
	uint16_t	S0012;		// note+D
	uint16_t	S0014;		// note+D+bend+Pitch LFO offset
	uint8_t		S0016;		// flags b3=keyon/off
	uint8_t		S0017;		// flags
	uint8_t		S0018;		// ch
	uint8_t		S0019;		// carrier slot
	uint8_t		S001a;		// len
	uint8_t		S001b;		// gate
	uint8_t		S001c;		// p
	uint8_t		S001d;		// keyon slot
	uint8_t		S001e;		// Q
	uint8_t		S001f;		// Keyon delay
	uint8_t		S0020;		// Keyon delay counter
	uint8_t		S0021;		// PMS/AMS
	uint8_t		S0022;		// v
	uint8_t		S0023;		// v last
	uint8_t		S0024;		// LFO delay
	uint8_t		S0025;		// LFO delay counter
	uint32_t	S0026;		// Pitch LFO Type
	uint32_t	S002a;		// Pitch LFO offset start
	uint32_t	S002e;		// Pitch LFO delta start
	uint32_t	S0032;		// Pitch LFO delta
	uint32_t	S0036;		// Pitch LFO offset
	uint16_t	S003a;		// Pitch LFO length (cooked)
	uint16_t	S003c;		// Pitch LFO length
	uint16_t	S003e;		// Pitch LFO length counter
	uint32_t	S0040;		// Volume LFO Type
	uint16_t	S0044;		// Volume LFO delta start
	uint16_t	S0046;		// Volume LFO delta (cooked)
	uint16_t	S0048;		// Volume LFO delta
	uint16_t	S004a;		// Volume LFO offset
	uint16_t	S004c;		// Volume LFO length
	uint16_t	S004e;		// Volume LFO length counter
} MXWORK_CH;

typedef struct tagMXWORK_GLOBAL {
	uint16_t	L001ba6;
	uint32_t	L001ba8;
	uint32_t	L001bac;
	uint8_t		L001bb4[16];
	uint8_t		L001df4;
	uint8_t		L001df6[16];
	uint16_t	L001e06;	// Channel Mask (true)
	uint8_t		L001e08;
	uint8_t		L001e09;
	uint8_t		L001e0a;
	uint8_t		L001e0b;
	uint8_t		L001e0c;	// @t
	uint8_t		L001e0d;
	uint8_t		L001e0e;
	uint8_t		L001e10;
	uint8_t		L001e12;	// Paused
	uint8_t		L001e13;	// End
	uint8_t		L001e14;	// Fadeout Offset
	uint8_t		L001e15;
	uint8_t		L001e17;	// Fadeout Enable
	uint8_t		L001e18;
	uint8_t		L001e19;
	uint16_t	L001e1a;	// Channel Enable
	uint16_t	L001e1c;	// Channel Mask
	uint16_t	L001e1e[2];	// Fadeout Speed
	uint16_t	L001e22;
	uint32_t	L001e24;
	uint32_t	L001e28;
	uint32_t	L001e2c;
	uint32_t	L001e30;
	uint32_t	L001e34;
	uint32_t	L001e38;
	uint32_t	L00220c;
	uint32_t	L002218;
	uint32_t	L00221c;
	uint32_t	L002220;	// L_MDXSIZE
	uint32_t	L002224;	// L_PDXSIZE
	uint32_t	L002228;	// voice data
	uint32_t	L00222c;
	uint8_t		L002230;
	uint8_t		L002231;
	uint8_t		L002232;
	uint8_t		L002233[9];
	uint8_t		L00223c[12];
	uint8_t		L002245;
	uint16_t	L002246;	// loop count
	uint32_t	FATALERROR;
	uint32_t	FATALERRORADR;
	uint32_t	PLAYTIME;	// 演奏時間
	uint8_t		MUSICTIMER;	// 演奏時間タイマー定数
	uint8_t		STOPMUSICTIMER;		// 演奏時間タイマー停止
	uint32_t	MEASURETIMELIMIT;	// 演奏時間計測中止時間
} MXWORK_GLOBAL;

typedef struct tagMXWORK_KEY {
	uint8_t		OPT1;
	uint8_t		OPT2;
	uint8_t		SHIFT;
	uint8_t		CTRL;
	uint8_t		XF3;
	uint8_t		XF4;
	uint8_t		XF5;
} MXWORK_KEY;

typedef int8_t MXWORK_OPM[256];

typedef void MXCALLBACK_OPMINTFUNC( void );

enum {
	MXDRV_WORK_FM = 0,		// FM8ch+PCM1ch
	MXDRV_WORK_PCM,			// PCM7ch
	MXDRV_WORK_GLOBAL,
	MXDRV_WORK_KEY,
	MXDRV_WORK_OPM,
	MXDRV_WORK_PCM8,
	MXDRV_WORK_CREDIT,
	MXDRV_CALLBACK_OPMINT,
};

enum {
	MXDRV_ERR_MEMORY = 1,
};

int MXDRV_Start(
	MxdrvContext *context,
	int samprate,
	int betw,
	int pcmbuf,
	int late,
	int mdxbuf,
	int pdxbuf,
	int opmmode
);

void MXDRV_End(
	MxdrvContext *context
);

int MXDRV_GetPCM(
	MxdrvContext *context,
	void *buf,
	int len
);

void MXDRV_Play(
	MxdrvContext *context,
	void *mdx,
	uint32_t mdxsize,
	void *pdx,
	uint32_t pdxsize
);

void volatile *MXDRV_GetWork(
	const MxdrvContext *context,
	int i
);

void MXDRV(
	MxdrvContext *context,
	X68REG *reg
);

uint32_t MXDRV_MeasurePlayTime(
	MxdrvContext *context,
	void *mdx,
	uint32_t mdxsize,
	void *pdx,
	uint32_t pdxsize,
	int loop,
	int fadeout
);

void MXDRV_PlayAt(
	MxdrvContext *context,
	uint32_t playat,
	int loop,
	int fadeout
);

int MXDRV_TotalVolume(
	MxdrvContext *context,
	int vol
);


#define MXDRV_Call( context, a )		\
{										\
	X68REG reg;							\
										\
	reg.d0 = (a);						\
	reg.d1 = 0x00;						\
	MXDRV( context, &reg );				\
}										\
										

#define MXDRV_Call_2( context, a, b )	\
{										\
	X68REG reg;							\
										\
	reg.d0 = (a);						\
	reg.d1 = (b);						\
	MXDRV( context, &reg );				\
}										\
										

#define MXDRV_Replay(context) MXDRV_Call(context, 0x0f )
#define MXDRV_Stop(context) MXDRV_Call(context, 0x05 )
#define MXDRV_Pause(context) MXDRV_Call(context, 0x06 )
#define MXDRV_Cont(context) MXDRV_Call(context, 0x07 )
#define MXDRV_Fadeout(context) MXDRV_Call_2(context, 0x0c, 19 )
#define MXDRV_Fadeout2(context, a) MXDRV_Call_2(context, 0x0c, (a) )

#ifdef __cplusplus
}
#endif

#endif //__MXDRV_H__
