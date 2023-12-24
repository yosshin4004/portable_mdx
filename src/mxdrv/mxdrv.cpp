// X68k MXDRV music driver version 2.06+17 Rel.X5-S 
// (c)1988-92 milk.,K.MAEKAWA, Missy.M, Yatsube
//
// Converted for Win32 [MXDRVg] V1.50a
// Copyright (C) 2000 GORRY.
//
// Ported for 64bit environments
// Copyright (C) 2018 Yosshin.

// ;=============================================
// ;  Filename mxdrv17.x
// ;  Time Stamp Sun Mar 15 11:52:06 1998
// ;
// ;  Base address 000000
// ;  Exec address 0017ea
// ;  Text size    001ba6 bytes
// ;  Data size    000000 byte(s)
// ;  Bss  size    0006a2 byte(s)
// ;  438 Labels
// ;  Code Generate date Wed May 06 12:59:13 1998
// ;  Command Line D:\FTOOL\dis.x -C2 --overwrite -7 -m 68040 -M -s8192 -e -g mxdrv17.x mxdrv17.dis 
// ;          DIS version 2.75
// ;=============================================

//         .cpu    68040

//         .include        D:\include/doscall.mac
//         .include        D:\include/iocscall.mac
//         .include        D:\include/fefunc.dis

//         .text

#include "mxdrv_config.h"

#if MXDRV_ENABLE_PORTABLE_CODE
	#include <string.h>		/* for memset */
	#include <mxdrv.h>
	#include <x68sound.h>
	#include <mxdrv_context.h>
	#include "mxdrv_context.internal.h"
	#include "sound_iocs.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <float.h>
#else
	#define DLLEXPORT __declspec(dllexport)

	#undef _WIN32_IE
	#define _WIN32_IE 0x370
	#include <windows.h>
	#include "mxdrv.h"
	#include "mxdrv_depend.h"
	#include "x68sound.h"
	#include "sound_iocs.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <float.h>

	#include <mmsystem.h>
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
	/* MxdrvContextImpl 構造体上に移動 */
#else
extern volatile unsigned char OpmReg1B;  // OPM レジスタ $1B の内容
#endif

#define LOGOPM 0
#define LOGSEQ 0
#define LOGINT 0

/***************************************************************/

#if MXDRV_ENABLE_PORTABLE_CODE
static const char MXWORK_CREDIT[] = "X68k MXDRV music driver version 2.06+17 Rel.X5-S (c)1988-92 milk.,K.MAEKAWA, Missy.M, Yatsube\nConverted for Win32 [MXDRVg] V2.00a Copyright (C) 2000-2002 GORRY.";
#else
static TCHAR MXWORK_CREDIT[] = "X68k MXDRV music driver version 2.06+17 Rel.X5-S (c)1988-92 milk.,K.MAEKAWA, Missy.M, Yatsube\nConverted for Win32 [MXDRVg] V2.00a Copyright (C) 2000-2002 GORRY.";
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
	/* MxdrvContextImpl 構造体上に移動 */
#else
static volatile MXWORK_CH MXWORK_CHBUF_FM[9];
static volatile MXWORK_CH MXWORK_CHBUF_PCM[7];

static volatile MXWORK_GLOBAL MXWORK_GLOBALBUF;
	#define G MXWORK_GLOBALBUF

static volatile MXWORK_KEY MXWORK_KEYBUF;
	#define KEY MXWORK_KEYBUF

static volatile MXWORK_OPM MXWORK_OPMBUF;
	#define OPMBUF MXWORK_OPMBUF

static volatile UBYTE MXWORK_PCM8;
	#define PCM8 MXWORK_PCM8
#endif

/***************************************************************/

#if MXDRV_ENABLE_PORTABLE_CODE
	/* MxdrvContextImpl 構造体上に移動 */
#else
static UBYTE FAKEA6S0004[256];

static ULONG D0;
static ULONG D1;
static ULONG D2;
static ULONG D3;
static ULONG D4;
static ULONG D5;
static ULONG D6;
static ULONG D7;

static UBYTE volatile *A0;
static UBYTE volatile *A1;
static UBYTE volatile *A2;
static UBYTE volatile *A3;
static UBYTE volatile *A4;
static UBYTE volatile *A5;
static MXWORK_CH volatile *A6;
static UBYTE volatile *A7;

static UBYTE DisposeStack_L00122e;

static void (*OPMINT_FUNC)(void);
static void (CALLBACK *MXCALLBACK_OPMINT)(void);

static BOOL volatile MeasurePlayTime;
#endif

#if !MXDRV_ENABLE_PORTABLE_CODE
static CRITICAL_SECTION CS_OPMINT;
#endif

/***************************************************************/

#if MXDRV_ENABLE_PORTABLE_CODE

static void L_0A( MxdrvContext *context );
static void L_0B( MxdrvContext *context );
static void L_0C( MxdrvContext *context );
static void L_0E( MxdrvContext *context );
static void L_10( MxdrvContext *context );
static void L_11( MxdrvContext *context );
static void L_12( MxdrvContext *context );
static void L_13( MxdrvContext *context );
static void L_14( MxdrvContext *context );
static void L_15( MxdrvContext *context );
static void L_16( MxdrvContext *context );
static void L_17( MxdrvContext *context );
static void L0000dc( MxdrvContext *context );
static void L_18( MxdrvContext *context );
static void L_19( MxdrvContext *context );
static void L_1A( MxdrvContext *context );
static void L000216( MxdrvContext *context );
static void L_1B( MxdrvContext *context );
static void L_1C( MxdrvContext *context );
static void L_1D( MxdrvContext *context );
static void L_1E( MxdrvContext *context );
static void L000496( MxdrvContext *context );
static void L_1F( MxdrvContext *context );
static void L_0D( MxdrvContext *context );
static void L000534( MxdrvContext *context );
static void L000552( MxdrvContext *context );
static void L000554( MxdrvContext *context );
static void L00056a( MxdrvContext *context );
static void L_FREE( MxdrvContext *context );
static void L_SETMDX( MxdrvContext *context );
static void L_SETPDX( MxdrvContext *context );
static void L0005f8( MxdrvContext *context );
static void L_STOP( MxdrvContext *context );
static void L00063e( MxdrvContext *context );
static void L_PAUSE( MxdrvContext *context );
static void L0006c4( MxdrvContext *context );
static void L_CONT( MxdrvContext *context );
static void L000756( MxdrvContext *context );
static void L000766( MxdrvContext *context );
static void L00077a( MxdrvContext *context );
static void L000788( MxdrvContext *context );
static void L0007c0( MxdrvContext *context );
static void L_PLAY( MxdrvContext *context );
static void L_0F( MxdrvContext *context );
static void L_ERROR( MxdrvContext *context );
static void L00095a( MxdrvContext *context );
static void L_08( MxdrvContext *context );
static void L_09( MxdrvContext *context );
static void L000998( MxdrvContext *context );
static void L_OPMINT( MxdrvContext *context );
static void L000c66( MxdrvContext *context );
static void L000cdc( MxdrvContext *context );
static void L000d84( MxdrvContext *context );
static void L000dfe( MxdrvContext *context );
static void L000e28( MxdrvContext *context );
static void L000e66( MxdrvContext *context );
static void L000e7e( MxdrvContext *context );
static void L000fe6( MxdrvContext *context );
static void L000ff6( MxdrvContext *context );
static void L001050( MxdrvContext *context );
static void L001094( MxdrvContext *context );
static void L0010b4( MxdrvContext *context );
static void L0010be( MxdrvContext *context );
static void L0010d4( MxdrvContext *context );
static void L001116( MxdrvContext *context );
static void L0010ea( MxdrvContext *context );
static void L001100( MxdrvContext *context );
static void L001120( MxdrvContext *context );
static void L001138( MxdrvContext *context );
static void L00114e( MxdrvContext *context );
static void L001164( MxdrvContext *context );
static void L00117a( MxdrvContext *context );
static void L001192( MxdrvContext *context );
static void L0011b4( MxdrvContext *context );
static void L0011ce( MxdrvContext *context );
static void L0011d4( MxdrvContext *context );
//static void L001240( MxdrvContext *context );		/* 未使用 */
static void L001292( MxdrvContext *context );
static void L0012a6( MxdrvContext *context );
static void L0012be( MxdrvContext *context );
static void L0012e6( MxdrvContext *context );
static void L00131c( MxdrvContext *context );
static void L001328( MxdrvContext *context );
static void L001330( MxdrvContext *context );
static void L001344( MxdrvContext *context );
static void L001364( MxdrvContext *context );
static void L00136a( MxdrvContext *context );
static void L001372( MxdrvContext *context );
static void L001376( MxdrvContext *context );
static void L00139a( MxdrvContext *context );
static void L0013ba( MxdrvContext *context );
static void L0013c6( MxdrvContext *context );
static void L0013dc( MxdrvContext *context );
static void L0013e6( MxdrvContext *context );
static void L001440( MxdrvContext *context );
static void L001442( MxdrvContext *context );
static void L001492( MxdrvContext *context );
static void L001498( MxdrvContext *context );
static void L0014b0( MxdrvContext *context );
static void L0014dc( MxdrvContext *context );
static void L0014fc( MxdrvContext *context );
static void L001590( MxdrvContext *context );
static void L0015d0( MxdrvContext *context );
static void L0015e4( MxdrvContext *context );
static void L0015fe( MxdrvContext *context );
static void L001656( MxdrvContext *context );
static void L00165c( MxdrvContext *context );
static void L001694( MxdrvContext *context );
static void L0016b8( MxdrvContext *context );
static void L0016c6( MxdrvContext *context );
static void L0016fa( MxdrvContext *context );
static void L00170e( MxdrvContext *context );
static void L001216( MxdrvContext *context );
static void L00178a( MxdrvContext *context );
static void L0017a0( MxdrvContext *context );
static void L_WRITEOPM( MxdrvContext *context );
static int Initialize( MxdrvContext *context, int, int );

static void ADPCMOUT( MxdrvContext *context );
static void ADPCMMOD_STOP( MxdrvContext *context );
static void ADPCMMOD_END( MxdrvContext *context );

static void MX_ABORT( MxdrvContext *context );

static void OPMINTFUNC(void *);

static void L_PAUSE_( MxdrvContext *context );
#else
static void L_0A( void );
static void L_0B( void );
static void L_0C( void );
static void L_0E( void );
static void L_10( void );
static void L_11( void );
static void L_12( void );
static void L_13( void );
static void L_14( void );
static void L_15( void );
static void L_16( void );
static void L_17( void );
static void L0000dc( void );
static void L_18( void );
static void L_19( void );
static void L_1A( void );
static void L000216( void );
static void L_1B( void );
static void L_1C( void );
static void L_1D( void );
static void L_1E( void );
static void L000496( void );
static void L_1F( void );
static void L_0D( void );
static void L000534( void );
static void L000552( void );
static void L000554( void );
static void L00056a( void );
static void L_FREE( void );
static void L_SETMDX( void );
static void L_SETPDX( void );
static void L0005f8( void );
static void L_STOP( void );
static void L00063e( void );
static void L_PAUSE( void );
static void L0006c4( void );
static void L_CONT( void );
static void L000756( void );
static void L000766( void );
static void L00077a( void );
static void L000788( void );
static void L0007c0( void );
static void L_PLAY( void );
static void L_0F( void );
static void L_ERROR( void );
static void L00095a( void );
static void L_08( void );
static void L_09( void );
static void L000998( void );
static void L_OPMINT( void );
static void L000c66( void );
static void L000cdc( void );
static void L000d84( void );
static void L000dfe( void );
static void L000e28( void );
static void L000e66( void );
static void L000e7e( void );
static void L000fe6( void );
static void L000ff6( void );
static void L001050( void );
static void L001094( void );
static void L0010b4( void );
static void L0010be( void );
static void L0010d4( void );
static void L001116( void );
static void L0010ea( void );
static void L001100( void );
static void L001120( void );
static void L001138( void );
static void L00114e( void );
static void L001164( void );
static void L00117a( void );
static void L001192( void );
static void L0011b4( void );
static void L0011ce( void );
static void L0011d4( void );
static void L001240( void );
static void L001292( void );
static void L0012a6( void );
static void L0012be( void );
static void L0012e6( void );
static void L00131c( void );
static void L001328( void );
static void L001330( void );
static void L001344( void );
static void L001364( void );
static void L00136a( void );
static void L001372( void );
static void L001376( void );
static void L00139a( void );
static void L0013ba( void );
static void L0013c6( void );
static void L0013dc( void );
static void L0013e6( void );
static void L001440( void );
static void L001442( void );
static void L001492( void );
static void L001498( void );
static void L0014b0( void );
static void L0014dc( void );
static void L0014fc( void );
static void L001590( void );
static void L0015d0( void );
static void L0015e4( void );
static void L0015fe( void );
static void L001656( void );
static void L00165c( void );
static void L001694( void );
static void L0016b8( void );
static void L0016c6( void );
static void L0016fa( void );
static void L00170e( void );
static void L001216( void );
static void L00178a( void );
static void L0017a0( void );
static void L_WRITEOPM( void );
static int Initialize( int, int );

static void ADPCMOUT( void );
static void ADPCMMOD_STOP( void );
static void ADPCMMOD_END( void );

static void MX_ABORT( void );

static void CALLBACK OPMINTFUNC(
	void
);

static void L_PAUSE_(
	void
);
#endif

/***************************************************************/

int MXDRV_Start(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
#endif
	int samprate,
	int betw,
	int pcmbuf,
	int late,
	int mdxbuf,
	int pdxbuf,
	int opmmode
) {
	int	ret;

#if !MXDRV_ENABLE_PORTABLE_CODE
	InitializeCriticalSection( &CS_OPMINT );
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
	memset( (void *)&G, 0, sizeof(G) );
	memset( (void *)&KEY, 0, sizeof(KEY) );
#else
	ZeroMemory( (void *)&G, sizeof(G) );
	ZeroMemory( (void *)&KEY, sizeof(KEY) );
#endif
	G.MEASURETIMELIMIT = (1000*(60*20-2))*(LONGLONG)4000/1024; // 20min-2sec

/* DLL を利用しないので不要 */
#if !MXDRV_ENABLE_PORTABLE_CODE
	ret = X68Sound_Load();
	if ( ret != 0 ) {
		switch ( ret ) {
		  case X68SNDERR_DLL:
		  case X68SNDERR_FUNC:
		  default:
			return (10000+ret);
		}
	}
#endif

	if ( opmmode > 1 ) opmmode = 0;
	if ( opmmode < 0 ) opmmode = 0;

	if ( betw ) {
#if MXDRV_ENABLE_PORTABLE_CODE
		ret = X68Sound_Start( &context->m_impl->m_x68SoundContext, samprate, opmmode+1, 1, betw, pcmbuf, late, 1.0 );
#else
		ret = X68Sound_Start( samprate, opmmode+1, 1, betw, pcmbuf, late, 1.0 );
#endif
	} else {
#if MXDRV_ENABLE_PORTABLE_CODE
		ret = X68Sound_StartPcm( &context->m_impl->m_x68SoundContext, samprate, 1, 1, pcmbuf );
#else
		ret = X68Sound_StartPcm( samprate, 1, 1, pcmbuf );
#endif
	}
	if ( ret != 0 ) {
		switch ( ret ) {
		  case X68SNDERR_PCMOUT:
		  case X68SNDERR_TIMER:
		  case X68SNDERR_MEMORY:
			return (10100+ret);
		}
	}

#if MXDRV_ENABLE_PORTABLE_CODE
	sound_iocs_init(context);
	ret = Initialize( context, mdxbuf, pdxbuf );
#else
	sound_iocs_init();
	ret = Initialize( mdxbuf, pdxbuf );
#endif
	if ( ret != 0 ) {
		return (MXDRV_ERR_MEMORY);
	}

	return (0);
}

void MXDRV_End(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_OpmInt( &context->m_impl->m_x68SoundContext, NULL, NULL);
#else
	X68Sound_OpmInt( NULL );
#endif
	MXCALLBACK_OPMINT = NULL;
	OPMINT_FUNC = NULL;

#if MXDRV_ENABLE_PORTABLE_CODE
	if (context->m_impl->m_pdxReservedMemoryPoolSize != 0) {
		MxdrvContextImpl_ReleaseMemory(context->m_impl, context->m_impl->m_pdxReservedMemoryPoolSize);
		context->m_impl->m_pdxReservedMemoryPoolSize = 0;
	}
	if (context->m_impl->m_mdxReservedMemoryPoolSize != 0) {
		MxdrvContextImpl_ReleaseMemory(context->m_impl, context->m_impl->m_mdxReservedMemoryPoolSize);
		context->m_impl->m_mdxReservedMemoryPoolSize = 0;
	}
	if ( G.L001bac) {
		MxdrvContextImpl_ReleaseMemory(context->m_impl, G.L001ba8);
		G.L001bac = (uint32_t)NULL;
	}
	if ( G.L001e38) {
		MxdrvContextImpl_ReleaseMemory(context->m_impl, G.L002224);
		G.L001e38 = (uint32_t)NULL;
	}
	if ( G.L001e34) {
		MxdrvContextImpl_ReleaseMemory(context->m_impl, G.L002220);
		G.L001e34 = (uint32_t)NULL;
	}
	assert(MxdrvContextImpl_GetReservedMemoryPoolSize(context->m_impl) == 0);
#else
	if ( G.L001e34 ) {
		free( (void*)G.L001e34 );
		G.L001e34 = (uint32_t)NULL;
	}
	if ( G.L001e38 ) {
		free( (void*)G.L001e38 );
		G.L001e38 = (uint32_t)NULL;
	}
	if ( G.L001bac ) {
		free( (void*)G.L001bac );
		G.L001bac = (uint32_t)NULL;
	}
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
	memset( (void *)&G, 0, sizeof(G) );
	memset( (void *)&KEY, 0, sizeof(KEY) );
	memset( FAKEA6S0004, 0, sizeof(FAKEA6S0004) );
#else
	ZeroMemory( (void *)&G, sizeof(G) );
	ZeroMemory( (void *)&KEY, sizeof(KEY) );
	ZeroMemory( FAKEA6S0004, sizeof(FAKEA6S0004) );
#endif
	D0 = D1 = D2 = D3 = D4 = D5 = D6 = D7 = 0;
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = A1 = A2 = A3 = A4 = A5 = A6 = A7 = 0;
#else
	A0 = A1 = A2 = A3 = A4 = A5 = A7 = NULL;
	A6 = NULL;
#endif
#if MXDRV_ENABLE_PORTABLE_CODE
	DisposeStack_L00122e = FALSE;
	memset((void*)MXWORK_CHBUF_FM, 0, sizeof(MXWORK_CHBUF_FM));
	memset((void*)MXWORK_CHBUF_PCM, 0, sizeof(MXWORK_CHBUF_PCM));
	memset((void*)&OPMBUF, 0, sizeof(OPMBUF));
#else
	DisposeStack_L00122e = NULL;
	ZeroMemory((void*)MXWORK_CHBUF_FM, sizeof(MXWORK_CHBUF_FM));
	ZeroMemory((void*)MXWORK_CHBUF_PCM, sizeof(MXWORK_CHBUF_PCM));
	ZeroMemory((void*)&OPMBUF, sizeof(OPMBUF));
#endif
	PCM8 = 0;

#if !MXDRV_ENABLE_PORTABLE_CODE
	DeleteCriticalSection( &CS_OPMINT );
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_Free( &context->m_impl->m_x68SoundContext );
#else
	X68Sound_Free();
#endif
}

int MXDRV_GetPCM(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
#endif
	void *buf,
	int len
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	return ( X68Sound_GetPcm( &context->m_impl->m_x68SoundContext, buf, len ) );
#else
	return ( X68Sound_GetPcm( buf, len ) );
#endif
}

int MXDRV_TotalVolume(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
#endif
	int vol
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	return ( X68Sound_TotalVolume( &context->m_impl->m_x68SoundContext, vol ) );
#else
	return ( X68Sound_TotalVolume( vol ) );
#endif
}

int MXDRV_GetTotalVolume(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	return ( X68Sound_GetTotalVolume( &context->m_impl->m_x68SoundContext) );
#else
	return ( X68Sound_GetTotalVolume() );
#endif
}

void MXDRV_ChannelMask(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
#endif
	int mask
) {
	G.L001e1c = mask;
}

int MXDRV_GetChannelMask(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#endif
) {
	return G.L001e1c;
}

void MXDRV_PCM8Enable(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
#endif
	int sw
) {
	PCM8 = (sw != 0) ? 1 : 0;
}

int MXDRV_GetPCM8Enable(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#endif
) {
	return PCM8;
}

int MXDRV_SetData2(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
	void *mdx,
	uint32_t mdxsize,
	void *pdx,
	uint32_t pdxsize
#else
	void *mdx,
	DWORD mdxsize,
	void *pdx,
	DWORD pdxsize
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	/*
		32bit の相対アドレスで届く範囲のメモリ領域に mdx pdx を配置する必要が
		あるため、MxdrvContextImpl のメモリプールから確保した領域を利用する。
	*/
	if (context->m_impl->m_pdxReservedMemoryPoolSize != 0) {
		MxdrvContextImpl_ReleaseMemory(context->m_impl, context->m_impl->m_pdxReservedMemoryPoolSize);
		context->m_impl->m_pdxReservedMemoryPoolSize = 0;
	}
	if (context->m_impl->m_mdxReservedMemoryPoolSize != 0) {
		MxdrvContextImpl_ReleaseMemory(context->m_impl, context->m_impl->m_mdxReservedMemoryPoolSize);
		context->m_impl->m_mdxReservedMemoryPoolSize = 0;
	}

	void *mdxOnMemoryPool = MxdrvContextImpl_ReserveMemory(context->m_impl, mdxsize);
	if (mdxOnMemoryPool == NULL) return MXDRV_ERR_MEMORY;
	context->m_impl->m_mdxReservedMemoryPoolSize = mdxsize;

	void *pdxOnMemoryPool = MxdrvContextImpl_ReserveMemory(context->m_impl, pdxsize);
	if (pdxOnMemoryPool == NULL) return MXDRV_ERR_MEMORY;
	context->m_impl->m_pdxReservedMemoryPoolSize = pdxsize;

	memcpy(mdxOnMemoryPool, mdx, mdxsize);
	memcpy(pdxOnMemoryPool, pdx, pdxsize);
#endif
#if MXDRV_ENABLE_PORTABLE_CODE
	X68REG reg = {0};
#else
	X68REG reg;
#endif

	reg.d0 = 0x02;
	reg.d1 = mdxsize;
#if MXDRV_ENABLE_PORTABLE_CODE
	reg.a1 = TO_OFS(mdxOnMemoryPool);
	MXDRV( context, &reg );
#else
	reg.a1 = (UBYTE *)mdx;
	MXDRV( &reg );
#endif

	if ( pdx ) {
		reg.d0 = 0x03;
		reg.d1 = pdxsize;
#if MXDRV_ENABLE_PORTABLE_CODE
		reg.a1 = TO_OFS(pdxOnMemoryPool);
		MXDRV( context, &reg );
#else
		reg.a1 = (UBYTE *)pdx;
		MXDRV( &reg );
#endif
	} else {
		G.L002231 = CLR;
	}

	return 0;
}

void MXDRV_Play(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
	void *mdx,
	uint32_t mdxsize,
	void *pdx,
	uint32_t pdxsize
#else
	void *mdx,
	DWORD mdxsize,
	void *pdx,
	DWORD pdxsize
#endif
) {
	int ret = MXDRV_SetData2(
#if MXDRV_ENABLE_PORTABLE_CODE
		context,
#endif
		mdx, mdxsize, pdx, pdxsize
	);
	if (ret != 0) return;

	MXDRV_Play2(
#if MXDRV_ENABLE_PORTABLE_CODE
		context
#endif
	);
}

void MXDRV_Play2(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	X68REG reg = {0};
#else
	X68REG reg;
#endif

	reg.d0 = 0x0f;
	reg.d1 = 0x00;
#if MXDRV_ENABLE_PORTABLE_CODE
	MXDRV( context, &reg );
#else
	MXDRV( &reg );
#endif
}

void volatile *MXDRV_GetWork(
#if MXDRV_ENABLE_PORTABLE_CODE
	const MxdrvContext *context,
#endif
	int i
) {
	switch (i) {
	  case MXDRV_WORK_FM:
		return (void *)&MXWORK_CHBUF_FM[0];
	  case MXDRV_WORK_PCM:
		return (void *)&MXWORK_CHBUF_PCM[0];
	  case MXDRV_WORK_GLOBAL:
		return (void *)&MXWORK_GLOBALBUF;
	  case MXDRV_WORK_KEY:
		return (void *)&MXWORK_KEYBUF;
	  case MXDRV_WORK_OPM:
		return (void *)&MXWORK_OPMBUF;
	  case MXDRV_WORK_PCM8:
		return (void *)&MXWORK_PCM8;
	  case MXDRV_WORK_CREDIT:
		return (void *)&MXWORK_CREDIT;
	  case MXDRV_CALLBACK_OPMINT:
		return (void *)&MXCALLBACK_OPMINT;
	}
	return (NULL);
}

/***************************************************************/

#if MXDRV_ENABLE_PORTABLE_CODE
	/* MxdrvContextImpl 構造体上に移動 */
#else
static BOOL volatile TerminatePlay;
static int LoopCount;
static int LoopLimit;
static BOOL FadeoutStart;
static BOOL ReqFadeout;
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
static void MXDRV_MeasurePlayTime_OPMINT(
	MxdrvContext *context
#else
static void CALLBACK MXDRV_MeasurePlayTime_OPMINT(
	void
#endif
) {
	if ( G.PLAYTIME >= G.MEASURETIMELIMIT ) {
		TerminatePlay = TRUE;
	}
	if ( G.L001e13 != 0 ) {
		TerminatePlay = TRUE;
	}
	if ( G.L002246 == 65535 ) {
		TerminatePlay = TRUE;
	} else {
		LoopCount = G.L002246;
		if ( !FadeoutStart ) {
			if ( LoopCount >= LoopLimit ) {
				if ( ReqFadeout ) {
					FadeoutStart = TRUE;
#if MXDRV_ENABLE_PORTABLE_CODE
					MXDRV_Fadeout(context);
#else
					MXDRV_Fadeout();
#endif
				} else {
					TerminatePlay = TRUE;
				}
			}
		}
	}
}

uint32_t MXDRV_MeasurePlayTime2(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
#endif
	int loop,
	int fadeout
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	X68REG reg = {0};
	void (*opmintback)( MxdrvContext *context );
#else
	X68REG reg;
	void (CALLBACK *opmintback)(void);
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_OpmInt( &context->m_impl->m_x68SoundContext, NULL, NULL );
#else
	X68Sound_OpmInt( NULL );
#endif

	MeasurePlayTime = TRUE;
	TerminatePlay = FALSE;
	LoopCount = 0;
	LoopLimit = loop;
	FadeoutStart = FALSE;
	ReqFadeout = fadeout;

	opmintback = MXCALLBACK_OPMINT;
	MXCALLBACK_OPMINT = MXDRV_MeasurePlayTime_OPMINT;

	reg.d0 = 0x0f;
	reg.d1 = -1;
#if MXDRV_ENABLE_PORTABLE_CODE
	MXDRV( context, &reg );
#else
	MXDRV( &reg );
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
	while ( !TerminatePlay ) OPMINTFUNC( context );

	MXDRV_Stop( context );
#else
	while ( !TerminatePlay ) OPMINTFUNC();

	MXDRV_Stop();
#endif

	MXCALLBACK_OPMINT = opmintback;
	MeasurePlayTime = FALSE;
#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_OpmInt( &context->m_impl->m_x68SoundContext, &OPMINTFUNC, (void *)context );
#else
	X68Sound_OpmInt( &OPMINTFUNC );
#endif

	uint32_t ret = (DWORD)(G.PLAYTIME*(LONGLONG)1024/4000+(1-DBL_EPSILON))+2000;
	G.PLAYTIME = 0;
	return ret;
}

#if MXDRV_ENABLE_PORTABLE_CODE
uint32_t MXDRV_MeasurePlayTime(
	MxdrvContext *context,
	void *mdx,
	uint32_t mdxsize,
	void *pdx,
	uint32_t pdxsize,
	int loop,
	int fadeout
#else
DWORD MXDRV_MeasurePlayTime(
	void *mdx,
	DWORD mdxsize,
	void *pdx,
	DWORD pdxsize,
	int loop,
	int fadeout
#endif
) {
	int ret = MXDRV_SetData2(
#if MXDRV_ENABLE_PORTABLE_CODE
		context,
#endif
		mdx, mdxsize, pdx, pdxsize
	);
	if (ret != 0) return 0;

	return MXDRV_MeasurePlayTime2(
#if MXDRV_ENABLE_PORTABLE_CODE
		context,
#endif
		loop, fadeout
	);
}

/***************************************************************/

void MXDRV_PlayAt(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
	uint32_t playat,
#else
	DWORD playat,
#endif
	int loop,
	int fadeout

) {
#if MXDRV_ENABLE_PORTABLE_CODE
	X68REG reg = {0};
	void (*opmintback)(MxdrvContext *context);
#else
	X68REG reg;
	void (CALLBACK *opmintback)(void);
#endif
	UWORD chmaskback;
	int opmwaitback;

#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_OpmInt( &context->m_impl->m_x68SoundContext, NULL, NULL );
#else
	X68Sound_OpmInt( NULL );
#endif

	TerminatePlay = FALSE;
	LoopCount = 0;
	LoopLimit = loop;
	FadeoutStart = FALSE;
	ReqFadeout = fadeout;

#if MXDRV_ENABLE_PORTABLE_CODE
	L_PLAY(context);
#else
	L_PLAY();
#endif

	playat = (DWORD)(playat*(LONGLONG)4000/1024);

	opmintback = MXCALLBACK_OPMINT;
	MXCALLBACK_OPMINT = MXDRV_MeasurePlayTime_OPMINT;
	chmaskback = G.L001e1c;

	reg.d0 = 0x0f;
	reg.d1 = -1;
#if MXDRV_ENABLE_PORTABLE_CODE
	MXDRV( context , &reg );
#else
	MXDRV( &reg );
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
	opmwaitback = X68Sound_OpmWait(&context->m_impl->m_x68SoundContext, -1);
	X68Sound_OpmWait(&context->m_impl->m_x68SoundContext, 1);
#else
	opmwaitback = X68Sound_OpmWait(-1);
	X68Sound_OpmWait(1);
#endif
	while ( G.PLAYTIME < playat ) {
		if ( TerminatePlay ) break;
#if MXDRV_ENABLE_PORTABLE_CODE
		OPMINTFUNC( context );
#else
		OPMINTFUNC();
#endif
	}
#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_OpmWait( &context->m_impl->m_x68SoundContext, opmwaitback );
#else
	X68Sound_OpmWait(opmwaitback);
#endif

	G.L001e1c = chmaskback;
	MXCALLBACK_OPMINT = opmintback;
#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_OpmInt( &context->m_impl->m_x68SoundContext, &OPMINTFUNC, (void *)context );
#else
	X68Sound_OpmInt( &OPMINTFUNC );
#endif
}

/***************************************************************/

uint32_t MXDRV_GetPlayAt(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	return (ULONG)(G.PLAYTIME*(LONGLONG)1024/4000);
}

/***************************************************************/

int MXDRV_GetTerminated(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	
	if ( G.PLAYTIME >= G.MEASURETIMELIMIT ) {
		return (1);
	}
	if ( G.L001e13 != 0 ) {
		return (1);
	}
	if ( G.L002246 == 65535 ) {
		return (1);
	}
	return 0;
}

/***************************************************************/

static void PCM8_SUB(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	if ( MeasurePlayTime ) return;

	switch ( D0&0xfff0 ) {
	  case 0x0000:
#if MXDRV_ENABLE_PORTABLE_CODE
		X68Sound_Pcm8_Out( &context->m_impl->m_x68SoundContext, D0&0xff, TO_PTR(A1), D1, D2 );
		context->m_impl->m_logicalSumOfKeyOnFlagsForPcm[D0&7] = true;
#else
		X68Sound_Pcm8_Out( D0&0xff, (void *)A1, D1, D2 );
#endif
		break;
	  case 0x0100:
		switch ( D0&0xffff ) {
		  case 0x0100:
#if MXDRV_ENABLE_PORTABLE_CODE
			X68Sound_Pcm8_Out( &context->m_impl->m_x68SoundContext, D0&0xff, 0, 0, 0 );
			context->m_impl->m_logicalSumOfKeyOnFlagsForPcm[D0&7] = true;
#else
			X68Sound_Pcm8_Out( D0&0xff, 0, 0, 0 );
#endif
			break;
		  case 0x0101:
#if MXDRV_ENABLE_PORTABLE_CODE
			X68Sound_Pcm8_Abort( &context->m_impl->m_x68SoundContext );
#else
			X68Sound_Pcm8_Abort();
#endif
			break;
		}
		break;
	  case 0x01F0:
		switch ( D0&0xffff ) {
		  case 0x01FC:
			D0 = 1;
			break;
		}
		break;
	}
}

/***************************************************************/

static void OPM_SUB(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if LOGOPM
	FILE *fout;
	fout = fopen( "c:\\temp\\opm.log", "ab+" );
	fprintf( fout, "%02X %02X\n", D1&0xff, D2&0xff );
	fclose( fout );
#endif

	if ( MeasurePlayTime ) return;

#if MXDRV_ENABLE_PORTABLE_CODE
	_iocs_opmset( context, (BYTE)D1, (BYTE)D2 );
#else
	_iocs_opmset( (BYTE)D1, (BYTE)D2 );
#endif
}

/***************************************************************/

static void ADPCMOUT(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	void *addr = TO_PTR(A1);
	/*
		ZEL2_.MDX にて、A1 が不正なポインタの状態でここが実行される。
		対症療法で回避。
	*/
	if (&context->m_impl->m_memoryPool[0] <= addr
	&&	addr < &context->m_impl->m_memoryPool[context->m_impl->m_memoryPoolSizeInBytes]) {
		_iocs_adpcmout( context, TO_PTR(A1), D1, D2 );
	} else {
//		printf("invalid A1 %08X\n", A1);
	}
	context->m_impl->m_logicalSumOfKeyOnFlagsForPcm[0] = true;
#else
	_iocs_adpcmout( (void *)A1, D1, D2 );
#endif
}

static void ADPCMMOD_STOP(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	_iocs_adpcmmod( context, 1 );
#else
	_iocs_adpcmmod( 1 );
#endif
}

static void ADPCMMOD_END(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	_iocs_adpcmmod( context, 0 );
#else
	_iocs_adpcmmod( 0 );
#endif
}

/***************************************************************/

#if MXDRV_ENABLE_PORTABLE_CODE
static void OPMINTFUNC(
	void	*arg
#else
static void CALLBACK OPMINTFUNC(
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context = (MxdrvContext *)arg;
#endif

#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext_EnterCriticalSection( context );
#else
	EnterCriticalSection( &CS_OPMINT );
#endif
#if MXDRV_ENABLE_PORTABLE_CODE
	OPMINT_FUNC( context );
#else
	OPMINT_FUNC();
#endif
	if ( !G.STOPMUSICTIMER ) {
		G.PLAYTIME += 256-G.MUSICTIMER; // OPMBUF[0x12];
	}
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( MXCALLBACK_OPMINT ) MXCALLBACK_OPMINT( context );
#else
	if ( MXCALLBACK_OPMINT ) MXCALLBACK_OPMINT();
#endif
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext_LeaveCriticalSection( context );
#else
	LeaveCriticalSection( &CS_OPMINT );
#endif
}

static void SETOPMINT(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
	void (*func)( MxdrvContext * )
#else
	void (*func)( void )
#endif
) {
	OPMINT_FUNC = func;
#if MXDRV_ENABLE_PORTABLE_CODE
	X68Sound_OpmInt( &context->m_impl->m_x68SoundContext, &OPMINTFUNC, (void *)context );
#else
	X68Sound_OpmInt( &OPMINTFUNC );
#endif
}

/***************************************************************/


static void MX_ABORT(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	(void)context;
#endif
}

/***************************************************************/
/*
L000000:;
		.dc.b   'EX17'
L000004:;
		.dc.b   'mxdrv206'
*/

/***************************************************************/
/*
L00000c:;
		movem.l d1-d7/a0-a6,-(sp)
		lea.l   L00220c(pc),a5
		cmp.b   #$20,d0                 ;' '
		bcc     L000024
		add.w   d0,d0
		move.w  L00002a(pc,d0.w),d0
		jsr     L00002a(pc,d0.w)
L000024:;
		movem.l (sp)+,d1-d7/a0-a6
		rte

L00002a:;
		.dc.w   L_FREE-L00002a
		.dc.w   L_ERROR-L00002a
		.dc.w   L_SETMDX-L00002a
		.dc.w   L_SETPDX-L00002a
		.dc.w   L_PLAY-L00002a
		.dc.w   L_STOP-L00002a
		.dc.w   L_PAUSE-L00002a
		.dc.w   L_CONT-L00002a
		.dc.w   L_08-L00002a
		.dc.w   L_09-L00002a
		.dc.w   L_0A-L00002a
		.dc.w   L_0B-L00002a
		.dc.w   L_0C-L00002a
		.dc.w   L_0D-L00002a
		.dc.w   L_0E-L00002a
		.dc.w   L_0F-L00002a
		.dc.w   L_10-L00002a
		.dc.w   L_11-L00002a
		.dc.w   L_12-L00002a
		.dc.w   L_13-L00002a
		.dc.w   L_14-L00002a
		.dc.w   L_15-L00002a
		.dc.w   L_16-L00002a
		.dc.w   L_17-L00002a
		.dc.w   L_18-L00002a
		.dc.w   L_19-L00002a
		.dc.w   L_1A-L00002a
		.dc.w   L_1B-L00002a
		.dc.w   L_1C-L00002a
		.dc.w   L_1D-L00002a
		.dc.w   L_1E-L00002a
		.dc.w   L_1F-L00002a
*/
void MXDRV(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
#endif
	X68REG *reg
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	static void (*jumptable[])( MxdrvContext *context ) = {
#else
	static void (*jumptable[])(void) = {
#endif
		L_FREE,
		L_ERROR,
		L_SETMDX,
		L_SETPDX,
		L_PLAY,
		L_STOP,
		L_PAUSE,
		L_CONT,
		L_08,
		L_09,
		L_0A,
		L_0B,
		L_0C,
		L_0D,
		L_0E,
		L_0F,
		L_10,
		L_11,
		L_12,
		L_13,
		L_14,
		L_15,
		L_16,
		L_17,
		L_18,
		L_19,
		L_1A,
		L_1B,
		L_1C,
		L_1D,
		L_1E,
		L_1F,
	};

	D0 = reg->d0;
	D1 = reg->d1;
	D2 = reg->d2;
	D3 = reg->d3;
	D4 = reg->d4;
	D5 = reg->d5;
	D6 = reg->d6;
	D7 = reg->d7;
	A0 = reg->a0;
	A1 = reg->a1;
	A2 = reg->a2;
	A3 = reg->a3;
	A4 = reg->a4;
	A5 = reg->a5;
#if MXDRV_ENABLE_PORTABLE_CODE
	A6 = reg->a6;
#else
	A6 = (MXWORK_CH *)reg->a6;
#endif
	A7 = reg->a7;

	if ( D0 >= 0x20 ) return;
#if MXDRV_ENABLE_PORTABLE_CODE
	jumptable[D0]( context );
#else
	jumptable[D0]();
#endif

	reg->d0 = D0;
	reg->d1 = D1;
	reg->d2 = D2;
	reg->d3 = D3;
	reg->d4 = D4;
	reg->d5 = D5;
	reg->d6 = D6;
	reg->d7 = D7;
#if MXDRV_ENABLE_PORTABLE_CODE
	reg->a0 = A0;
	reg->a1 = A1;
	reg->a2 = A2;
	reg->a3 = A3;
	reg->a4 = A4;
	reg->a5 = A5;
	reg->a6 = A6;
	reg->a7 = A7;
#else
	reg->a0 = (UBYTE *)A0;
	reg->a1 = (UBYTE *)A1;
	reg->a2 = (UBYTE *)A2;
	reg->a3 = (UBYTE *)A3;
	reg->a4 = (UBYTE *)A4;
	reg->a5 = (UBYTE *)A5;
	reg->a6 = (UBYTE *)A6;
	reg->a7 = (UBYTE *)A7;
#endif
}


/***************************************************************/
/*
L_0A:;
		move.b  d1,(L001e14)
		rts
*/
static void L_0A(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e14 = D1&0xff;
#else
	G.L001e14 = D1;
#endif
}


/***************************************************************/
/*
L_0B:;
		move.b  d1,(L001e15)
		rts
*/
static void L_0B(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e15 = D1&0xff;
#else
	G.L001e15 = D1;
#endif
}


/***************************************************************/
/*
L_0C:;
		move.w  d1,(L001e1e)
		st.b    (L001e17)
		rts
*/
static void L_0C(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e1e[0] = D1&0xffff;
#else
	G.L001e1e[0] = D1;
#endif
	G.L001e17 = SET;
}


/***************************************************************/
/*
L_0E:;
		move.w  d1,(L001e1c)
		rts
*/
static void L_0E(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e1c = D1&0xffff;
#else
	G.L001e1c = D1;
#endif
}


/***************************************************************/
/*
L_10:;
		lea.l   OPMBUF(pc),a0
		move.l  a0,d0
		rts
*/
static void L_10(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	D0 = TO_OFS(&OPMBUF);
#else
	D0 = (ULONG)(&OPMBUF);
#endif
}


/***************************************************************/
/*
L_11:;
		lea.l   L001e0e(pc),a0
		tst.l   d1
		bmi     L00009a
		move.b  d1,(a0)
		rts

L00009a:;
		move.b  (a0),d0
		rts
*/
static void L_11(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	if ( (SLONG)D1 < 0 ) {
		D0 = G.L001e0e;
	} else {
		G.L001e0e = (UBYTE)D1;
	}
}


/***************************************************************/
/*
L_12:;
		move.b  (L001e12),-(sp)
		move.w  (sp)+,d0
		move.b  (L001e13),d0
		rts
*/
static void L_12(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	D0 = G.L001e12*256 + G.L001e13;
}


/***************************************************************/
/*
L_13:;
		move.b  (L001e0a),d0
		move.b  d1,(L001e0a)
		rts
*/
static void L_13(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	D0 = G.L001e0a;
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e0a = (uint8_t)D1;
#else
	G.L001e0a = D1;
#endif
}


/***************************************************************/
/*
L_14:;
		move.w  (L001e06),d0
		not.w   d0
		rts
*/
static void L_14(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	D0 = ~G.L001e06;
}


/***************************************************************/
/*
L_15:;
		move.b  (L001e0b),d0
		move.b  d1,(L001e0b)
		rts
*/
static void L_15(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	D0 = G.L001e0b;
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e0b = D1&0xff;
#else
	G.L001e0b = D1;
#endif
}


/***************************************************************/
/*
L_16:;
		move.b  (L001e08),d0
		move.b  d1,(L001e08)
		bsr     L_STOP
		rts
*/
static void L_16(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	D0 = G.L001e08;
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e08 = D1&0xff;
	L_STOP( context );
#else
	G.L001e08 = D1;
	L_STOP();
#endif
}


/***************************************************************/

static void L_17(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

/*
														move.b  (L001e08),d0
														beq     L0001ee
*/
	D0 = G.L001e08;
	if ( D0 == 0 ) {

//L0001ee:;
/*
														move.b  (L001e12),-(sp)
														move.w  (sp)+,d0
														move.b  (L001e13),d0
														rts
*/
		D0 = G.L001e12*256 + G.L001e13;
		return;
	}

/*
														; fall down;
*/

#if MXDRV_ENABLE_PORTABLE_CODE
	L0000dc( context );
#else
	L0000dc();
#endif
}

static void L0000dc(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	UWORD volatile * a0_w;
	ULONG d1,d2,d3,d4,d5,d6,d7;
#if MXDRV_ENABLE_PORTABLE_CODE
	uint32_t a0,a1,a2,a3,a4,a5,a6;
#else
	UBYTE volatile *a0,*a1,*a2,*a3,*a4,*a5,*a6;
#endif

//L0000dc:;
/*
														movem.l d1-d7/a0-a6,-(sp)
														st.b    (L002245)
														lea.l   L001e1e(pc),a0
														lea.l   L001e17(pc),a1
														tst.b   (a1)
														beq     L000174
														bpl     L0000fc
														move.b  #$7f,(a1)
														move.w  (a0),$0002(a0)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	d1=D1, d2=D2, d3=D3, d4=D4, d5=D5, d6=D6, d7=D7, a0=A0, a1=A1, a2=A2, a3=A3, a4=A4, a5=A5, a6=A6;
#else
	d1=D1, d2=D2, d3=D3, d4=D4, d5=D5, d6=D6, d7=D7, a0=A0, a1=A1, a2=A2, a3=A3, a4=A4, a5=A5, a6=(UBYTE *)A6;
#endif
	G.L002245 = SET;
	a0_w = &G.L001e1e[0];
#if MXDRV_ENABLE_PORTABLE_CODE
	A1 = TO_OFS(&G.L001e17);
	if ( *TO_PTR(A1) == 0 ) goto L000174;
	if ( (SBYTE)*TO_PTR(A1) >= 0 ) goto L0000fc;
	*TO_PTR(A1) = 0x7f;
#else
	A1 = &G.L001e17;
	if ( *(A1) == 0 ) goto L000174;
	if ( (SBYTE)*(A1) >= 0 ) goto L0000fc;
	*(A1) = 0x7f;
#endif
	a0_w[1] = a0_w[0];

L0000fc:;
/*
														tst.w   $0002(a0)
														bmi     L000108
														subq.w  #2,$0002(a0)
														bra     L000174
*/
	if ( (SWORD)a0_w[1] < 0 ) goto L000108;
	a0_w[1] -= 2;
	goto L000174;

L000108:;
/*
														lea.l   L001e14(pc),a1
														cmpi.b  #$0a,(a1)
														bge     L000120
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A1 = TO_OFS(&G.L001e14);
	if ( (SBYTE)*TO_PTR(A1) >= 0x0a ) goto L000120;
#else
	A1 = &G.L001e14;
	if ( (SBYTE)*(A1) >= 0x0a ) goto L000120;
#endif

L000112:;
/*
														cmpi.b  #$3e,(a1)               ;'>'
														bge     L000126
														addq.b  #1,(a1)
														move.w  (a0),$0002(a0)
														bra     L000174
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( (SBYTE)*TO_PTR(A1) >= 0x3e ) goto L000126;
	*TO_PTR(A1) += 1;
#else
	if ( (SBYTE)*(A1) >= 0x3e ) goto L000126;
	*(A1) += 1;
#endif
	a0_w[1] = a0_w[0];
	goto L000174;

L000120:;
/*
														st.b    (L001e15)
														bra     L000112
*/
	G.L001e15 = SET;
	goto L000112;

L000126:;
/*
														tst.b   (L001e18)
														beq     L000134
														bsr     L00077a
														bra     L0001d6
*/
	if ( G.L001e18 == 0 ) goto L000134;
#if MXDRV_ENABLE_PORTABLE_CODE
	L00077a( context );
#else
	L00077a();
#endif
	goto L0001d6;

L000134:;
/*
														move.b  #$7f,(a1)
														clr.b   (L001e17)
														move.b  #$01,(L001e13)
														bsr     L_PAUSE
														movea.l $0088.w,a0
														move.l  -$0008(a0),d0
														cmp.l   #$50434d34,d0           ;'PCM4'
														beq     L00015e
														cmp.l   #$50434d38,d0           ;'PCM8'
														bne     L000164
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	*TO_PTR(A1) = 0x7f;
#else
	*(A1) = 0x7f;
#endif
	G.L001e17 = CLR;
	G.L001e13 = 0x01;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_PAUSE_( context );  /* L_PAUSE()のタイマーを止めない */
#else
	L_PAUSE_();  /* L_PAUSE()のタイマーを止めない */
#endif
	if ( !PCM8 ) goto L000164;

//L00015e:;
/*
														move.w  #$0100,d0
														trap    #2
*/
	D0 = 0x0100;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif

L000164:;
/*
														tst.b   (L001df4)
														beq     L000174
														move.w  #$01ff,d0
														trap    #2
														clr.b   (L001df4)
*/
	if ( G.L001df4 == 0 ) goto L000174;
	D0 = 0x01ff;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif
	G.L001df4 = CLR;

L000174:;
/*
														lea.l   L001e0c(pc),a0
														move.b  (a0),d2
														moveq.l #$12,d1
														tst.b   (L001e13)
														bne     L0001d6
														addq.w  #1,(L001ba6)
														lea.l   CHBUF_FM(pc),a6
														moveq.l #$00,d7
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = TO_OFS(&G.L001e0c);
	D2 = *TO_PTR(A0);
#else
	A0 = &G.L001e0c;
	D2 = *(A0);
#endif
	D1 = 0x12;
	if ( G.L001e13 != 0 ) goto L0001d6;
	G.L001ba6++;
#if MXDRV_ENABLE_PORTABLE_CODE
	A6 = TO_OFS(&MXWORK_CHBUF_FM[0]);
#else
	A6 = &MXWORK_CHBUF_FM[0];
#endif
	D7 = 0x00;

L00018c:;
/*
														bsr     L001050
														bsr     L0011b4
														move.w  L001e1c(pc),d0
														btst.l  d7,d0
														bne     L0001a0
														bsr     L000c66
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L001050( context );
	L0011b4( context );
#else
	L001050();
	L0011b4();
#endif
	D0 = G.L001e1c;
	if ( D0 & (1<<D7) ) goto L0001a0;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000c66( context );
#else
	L000c66();
#endif

L0001a0:;
/*
														lea.l   $0050(a6),a6
														addq.w  #1,d7
														cmp.w   #$0009,d7
														bcs     L00018c
														tst.b   (L001df4)
														beq     L0001d6
														lea.l   CHBUF_PCM(pc),a6
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A6+=sizeof(MXWORK_CH);
#else
	A6++;
#endif
	D7++;
	if ( D7 < 0x0009 ) goto L00018c;
	if ( G.L001df4 == 0 ) goto L0001d6;
#if MXDRV_ENABLE_PORTABLE_CODE
	A6 = TO_OFS(&MXWORK_CHBUF_PCM[0]);
#else
	A6 = &MXWORK_CHBUF_PCM[0];
#endif

L0001b6:;
/*
														bsr     L001050
														bsr     L0011b4
														move.w  L001e1c(pc),d0
														btst.l  d7,d0
														bne     L0001ca
														bsr     L000c66
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L001050( context );
	L0011b4( context );
#else
	L001050();
	L0011b4();
#endif
	D0 = G.L001e1c;
	if ( D0 & (1<<D7) ) goto L0001ca;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000c66( context );
#else
	L000c66();
#endif

L0001ca:;
/*
														lea.l   $0050(a6),a6
														addq.w  #1,d7
														cmp.w   #$0010,d7
														bcs     L0001b6
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A6+=sizeof(MXWORK_CH);
#else
	A6++;
#endif
	D7++;
	if ( D7 < 0x0010 ) goto L0001b6;

L0001d6:;
/*
														tst.b   $00e90003
														bmi     L0001d6
														move.b  #$1b,$00e90001
														clr.b   (L002245)
														movem.l (sp)+,d1-d7/a0-a6
*/
	G.L002245 = CLR;
#if MXDRV_ENABLE_PORTABLE_CODE
	D1=d1, D2=d2, D3=d3, D4=d4, D5=d5, D6=d6, D7=d7, A0=a0, A1=a1, A2=a2, A3=a3, A4=a4, A5=a5, A6=a6;
#else
	D1=d1, D2=d2, D3=d3, D4=d4, D5=d5, D6=d6, D7=d7, A0=a0, A1=a1, A2=a2, A3=a3, A4=a4, A5=a5, A6=(MXWORK_CH *)a6;
#endif

//L0001ee:;
/*
														move.b  (L001e12),-(sp)
														move.w  (sp)+,d0
														move.b  (L001e13),d0
														rts
*/
	D0 = G.L001e12*256 + G.L001e13;

}


/***************************************************************/
/*
L_18:;
		lea.l   CHBUF_PCM(pc),a0
		move.l  a0,d0
		rts
*/
static void L_18(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = TO_OFS(&MXWORK_CHBUF_PCM[0]);
	D0 = A0;
#else
	A0 = (UBYTE *)&MXWORK_CHBUF_PCM[0];
	D0 = (ULONG)A0;
#endif
}


/***************************************************************/
/*
L_19:;
		lea.l   L001bb4(pc),a0
		move.l  a0,d0
		rts
*/
static void L_19(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = TO_OFS(&G.L001bb4[0]);
	D0 = A0;
#else
	A0 = &G.L001bb4[0];
	D0 = (ULONG)A0;
#endif
}


/***************************************************************/

static void L_1A(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

/*
														bsr     L000216
														tst.l   d0
														bmi     L000214
														move.l  d1,$0004(sp)
L000214:;
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L000216( context );
#else
	L000216();
#endif
}


/***************************************************************/

static void L000216(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	ULONG volatile *a1_l;
	ULONG volatile *a2_l;
	ULONG t0;
	ULONG d2,d3,d4;
#if MXDRV_ENABLE_PORTABLE_CODE
	uint32_t a0,a1,a2;
#else
	UBYTE volatile *a0,*a1,*a2;
#endif

//L000216:;
/*
														movem.l d2-d4/a0-a2,-(sp)
														movea.l a0,a1
														moveq.l #$00,d0
														moveq.l #$ff,d3
														moveq.l #$00,d1
*/
	d2=D2, d3=D3, d4=D4, a0=A0, a1=A1, a2=A2;
#if MXDRV_ENABLE_PORTABLE_CODE
	a1_l = (ULONG *)TO_PTR(A0);
#else
	a1_l = (ULONG *)A0;
#endif
	D0 = 0x00000000;
	D3 = 0xffffffff;
	D1 = 0x00000000;

L000222:;
/*
														move.l  (a1)+,d4
														move.l  (a1)+,d2
														and.l   #$00ffffff,d4
														beq     L00025a
														cmp.l   -$0008(a1),d4
														bne     L000260
														and.l   #$00ffffff,d2
														beq     L00025a
														cmp.l   -$0004(a1),d2
														bne     L000260
														add.l   d4,d2
														cmp.l   d1,d2
														bcs     L00024a
														move.l  d2,d1
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D4 = GETBLONG(TO_OFS(a1_l)); a1_l++;
	D2 = GETBLONG(TO_OFS(a1_l)); a1_l++;
	D4 &= 0x00ffffff;
	if ( D4 == 0 ) goto L00025a;
	t0 = GETBLONG(TO_OFS(a1_l-2));
	if ( t0 != D4 ) goto L000260;
	D2 &= 0x00ffffff;
	if ( D2 == 0 ) goto L00025a;
	t0 = GETBLONG(TO_OFS(a1_l-1));
#else
	D4 = GETBLONG(a1_l); a1_l++;
	D2 = GETBLONG(a1_l); a1_l++;
	D4 &= 0x00ffffff;
	if ( D4 == 0 ) goto L00025a;
	t0 = GETBLONG(a1_l-2);
	if ( t0 != D4 ) goto L000260;
	D2 &= 0x00ffffff;
	if ( D2 == 0 ) goto L00025a;
	t0 = GETBLONG(a1_l-1);
#endif
	if ( t0 != D2 ) goto L000260;
	D2 += D4;
	if ( D1 > D2 ) goto L00024a;
	D1 = D2;

L00024a:;
/*
														cmp.l   d4,d3
														bcs     L000250
														move.l  d4,d3
*/
	if ( D4 > D3 ) goto L000250;
	D3 = D4;

L000250:;
/*
														lea.l   $00(a0,d3.l),a2
														cmpa.l  a2,a1
														beq     L00025e
														bhi     L000266
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	a2_l = (ULONG *)TO_PTR(A0+D3);
#else
	a2_l = (ULONG *)(A0+D3);
#endif
	if ( a2_l == a1_l ) goto L00025e;
	if ( a2_l < a1_l ) goto L000266;

L00025a:;
/*
														addq.w  #1,d0
														bra     L000222
*/
	D0++;
	goto L000222;

L00025e:;
/*
														addq.w  #1,d0
*/
	D0++;

L000260:;
/*
														movem.l (sp)+,d2-d4/a0-a2
														rts
*/
	D2=d2, D3=d3, D4=d4, A0=a0, A1=a1, A2=a2;
	return;

L000266:;
/*
														moveq.l #$ff,d0
														movem.l (sp)+,d2-d4/a0-a2
														rts
*/
	D0 = 0xffffffff;
	D2=d2, D3=d3, D4=d4, A0=a0, A1=a1, A2=a2;
	return;

}


/***************************************************************/

static void L_1B(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	ULONG d1,d2,d3,d4,d5;
#if MXDRV_ENABLE_PORTABLE_CODE
	uint32_t a0,a1,a2;
#else
	UBYTE volatile *a0,*a1,*a2;
#endif
	ULONG volatile *a0_l, *a1_l, *a2_l;
	UWORD volatile *a1_w, *a2_w;

//L_1B:;
/*
														movem.l d1-d5/a0-a2,-(sp)
														bsr     L000216
														move.l  d0,d2
														bmi     L0002e4
														move.l  d0,d5
														lsl.l   #3,d0
														moveq.l #$60,d3
*/
	d1=D1, d2=D2, d3=D3, d4=D4, d5=D5, a0=A0, a1=A1, a2=A2;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000216( context );
#else
	L000216();
#endif
	D2 = D0;
	if ( (SLONG)d2 < 0 ) goto L0002e4;
	D5 = D0;
	D0 <<= 3;
	D3 = 0x60;

L00027e:;
/*
														sub.l   d3,d2
														bcc     L00027e
														add.l   d3,d2
														beq     L0002e4
														sub.l   d2,d3
														move.l  d3,d4
														lsl.l   #3,d3
														move.l  d1,d2
														addq.l  #1,d2
														and.w   #$fffe,d2
														lea.l   $00(a0,d2.l),a2
														add.l   d3,d1
														lea.l   $00(a0,d1.l),a1
														sub.l   d0,d2
														lsr.l   #1,d2
														move.l  d2,d0
														lsr.l   #1,d0
														subq.l  #1,d0
														swap.w  d0
*/
	D2 -= D3;
	if ( (SLONG)d2 >= 0 ) goto L00027e;
	D2 += D3;
	if ( D2 == 0 ) goto L0002e4;
	D3 -= D2;
	D4 = D3;
	D3 <<= 3;
	D2 = D1;
	D2++;
	D2 &= 0xfffffffe;
#if MXDRV_ENABLE_PORTABLE_CODE
	a2_l = (ULONG *)TO_PTR(A0+D2);
#else
	a2_l = (ULONG *)(A0+D2);
#endif
	D1 += D3;
#if MXDRV_ENABLE_PORTABLE_CODE
	a1_l = (ULONG *)TO_PTR(A0+D1);
#else
	a1_l = (ULONG *)(A0+D1);
#endif
	D2 -= D0;
	D2 >>= 1;
	D0 = D2;
	D0 >>= 1;
	D0--;

//L0002aa:;
/*
														swap.w  d0
*/

L0002ac:;
/*
														move.l  -(a2),-(a1)
														dbra    d0,L0002ac
														swap.w  d0
														dbra    d0,L0002aa
														and.w   #$0001,d2
														beq     L0002c0
														move.w  -(a2),-(a1)
*/
	*(--a1_l) = *(--a2_l);
	if ( (D0--) != 0 ) goto L0002ac;
	D2 &= 0xffff0001;
	if ( (D2 & 0xffff) == 0 ) goto L0002c0;
	a1_w = (UWORD *)a1_l; a2_w = (UWORD *)a1_l; *(--a1_w) = *(--a2_w); a1_l = (ULONG *)a1_w; a2_l = (ULONG *)a2_w;
#if MXDRV_ENABLE_PORTABLE_CODE
	A2 = TO_OFS(a2_l);
#else
	A2 = (UBYTE *)a2_l;
#endif

L0002c0:;
/*
														subq.w  #1,d4
*/
	D4--;
	D4 &= 0xffff;

L0002c2:;
/*
														clr.l   -(a1)
														clr.l   -(a1)
														dbra    d4,L0002c2
														subq.l  #1,d5
														swap.w  d5
*/
	*(--a1_l) = CLR;
	*(--a1_l) = CLR;
	if ( (D4--) != 0 ) goto L0002c2;
#if MXDRV_ENABLE_PORTABLE_CODE
	A1 = TO_OFS(a1_l);
#else
	A1 = (UBYTE *)a1_l;
#endif
	D5--;
	d5 = D5;

#if MXDRV_ENABLE_PORTABLE_CODE
	a0_l = (ULONG *)TO_PTR(A0);
#else
	a0_l = (ULONG *)A0;
#endif
//L0002ce:;
/*
														swap.w  d5
*/

L0002d0:;
/*
														move.l  (a0)+,d0
														beq     L0002d8
														add.l   d3,-$0004(a0)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D0 = GETBLONG(TO_OFS(a0_l)); a0_l++;
	if ( D0 == 0 ) goto L0002d8;
	PUTBLONG(TO_OFS(a0_l-1), D0+D3 );
#else
	D0 = GETBLONG(a0_l); a0_l++;
	if ( D0 == 0 ) goto L0002d8;
	PUTBLONG( a0_l-1, D0+D3 );
#endif

L0002d8:;
/*
														addq.w  #4,a0
														dbra    d5,L0002d0
														swap.w  d5
														dbra    d5,L0002ce
*/
	a0_l++;
	if ( (D5--) != 0 ) goto L0002d0;
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = TO_OFS(a0_l);
#else
	A0 = (UBYTE *)a0_l;
#endif

L0002e4:;
/*
														move.l  d1,d0
														movem.l (sp)+,d1-d5/a0-a2
														rts
*/
	D1 = D0;
	D1=d1, D2=d2, D3=d3, D4=d4, D5=d5, A0=a0, A1=a1, A2=a2;

}

/***************************************************************/

static void L_1C(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	uint32_t t0;
#else
	UBYTE volatile *t0;
#endif
	ULONG c0;
	ULONG d1,d2,d3,d4,d5,d6,d7;
#if MXDRV_ENABLE_PORTABLE_CODE
	uint32_t a0,a1,a2,a3,a4;
#else
	UBYTE volatile *a0,*a1,*a2,*a3,*a4;
#endif
	ULONG volatile *a1_l, *a2_l, *a3_l, *a4_l;
	UWORD volatile *a1_w, *a2_w, *a3_w;

//L_1C:;
/*
														movem.l d1-d7/a0-a4,-(sp)
														bsr     L000216
														tst.l   d0
														bmi     L000462
														add.l   a0,d1
														addq.l  #1,d1
														and.w   #$fffe,d1
														move.l  d1,d3
														move.l  d3,d7
														move.l  d0,d2
														exg.l   a0,a1
														bsr     L000216
														tst.l   d0
														bmi     L000462
														add.l   d1,d3
														add.l   a0,d1
														move.l  d1,d6
														move.l  d0,d1
														lsl.l   #3,d1
														add.l   a0,d1
														move.l  d1,d4
														suba.l  d3,a2
														bcs     L00045a
														moveq.l #$60,d1
														move.l  d2,d3
*/
	d1=D1, d2=D2, d3=D3, d4=D4, d5=D5, d6=D6, d7=D7, a0=A0, a1=A1, a2=A2, a3=A3, a4=A4;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000216( context );
#else
	L000216();
#endif
	if ( (SLONG)D0 < 0 ) goto L000462;
	D1 += (ULONG)A0;
	D1++;
	D1 &= 0xfffffffe;
	D3 = D1;
	D7 = D3;
	D2 = D0;
	t0 = A0; A0 = A1; A1 = t0;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000216( context );
#else
	L000216();
#endif
	if ( (SLONG)D0 < 0 ) goto L000462;
	D3 += D1;
	D1 += (ULONG)A0;
	D6 = D1;
	D1 = D0;
	D1 <<= 3;
	D1 += (ULONG)A0;
	D4 = D1;
	A2 -= D3;
	if ( (SLONG)D3 < 0 ) goto L00045a;
	D1 = 0x60;
	D3 = D2;

L00032c:;
/*
														sub.l   d1,d3
														bcc     L00032c
														add.l   d1,d3
														beq     L000342
														sub.l   d1,d3
														neg.l   d3
														move.l  d3,d1
														lsl.l   #3,d3
														cmp.l   a2,d3
														bhi     L00045a
*/
	D3 -= D1;
	if ( (SLONG)D3 >= 0 ) goto L00032c;
	D3 += D1;
	if ( D3 == 0 ) goto L000342;
	D3 -= D1;
	D3 = (ULONG)(-((SLONG)D3));
	D1 = D3;
	D3 <<= 3;
	if ( (ULONG)A2 < D3 ) goto L00045a;

L000342:;
/*
														add.l   d0,d3
														lsl.l   #3,d3
														add.l   d7,d3
														movea.l a0,a4
														cmp.l   a0,d3
														bcs     L00037a
														move.l  d0,d1
														lsl.l   #3,d1
														cmp.l   (L001ba8),d1
														bhi     L00045e
														movea.l (L001bac),a4
														movea.l a0,a3
														move.l  d0,d1
														subq.l  #1,d1
														swap.w  d1
*/
	D3 += D0;
	D3 <<= 3;
	D3 += D7;
	A4 = A0;
	if ( (ULONG)A0 > D3 ) goto L00037a;
	D1 = D0;
	D1 <<= 3;
	if ( G.L001ba8 < D1 ) goto L00045e;
	A4 = G.L001bac;
	A3 = A0;
	D1 = D0;
	D1--;

#if MXDRV_ENABLE_PORTABLE_CODE
	a3_l = (ULONG *)TO_PTR(A3); a4_l = (ULONG *)TO_PTR(A4);
#else
	a3_l = (ULONG *)A3; a4_l = (ULONG *)A4;
#endif
//L000366:;
/*
														swap.w  d1
*/

L000368:;
/*
														move.l  (a3)+,(a4)+
														move.l  (a3)+,(a4)+
														dbra    d1,L000368
														swap.w  d1
														dbra    d1,L000366
														movea.l (L001bac),a4
*/
	*(a4_l++) = *(a3_l++);
	*(a4_l++) = *(a3_l++);
	if ( (D1--) != 0 ) goto L000368;
	A4 = G.L001bac;
#if MXDRV_ENABLE_PORTABLE_CODE
	A3 = TO_OFS(a3_l);
#else
	A3 = (UBYTE *)a3_l;
#endif

L00037a:;
/*
														lsl.l   #3,d0
														move.l  d0,d5
														exg.l   a0,a1
														bsr     L_1B
														tst.l   d0
														bmi     L000466
														bsr     L000216
														move.l  d0,d2
														bmi     L000462
														add.l   a0,d1
														addq.l  #1,d1
														and.w   #$fffe,d1
														movea.l d1,a2
														add.l   d5,d1
														lsl.l   #3,d0
														add.l   a0,d0
														add.l   d5,d0
														movea.l d1,a3
														movea.l a3,a1
														sub.l   d0,d1
														move.l  d1,d7
														lsr.l   #2,d1
														move.w  sr,-(sp)
														subq.l  #1,d1
														swap.w  d1
*/
	D0 <<= 3;
	D5 = D0;
	t0 = A0; A0 = A1; A1 = t0;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_1B( context );
#else
	L_1B();
#endif
	if ( (SLONG)D0 < 0 ) goto L000466;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000216( context );
#else
	L000216();
#endif
	D2 = D0;
	if ( (SLONG)D2 < 0 ) goto L000462;
	D1 += (ULONG)A0;
	D1++;
	D1 &= 0xfffffffe;
#if MXDRV_ENABLE_PORTABLE_CODE
	A2 = D1;
#else
	A2 = (UBYTE *)D1;
#endif
	D1 += D5;
	D0 <<= 3;
	D0 += (ULONG)A0;
	D0 += D5;
#if MXDRV_ENABLE_PORTABLE_CODE
	A3 = D1;
#else
	A3 = (UBYTE *)D1;
#endif
	A1 = A3;
	D1 -= D0;
	D7 = D1;
	D1 >>= 1; c0 = D1&1; D1 >>= 1;
	D1--;

#if MXDRV_ENABLE_PORTABLE_CODE
	a2_l = (ULONG *)TO_PTR(A2); a3_l = (ULONG *)TO_PTR(A3);
#else
	a2_l = (ULONG *)A2; a3_l = (ULONG *)A3;
#endif
//L0003b6:;
/*
														swap.w  d1
*/

L0003b8:;
/*
														move.l  -(a2),-(a3)
														dbra    d1,L0003b8
														swap.w  d1
														dbra    d1,L0003b6
														move.w  (sp)+,sr
														bcc     L0003ca
														move.w  -(a2),-(a3)
*/
	*(--a3_l) = *(--a2_l);
	if ( (D1--) != 0 ) goto L0003b8;
	if ( c0 == 0 ) goto L0003ca;
	a2_w = (UWORD *)a2_l; a3_w = (UWORD *)a3_l; *(--a3_w) = *(--a2_w);
#if MXDRV_ENABLE_PORTABLE_CODE
	A2 = TO_OFS(a2_w); A3 = TO_OFS(a3_w);
#else
	A2 = (UBYTE *)a2_w; A3 = (UBYTE *)a3_w;
#endif

L0003ca:;
/*
														movea.l d0,a2
														suba.l  d5,a2
														cmpa.l  a2,a4
														beq     L0003ea
														move.l  d5,d1
														lsr.l   #3,d1
														subq.l  #1,d1
														swap.w  d1
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A2 = D0;
#else
	A2 = (UBYTE *)D0;
#endif
	A2 -= D5;
	if ( A2 == A4 ) goto L0003ea;
	D1 = D5;
	D1 >>= 3;
	D1--;
#if MXDRV_ENABLE_PORTABLE_CODE
	a2_l = (ULONG *)TO_PTR(A2); a4_l = (ULONG *)TO_PTR(A4);
#else
	a2_l = (ULONG *)A2; a4_l = (ULONG *)A4;
#endif

//L0003da:;
/*
														swap.w  d1
*/

L0003dc:;
/*
														move.l  (a4)+,(a2)+
														move.l  (a4)+,(a2)+
														dbra    d1,L0003dc
														swap.w  d1
														dbra    d1,L0003da
*/
	*(a2_l++) = *(a4_l++);
	*(a2_l++) = *(a4_l++);
	if ( (D1--) > 0 ) goto L0003dc;
#if MXDRV_ENABLE_PORTABLE_CODE
	A2 = TO_OFS(a2_l); A4 = TO_OFS(a4_l);
#else
	A2 = (UBYTE *)a2_l; A4 = (UBYTE *)a4_l;
#endif

L0003ea:;
/*
														movea.l d4,a2
														sub.l   d4,d6
														move.l  d6,d1
														lsr.l   #2,d1
														subq.l  #1,d1
														swap.w  d1
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A2 = D4;
#else
	A2 = (UBYTE *)D4;
#endif
	D6 -= D4;
	D1 = D6;
	D2 >>= 2;
	D1 --;
#if MXDRV_ENABLE_PORTABLE_CODE
	a2_l = (ULONG volatile *)TO_PTR(A2); a1_l = (ULONG volatile *)TO_PTR(A1);
#else
	a2_l = (ULONG *)A2; a1_l = (ULONG *)A1;
#endif

//L0003f6:;
/*
														swap.w  d1
*/

L0003f8:;
/*
														move.l  (a2)+,(a1)+
														dbra    d1,L0003f8
														swap.w  d1
														dbra    d1,L0003f6
														move.w  d6,d1
														and.w   #$0002,d1
														beq     L00040e
														move.w  (a2)+,(a1)+
*/
	*(a1_l++) = *(a2_l++);
	if ( (D1--) > 0 ) goto L0003f8;
	D1 = D6;
	D1 &= 0x00000002;
	if ( D1 == 0 ) goto L00040e;
	a1_w = (UWORD *)a1_l; a2_w = (UWORD *)a2_l; *(a1_w++) = *(a2_w++);
#if MXDRV_ENABLE_PORTABLE_CODE
	A1 = TO_OFS(a1_w); A2 = TO_OFS(a2_w);
#else
	A1 = (UBYTE *)a1_w; A2 = (UBYTE *)a2_w;
#endif

L00040e:;
/*
														and.w   #$0001,d6
														beq     L000416
														move.b  (a2)+,(a1)+
*/
	D6 &= 0x00000001;
	if ( D1 == 0 ) goto L000416;
#if MXDRV_ENABLE_PORTABLE_CODE
	*TO_PTR(A1++) = *TO_PTR(A2++);
#else
	*(A1++) = *(A2++);
#endif

L000416:;
/*
														suba.l  a0,a1
														move.l  d5,d1
														move.l  d2,d0
														lsl.l   #3,d0
														add.l   d0,d7
														subq.l  #1,d2
														swap.w  d2
*/
	A1 -= (ULONG)A0;
	D1 = D5;
	D0 = D2;
	D0 <<= 3;
	D7 += D0;
	D2--;

//L000424:;
/*
														swap.w  d2
*/

L000426:;
/*
														move.l  (a0),d0
														beq     L00042c
														add.l   d1,(a0)
*/
	D0 = GETBLONG( A0 );
	if ( D0 == 0 ) goto L00042c;
	PUTBLONG( A0, D0+D1 );

L00042c:;
/*
														addq.w  #8,a0
														dbra    d2,L000426
														swap.w  d2
														dbra    d2,L000424
														lsr.l   #3,d5
														subq.l  #1,d5
														swap.w  d5
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = A0+8;
#else
	A0 = (UBYTE *)(((ULONG *)A0)+2);
#endif
	if ( (D2--) > 0 ) goto L000426;
	D5 >>= 3;
	D5--;

//L00043e:;
/*
														swap.w  d5
*/

//L000440:;
/*
														move.l  (a0),d0
														beq     L000446
														add.l   d7,(a0)
*/
	D0 = GETBLONG( A0 );
	if ( D0 == 0 ) goto L000446;
	PUTBLONG( A0, D0+D7 );

L000446:;
/*
														addq.w  #8,a0
														dbra    d5,L000440
														swap.w  d5
														dbra    d5,L00043e
														move.l  a1,d0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = A0+8;
#else
	A0 = (UBYTE *)(((ULONG *)A0)+2);
#endif
	if ( (D2--) > 0 ) goto L000426;
	D5 >>= 3;
	D5--;
	D0 = (ULONG)A1;

L000454:;
/*
														movem.l (sp)+,d1-d7/a0-a4
														rts
*/
	D1=d1, D2=d2, D3=d3, D4=d4, D5=d5, D6=d6, D7=d7, A0=a0, A1=a1, A2=a2, A3=a3, A4=a4;
	return;

L00045a:;
/*
														moveq.l #$ff,d0
														bra     L000454
*/
	D0 = 0xffffffff;
	goto L000454;

L00045e:;
/*
														moveq.l #$fe,d0
														bra     L000454
*/
	D0 = 0xfffffffe;
	goto L000454;

L000462:;
/*
														moveq.l #$fd,d0
														bra     L000454
*/
	D0 = 0xfffffffd;
	goto L000454;

L000466:;
/*
														moveq.l #$fc,d0
														bra     L000454
*/
	D0 = 0xfffffffc;
	goto L000454;

}


/***************************************************************/

static void L_1D(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	ULONG d2, d3, d4;

//L_1D:;
/*
														move.b  (L001e08),d4
														move.w  d1,d3
														st.b    (L001e08)
														move.w  #$ffff,d1
														movem.l d2-d4,-(sp)
														bsr     L_0F
														movem.l (sp)+,d2-d4
														bra     L000496
*/
	D4 = G.L001e08;
	D3 = D1;
	G.L001e08 = SET;
	D1 = 0xffff;
	d2 = D2, d3 = D3, d4 = D4;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_0F( context );
#else
	L_0F();
#endif
	D4 = d4, D3 = d3, D2 = d2;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000496( context );
#else
	L000496();
#endif
}


/***************************************************************/

static void L_1E(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//	ULONG d2_, d3_, d4_;

//L_1E:;
/*
		move.b  (L001e08),d4
		move.w  d1,d3
		move.w  #$ffff,(L001e1c)
		st.b    (L001e08)
*/

	D4 = G.L001e08;
	D3 = D1;
	G.L001e1c = 0xffff;
	G.L001e08 = SET;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000496( context );
#else
	L000496();
#endif
}


/***************************************************************/

static void L000496(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	ULONG d2, d3, d4;

//L000496:;
/*
														ori.w   #$0700,sr
														andi.b  #$f7,$00e88009
														andi.w  #$f8ff,sr
														tst.b   (L001e13)
														bne     L0004c0
														subq.w  #1,d2
														bcs     L0004c0
														movem.l d2-d4,-(sp)
*/
	if ( G.L001e13 != 0 ) goto L0004c0;
	if ( (D2--) == 0 ) goto L0004c0;
	d2 = D2, d3 = D3, d4 = D4;

L0004b4:;
/*
														bsr     L0000dc
														dbra    d2,L0004b4
														movem.l (sp)+,d2-d4
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L0000dc( context );
#else
	L0000dc();
#endif
	if ( (D2--) != 0 ) goto L0004b4;
	D4 = d4, D3 = d3, D2 = d2;

L0004c0:;
/*
														move.w  d3,(L001e1c)
														move.b  d4,(L001e08)
														bne     L_1F
														tst.b   (L001e13)
														bne     L_1F
														bsr     L00056a
														moveq.l #$12,d1
														move.b  (L001e0c),d2
														bsr     L_WRITEOPM
														moveq.l #$14,d1
														moveq.l #$3a,d2
														bsr     L_WRITEOPM
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e1c = (uint16_t)D3;
	G.L001e08 = (uint8_t)D4;
	if ( D4 != 0 ) { L_1F( context ); return; }
	if ( G.L001e13 != 0 ) { L_1F( context ); return; }
	L00056a( context );
#else
	G.L001e1c = D3;
	G.L001e08 = D4;
	if ( D4 != 0 ) { L_1F(); return; }
	if ( G.L001e13 != 0 ) { L_1F(); return; }
	L00056a();
#endif
	D1 = 0x00000012;
	D2 = G.L001e0c;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
#endif
	D1 = 0x00000014;
	D2 = 0x0000003a;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
#endif

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L_1F( context );
#else
	L_1F();
#endif
}


/***************************************************************/

static void L_1F(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L_1F:;
/*
														move.w  (L001ba6),d0
														rts
*/
   D0 = G.L001ba6;
}


/***************************************************************/

static void L_0D(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L_0D:;
/*
														cmp.b   #$f0,d1
														beq     L000552
														cmp.b   #$fc,d1
														beq     L00052e
														tst.l   d1
														bmi     L000534
														tst.b   (L001e18)
														bne     L_ERROR
														move.l  a2,(L001e30)
														move.l  a1,(L001e24)
														move.l  a1,(L001e28)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( (UBYTE)D1 == 0xf0 ) { L000552( context ); return; }
	if ( (UBYTE)D1 == 0xfc ) goto L00052e;
	if ( (SLONG)D1 < 0 ) { L000534( context ); return; }
	if ( G.L001e18 != 0 ) { L_ERROR( context ); return; }
#else
	if ( (UBYTE)D1 == 0xf0 ) { L000552(); return; }
	if ( (UBYTE)D1 == 0xfc ) goto L00052e;
	if ( (SLONG)D1 < 0 ) { L000534(); return; }
	if ( G.L001e18 != 0 ) { L_ERROR(); return; }
#endif
	G.L001e30 = A2;
	G.L001e24 = A1;
	G.L001e28 = A1;

L000510:;
/*
														tst.w   (a1)
														beq     L000518
														addq.w  #6,a1
														bra     L000510
*/
	if ( GETBWORD(A1) == 0 ) goto L000518;
#if MXDRV_ENABLE_PORTABLE_CODE
	A1 = A1+6;
#else
	A1 = (UBYTE *)(((UWORD *)A1)+3);
#endif
	goto L000510;

L000518:;
/*
														subq.w  #6,a1
														move.l  a1,(L001e2c)
														st.b    (L001e18)
														st.b    (L001e19)
														movea.l L001e24(pc),a0
														bra     L000788
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A1 = A1-6;
#else
	A1 = (UBYTE *)(((UWORD *)A1)-3);
#endif
	G.L001e2c = A1;
	G.L001e18 = SET;
	G.L001e19 = SET;
	A0 = G.L001e24;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000788( context ); return;
#else
	L000788(); return;
#endif

L00052e:;
/*
														move.b  L001e19(pc),d0
														rts
*/
	D0 = G.L001e19;
}


/***************************************************************/

static void L000534(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L000534:;
/*
														clr.b   (L001e18)
														clr.b   (L002230)
														clr.b   (L002231)
														movea.l L001e34(pc),a0
														move.l  (a0),(L002218)
														move.l  $0004(a0),(L00221c)
														bra     L00063e
*/
	G.L001e18 = CLR;
	G.L002230 = CLR;
	G.L002231 = CLR;
	A0 = G.L001e34;
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L002218 = GETBLONG( A0 );
	G.L00221c = GETBLONG( A0+4 );
	L00063e( context );
#else
	G.L002218 = (UBYTE *)GETBLONG( A0 );
	G.L00221c = (UBYTE *)GETBLONG( A0+4 );
	L00063e();
#endif
}


/***************************************************************/

static void L000552(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L000552:;
/*
														bsr     L000534
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L000534( context );
#else
	L000534();
#endif

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L000554( context );
#else
	L000554();
#endif
}


/***************************************************************/

static void L000554(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L000554:;
/*
														movea.l L001e30(pc),a0
														pea.l   (a0)
														DOS     _MFREE
														addq.w  #4,sp
														moveq.l #$00,d0
														move.l  d0,(L001e30)
														move.b  d0,(L001e19)
														rts
*/
	D0 = 0;
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e30 = D0;
	G.L001e19 = (uint8_t)D0;
#else
	G.L001e30 = (UBYTE *)D0;
	G.L001e19 = D0;
#endif
}


/***************************************************************/

static void L00056a(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L00056a:;
/*
														move.w  sr,-(sp)
														ori.w   #$0700,sr
														clr.b   (L001e13)
														tst.b   (L001e08)
														bne     L000596
														lea.l   L_OPMINT(pc),a1
														suba.l  a0,a0
														move.l  a1,$010c(a0)
														movea.l #$00e88000,a0
														ori.b   #$08,$0009(a0)
														ori.b   #$08,$0015(a0)
*/
	G.L001e13 = CLR;
	if ( G.L001e08 != 0 ) goto L000596;
#if MXDRV_ENABLE_PORTABLE_CODE
	SETOPMINT( context, L_OPMINT );
#else
	SETOPMINT( L_OPMINT );
#endif

L000596:;
/*
														move.w  (sp)+,sr
														rts
*/
}


/***************************************************************/

static void L_FREE(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L_FREE:;
/*
														bsr     L00063e
														move.l  (L00220c),$0090(a0)
														pea.l   L000000-$0000f0(pc)
														DOS     _MFREE
														addq.w  #4,sp
														tst.b   (L001e19)
														beq     L0005b2
														bra     L000554
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L00063e( context );
	if ( G.L001e19 == 0 ) goto L0005b2;
	L000554( context ); return;
#else
	L00063e();
	if ( G.L001e19 == 0 ) goto L0005b2;
	L000554(); return;
#endif

L0005b2:;
/*
		rts
*/
	return;
}


/***************************************************************/

static void L_SETMDX(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	ULONG d1;
#if MXDRV_ENABLE_PORTABLE_CODE
	uint32_t a1;
#else
	UBYTE volatile *a1;
#endif

//L_SETMDX:;
/*
														tst.b   (L001e18)
														beq     L0005c4
														movem.l d1/a1,-(sp)
														bsr     L000552
														movem.l (sp)+,d1/a1
*/
	if ( G.L001e18 == 0 ) goto L0005c4;
	d1 = D1, a1 = A1;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000552( context );
#else
	L000552();
#endif
	A1 = a1, D1 = d1;

L0005c4:;
/*
														lea.l   (L002230),a2
														movea.l L001e34(pc),a0
														move.l  a0,(L002218)
														move.l  (L002220),d0
														bra     L0005f8
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A2 = TO_OFS(&G.L002230);
#else
	A2 = &G.L002230;
#endif
	A0 = G.L001e34;
	G.L002218 = A0;
	D0 = G.L002220;
#if MXDRV_ENABLE_PORTABLE_CODE
	L0005f8( context );
#else
	L0005f8();
#endif
}


/***************************************************************/

static void L_SETPDX(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	ULONG d1;
#if MXDRV_ENABLE_PORTABLE_CODE
	uint32_t a1;
#else
	UBYTE volatile *a1;
#endif

//L_SETPDX:;
/*
														tst.b   (L001e18)
														beq     L0005e8
														movem.l d1/a1,-(sp)
														bsr     L000552
														movem.l (sp)+,d1/a1
*/
	if ( G.L001e18 == 0 ) goto L0005e8;
	d1 = D1, a1 = A1;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000552( context );
#else
	L000552();
#endif
	A1 = a1, D1 = d1;

L0005e8:;
/*
														lea.l   (L002231),a2
														movea.l L001e38(pc),a0
														move.l  a0,(L00221c)
														move.l  (L002224),d0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A2 = TO_OFS(&G.L002231);
#else
	A2 = &G.L002231;
#endif
	A0 = G.L001e38;
	G.L00221c = A0;
	D0 = G.L002224;

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L0005f8( context );
#else
	L0005f8();
#endif
}


/***************************************************************/

static void L0005f8(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	ULONG d1;
#if MXDRV_ENABLE_PORTABLE_CODE
	uint32_t a0, a1, a2;
#else
	UBYTE volatile *a0, *a1, *a2;
#endif
	ULONG volatile *a1_l, *a0_l;
//	UBYTE volatile *a2_b, a1_b, *a0_b;

//L0005f8:;
/*
														cmp.l   d1,d0
														bcs     L000630
														movem.l d1/a0-a2,-(sp)
														bsr     L00063e
														movem.l (sp)+,d1/a0-a2
														move.w  d1,d0
														andi.w  #$0003,d0
														lsr.l   #2,d1
														swap.w  d1
*/
	if ( D1 > D0 ) goto L000630;
	d1 = D1, a0 = A0, a1 = A1, a2 = A2;
#if MXDRV_ENABLE_PORTABLE_CODE
	L00063e( context );
#else
	L00063e();
#endif
	A2 = a2, A1 = a1, A0 = a0, D1 = d1;
	D0 = D1;
	D0 &= 0x0003;
#if MXDRV_ENABLE_PORTABLE_CODE
	a1_l = (ULONG *)TO_PTR(A1); a0_l = (ULONG *)TO_PTR(A0);
#else
	a1_l = (ULONG *)A1; a0_l = (ULONG *)A0;
#endif
	D1 >>= 2;

//L000610:;
/*
														swap.w  d1
*/


L000612:;
/*
														move.l  (a1)+,(a0)+
														dbra    d1,L000612
														swap.w  d1
														dbra    d1,L000610
														tst.w   d0
														beq     L00062a
														subq.w  #1,d0
*/
	*(a0_l++) = *(a1_l++);
	if ( D1-- != 0 ) goto L000612;
#if MXDRV_ENABLE_PORTABLE_CODE
	A1 = TO_OFS(a1_l); A0 = TO_OFS(a0_l);
#else
	A1 = (UBYTE *)a1_l; A0 = (UBYTE *)a0_l;
#endif
	if ( D0 == 0 ) goto L00062a;
	D0--;

L000624:;
/*
														move.b  (a1)+,(a0)+
														dbra    d0,L000624
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	*TO_PTR(A0++) = *TO_PTR(A1++);
#else
	*(A0++) = *(A1++);
#endif
	if ( D0-- != 0 ) goto L000624;

L00062a:;
/*
														st.b    (a2)
														moveq.l #$00,d0
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	*TO_PTR(A2) = SET;
#else
	*(A2) = SET;
#endif
	D0 = 0;
	return;

L000630:;
/*
														bset.l  #$1f,d0
														rts
*/
	D0 |= (1<<0x1f);
}


/***************************************************************/

static void L_STOP(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L_STOP:;
/*
														tst.b   (L001e18)
														bne     L000552
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( G.L001e18 ) { L000552( context ); return; }
#else
	if ( G.L001e18 ) { L000552(); return; }
#endif

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L00063e( context );
#else
	L00063e();
#endif
}


/***************************************************************/

static void L00063e(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L00063e:;
/*
														move.b  #$01,(L001e13)
														move.w  sr,-(sp)
														ori.w   #$0700,sr
														bsr     L0006c4
														movea.l $0088.w,a0
														move.l  -$0008(a0),d0
														cmp.l   #$50434d34,d0           ;'PCM4'
														beq     L000664
														cmp.l   #$50434d38,d0           ;'PCM8'
														bne     L00066a
*/
	G.L001e13 = 0x01;
#if MXDRV_ENABLE_PORTABLE_CODE
	L0006c4( context );
#else
	L0006c4();
#endif
	if ( !PCM8 ) goto L00066a;

//L000664:;
/*
														move.w  #$0100,d0
														trap    #2
*/
	D0 = 0x0100;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif

L00066a:;
/*
														tst.b   (L001df4)
														beq     L00067a
														move.w  #$01ff,d0
														trap    #2
														clr.b   (L001df4)
*/
	if ( G.L001df4 == 0 ) goto L00067a;
	D0 = 0x01ff;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif
	G.L001df4 = CLR;

L00067a:;
/*
														moveq.l #$0f,d2
														moveq.l #$e0,d1
*/
	D2 = 0x0f;
	D1 = 0xe0;

L00067e:;
/*
														bsr     L_WRITEOPM
														addq.b  #1,d1
														bne     L00067e
														lea.l   L00223c(pc),a0
														lea.l   L001bb4(pc),a1
														moveq.l #$07,d3
														moveq.l #$00,d2
														moveq.l #$08,d1
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
#endif
	D1++;
	if ( (UBYTE)D1 != 0 ) goto L00067e;
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = TO_OFS(&G.L00223c[0]);
	A1 = TO_OFS(&G.L001bb4[0]);
#else
	A0 = &G.L00223c[0];
	A1 = &G.L001bb4[0];
#endif
	D3 = 0x07;
	D2 = 0x00;
	D1 = 0x08;

L000694:;
/*
														bsr     L_WRITEOPM
														move.b  d2,(a0)+
														move.b  d2,(a1)+
														addq.b  #1,d2
														dbra    d3,L000694
														movea.l #$00e88000,a0
														andi.b  #$f7,$0009(a0)
														andi.b  #$f7,$0015(a0)
														suba.l  a0,a0
														move.l  $0004(a5),$010c(a0)
														move.w  (sp)+,sr
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
	*TO_PTR(A0++) = (UBYTE)D2;
	*TO_PTR(A1++) = (UBYTE)D2;
#else
	L_WRITEOPM();
	*(A0++) = D2;
	*(A1++) = D2;
#endif
	D2++;
	if ( D3-- != 0 ) goto L000694;

}


/***************************************************************/

static void L_PAUSE(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L_PAUSE:;
/*
														st.b    (L001e12)
*/
	G.L001e12 = SET;
	G.STOPMUSICTIMER = SET;

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L0006c4( context );
#else
	L0006c4();
#endif
}


/***************************************************************/

static void L_PAUSE_(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L_PAUSE:;
/*
														st.b    (L001e12)
*/
	G.L001e12 = SET;

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L0006c4( context );
#else
	L0006c4();
#endif
}


/***************************************************************/

static void L0006c4(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {


//L0006c4:;
/*
														moveq.l #$07,d7
														lea.l   CHBUF_FM(pc),a6
*/
	D7 = 0x07;
#if MXDRV_ENABLE_PORTABLE_CODE
	A6 = TO_OFS(&MXWORK_CHBUF_FM[0]);
#else
	A6 = &MXWORK_CHBUF_FM[0];
#endif

L0006ca:;
/*
														moveq.l #$7f,d0
														bsr     L000e28
														lea.l   $0050(a6),a6
														dbra    d7,L0006ca
														movea.l $0088.w,a0
														move.l  -$0008(a0),d0
														cmp.l   #$50434d34,d0           ;'PCM4'
														beq     L0006f0
														cmp.l   #$50434d38,d0           ;'PCM8'
														bne     L00070c
*/
	D0 = 0x7f;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000e28( context );
	A6+=sizeof(MXWORK_CH);
#else
	L000e28();
	A6++;
#endif
	if ( D7-- != 0 ) goto L0006ca;
	if ( !PCM8 ) goto L00070c;

//L0006f0:;
/*
														move.w  #$01fc,d0
														moveq.l #$ff,d1
														trap    #2
														cmp.b   #$01,d0
														bne     L000706
														move.w  #$0101,d0
														trap    #2
														rts
*/
	D0 = 0x01fc;
	D1 = 0xffffffff;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif
	if ( (UBYTE)D0 != 0x01 ) goto L000706;
	D0 = 0x0101;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif
	return;

L000706:;
/*
														moveq.l #$67,d0
														moveq.l #$01,d1
														trap    #15
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	ADPCMMOD_STOP( context );
#else
	ADPCMMOD_STOP();
#endif

L00070c:;
/*
														moveq.l #$67,d0
														moveq.l #$00,d1
														trap    #15
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	ADPCMMOD_END( context );
#else
	ADPCMMOD_END();
#endif
}


/***************************************************************/

static void L_CONT(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {


//L_CONT:;
/*
														clr.b   (L001e12)
														moveq.l #$07,d7
														lea.l   CHBUF_FM(pc),a6
*/
	G.L001e12 = CLR;
	G.STOPMUSICTIMER = CLR;
	G.MUSICTIMER = G.L001e0c;
	D7 = 0x07;
#if MXDRV_ENABLE_PORTABLE_CODE
	A6 = TO_OFS(&MXWORK_CHBUF_FM[0]);
#else
	A6 = &MXWORK_CHBUF_FM[0];
#endif

L00071e:;
/*
														bsr     L000dfe
														lea.l   $0050(a6),a6
														dbra    d7,L00071e
														movea.l $0088.w,a0
														move.l  -$0008(a0),d0
														cmp.l   #$50434d34,d0           ;'PCM4'
														beq     L000742
														cmp.l   #$50434d38,d0           ;'PCM8'
														bne     L000756
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L000dfe( context );
	A6+=sizeof(MXWORK_CH);
#else
	L000dfe();
	A6++;
#endif
	if ( D7-- != 0 ) goto L00071e;
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( !PCM8 ) { L000756( context ); return; }
#else
	if ( !PCM8 ) { L000756(); return; }
#endif

//L000742:;
/*
														move.w  #$01fc,d0
														moveq.l #$ff,d1
														trap    #2
														cmp.b   #$01,d0
														bne     L000756
														move.w  #$0102,d0
														trap    #2
*/
	D0 = 0x01fc;
	D1 = 0xffffffff;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
	if ( (UBYTE)D0 != 0x01 ) { L000756( context ); return; }
	D0 = 0x0102;
	PCM8_SUB( context );
#else
	PCM8_SUB();
	if ( (UBYTE)D0 != 0x01 ) { L000756(); return; }
	D0 = 0x0102;
	PCM8_SUB();
#endif

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L000756( context ); return;
#else
	L000756(); return;
#endif
}


/***************************************************************/

static void L000756(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L000756:;
/*
														moveq.l #$30,d2
														move.b  L001e08(pc),d1
														bne     L000760
														moveq.l #$3a,d2
*/
	D2 = 0x30;
	D1 = G.L001e08;
	if ( D1 != 0 ) goto L000760;
	D2 = 0x3a;

L000760:;
/*
														moveq.l #$14,d1
														bra     L_WRITEOPM
*/
	D1 = 0x14;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context ); return;
#else
	L_WRITEOPM(); return;
#endif
}


/***************************************************************/

static void L000766(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L000766:;
/*
														movea.l L001e28(pc),a0
														movea.l L001e24(pc),a1
														subq.w  #6,a0
														cmpa.l  a1,a0
														bcc     L000788
														movea.l L001e2c(pc),a0
														bra     L000788
*/
	A0 = G.L001e28;
	A1 = G.L001e24;
	A0 -= 0x06;
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( A1 <= A0 ) { L000788( context ); return; }
	A0 = G.L001e2c;
	L000788( context ); return;
#else
	if ( A1 <= A0 ) { L000788(); return; }
	A0 = G.L001e2c;
	L000788(); return;
#endif
}


/***************************************************************/

static void L00077a(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L00077a:;
/*
														movea.l L001e28(pc),a0
														addq.w  #6,a0
														tst.w   (a0)
														bne     L000788
														movea.l L001e24(pc),a0
*/
	A0 = G.L001e28;
	A0 += 0x06;
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( GETBWORD( A0 ) != 0 ) { L000788( context ); return; }
#else
	if ( GETBWORD( A0 ) != 0 ) { L000788(); return; }
#endif
	A0 = G.L001e24;

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L000788( context );
#else
	L000788();
#endif
}


/***************************************************************/

static void L000788(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L000788:;
/*
														move.l  a0,(L001e28)
														move.w  (a0),(L001e22)
														movea.l $0002(a0),a1
														move.l  (a1),(L00221c)
														addq.w  #4,a1
														move.w  (a1),d0
														not.w   d0
														move.w  $0002(a1),d1
														not.w   d1
														move.b  d0,(L002230)
														move.b  d1,(L002231)
														move.l  a1,(L002218)
														clr.w   (L001e1c)
														bra     L0007c0
*/
	G.L001e28 = A0;
	G.L001e22 = GETBWORD( A0 );
#if MXDRV_ENABLE_PORTABLE_CODE
	A1 = GETBLONG( A0+2 );
	G.L00221c = GETBLONG( A1 );
#else
	A1 = (UBYTE *)GETBLONG( A0+2 );
	G.L00221c = (UBYTE *)GETBLONG( A1 );
#endif
	A1 += 4;
	D0 = GETBWORD( A1 );
	D0 = ~D0;
	D1 = GETBWORD( A1+2 );
	D1 = ~D1;
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L002230 = (uint8_t)D0;
	G.L002231 = (uint8_t)D1;
#else
	G.L002230 = D0;
	G.L002231 = D1;
#endif
	G.L002218 = A1;
	G.L001e1c = CLR;
#if MXDRV_ENABLE_PORTABLE_CODE
	L0007c0( context ); return;
#else
	L0007c0(); return;
#endif

}


/***************************************************************/

static void L_PLAY(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L_PLAY:;
/*
														clr.w   (L001e1c)
														bra     L0007c0
*/
	G.L001e1c = CLR;
#if MXDRV_ENABLE_PORTABLE_CODE
	L0007c0( context ); return;
#else
	L0007c0(); return;
#endif
}


/***************************************************************/

static void L_0F(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L_0F:;
/*
														move.w  d1,(L001e1c)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e1c = (uint16_t)D1;
#else
	G.L001e1c = D1;
#endif

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L0007c0( context );
#else
	L0007c0();
#endif
}


/***************************************************************/

static void L0007c0(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L0007c0:;
	G.PLAYTIME = 0;
// checker
	G.FATALERROR = 0;
// checker end
/*
														clr.b   (L001e14)
														clr.b   (L001e15)
														clr.b   (L001e17)
														clr.b   (L001e13)
														tst.b   (L001e12)
														beq     L0007f4
														movea.l $0088.w,a0
														move.l  -$0008(a0),d0
														cmp.l   #$50434d34,d0           ;'PCM4'
														beq     L0007ee
														cmp.l   #$50434d38,d0           ;'PCM8'
														bne     L0007f4
*/
	G.L001e14 = CLR;
	G.L001e15 = CLR;
	G.L001e17 = CLR;
	G.L001e13 = CLR;
	if ( G.L001e12 == 0 ) goto L0007f4;
	if ( !PCM8 ) goto L0007f4;

//L0007ee:;
/*
														move.w  #$0100,d0
														trap    #2
*/
	D0 = 0x0100;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif

L0007f4:;
/*
														clr.b   (L001e12)
														clr.b   (L001df4)
														move.w  #$01ff,(L001e1a)
														move.w  #$01ff,(L001e06)
														clr.w   (L002246)
														clr.w   (L001ba6)
														move.b  (L002230),d0
														beq     L_ERROR
														bsr     L00063e
														movea.l (L002218),a2
														move.w  $0002(a2),d1
														bmi     L000848
														tst.b   (L002231)
														beq     L_ERROR
														movea.l (L00221c),a0
														bra     L00083c
*/
	G.L001e12 = CLR;
	G.STOPMUSICTIMER = CLR;
	G.L001df4 = CLR;
	G.L001e1a = 0x01ff;
	G.L001e06 = 0x01ff;
	G.L002246 = CLR;
	G.L001ba6 = CLR;
	D0 = G.L002230;
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( D0 == 0 ) { L_ERROR( context ); return; }
	L00063e( context );
#else
	if ( D0 == 0 ) { L_ERROR(); return; }
	L00063e();
#endif
	A2 = G.L002218;
	D1 = GETBWORD( A2+2 );
	if ( (SWORD)D1 < 0 ) goto L000848;
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( G.L002231 == 0 ) { L_ERROR( context ); return; }
#else
	if ( G.L002231 == 0 ) { L_ERROR(); return; }
#endif
	A0 = G.L00221c;
	goto L00083c;

L000834:;
/*
														tst.l   (a0)
														beq     L_ERROR
														adda.l  (a0),a0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( GETBLONG( A0 ) == 0 ) { L_ERROR( context ); return; }
#else
	if ( GETBLONG( A0 ) == 0 ) { L_ERROR(); return; }
#endif
	A0 += GETBLONG( A0 );

L00083c:;
/*
														dbra    d1,L000834
														adda.w  $0004(a0),a0
														move.l  a0,(L00222c)

*/
	if ( D1-- != 0 ) goto L000834;
	A0 += GETBWORD( A0+4 );
	G.L00222c = A0;

L000848:;
/*
														adda.w  $0004(a2),a2
														movea.l a2,a1
														movea.l a2,a0
														moveq.l #$00,d0
														move.w  (a1)+,d0
														adda.l  d0,a2
														move.l  a2,(L002228)
														lea.l   CHBUF_FM(pc),a6
														lea.l   L00095a(pc),a3
														moveq.l #$ff,d6
														moveq.l #$00,d7
*/
	A2 += GETBWORD( A2+4 );
	A1 = A2;
	A0 = A2;
	D0 = 0x00000000;
	D0 = GETBWORD( A1 ); A1 += 2;
	A2 += D0;
	G.L002228 = A2;
#if MXDRV_ENABLE_PORTABLE_CODE
	A6 = TO_OFS(&MXWORK_CHBUF_FM[0]);
	A3 = 0;
#else
	A6 = &MXWORK_CHBUF_FM[0];
	A3 = (UBYTE *)0;
#endif
	D6 = 0xffffffff;
	D7 = 0x00000000;

L000866:;
/*
														movea.l a0,a2
														move.w  (a1)+,d0
														adda.l  d0,a2
														move.l  a2,(a6)
														move.l  a3,$0026(a6)
														move.l  a3,$0040(a6)
														move.w  d6,$0014(a6)
														move.b  d6,$0023(a6)
														move.b  d7,$0018(a6)
														move.b  #$00,$001d(a6)
														move.b  #$01,$001a(a6)
														move.b  #$08,$0022(a6)
														move.b  #$c0,$001c(a6)
														move.b  #$08,$001e(a6)
														clr.w   $0036(a6)
														clr.w   $004a(a6)
														clr.w   $0010(a6)
														clr.b   $0024(a6)
														clr.b   $001f(a6)
														clr.b   $0019(a6)
														clr.w   $0016(a6)
														cmp.w   #$0008,d7
														bcc     L0008d4
														moveq.l #$38,d1
														add.b   d7,d1
														moveq.l #$00,d2
														bsr     L_WRITEOPM
														addq.w  #1,d7
														lea.l   $0050(a6),a6
														bra     L000866
*/
	A2 = A0;
	D0 = GETBWORD( A1 ); A1 += 2;
	A2 += D0;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0000 = A2;
		p->S0026 = A3;
		p->S0040 = A3;
		p->S0014 = (UWORD)D6;
		p->S0023 = (UBYTE)D6;
		p->S0018 = (UBYTE)D7;
		p->S001d = 0x00;
		p->S001a = 0x01;
		p->S0022 = 0x08;
		p->S001c = 0xc0;
		p->S001e = 0x08;
		p->S0036 = (p->S0036 & 0xffff);
		p->S004a = CLR;
		p->S0010 = CLR;
		p->S0024 = CLR;
		p->S001f = CLR;
		p->S0019 = CLR;
		p->S0016 = CLR; p->S0017 = CLR;
	}
#else
	A6->S0000 = (UBYTE *)A2;
	A6->S0026 = (UBYTE *)A3;
	A6->S0040 = (UBYTE *)A3;
	A6->S0014 = D6;
	A6->S0023 = D6;
	A6->S0018 = D7;
	A6->S001d = 0x00;
	A6->S001a = 0x01;
	A6->S0022 = 0x08;
	A6->S001c = 0xc0;
	A6->S001e = 0x08;
	A6->S0036 = (A6->S0036 & 0xffff);
	A6->S004a = CLR;
	A6->S0010 = CLR;
	A6->S0024 = CLR;
	A6->S001f = CLR;
	A6->S0019 = CLR;
	A6->S0016 = CLR; A6->S0017 = CLR;
#endif
	if ( D7 >= 0x0008 ) goto L0008d4;
	D1 = 0x38;
	D1 += D7;
	D2 = 0x00;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
#endif
	D7++;
#if MXDRV_ENABLE_PORTABLE_CODE
	A6+=sizeof(MXWORK_CH);
#else
	A6++;
#endif
	goto L000866;

L0008d4:;
/*
														move.b  #$10,$001c(a6)
														move.b  #$08,$0022(a6)
														move.b  d7,$0018(a6)
														andi.b  #$07,$0018(a6)
														ori.b   #$80,$0018(a6)
														clr.b   $0004_b(a6)
														cmp.w   #$000f,d7
														beq     L000910
														addq.w  #1,d7
														lea.l   $0050(a6),a6
														cmp.w   #$0009,d7
														bne     L000866
														lea.l   CHBUF_PCM(pc),a6
														bra     L000866
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S001c = 0x10;
		p->S0022 = 0x08;
		p->S0018 = (UBYTE)D7;
		p->S0018 &= 0x07;
		p->S0018 |= 0x80;
		p->S0004_b = 0x00;
	}
#else
	A6->S001c = 0x10;
	A6->S0022 = 0x08;
	A6->S0018 = D7;
	A6->S0018 &= 0x07;
	A6->S0018 |= 0x80;
	A6->S0004_b = 0x00;
#endif
	if ( D7 == 0x000f ) goto L000910;
	D7++;
#if MXDRV_ENABLE_PORTABLE_CODE
	A6+=sizeof(MXWORK_CH);
#else
	A6++;
#endif
	if ( D7 != 0x09 ) goto L000866;
#if MXDRV_ENABLE_PORTABLE_CODE
	A6 = TO_OFS(&MXWORK_CHBUF_PCM[0]);
#else
	A6 = &MXWORK_CHBUF_PCM[0];
#endif
	goto L000866;


L000910:;
/*
														lea.l   (L001df6),a0
														moveq.l #$0f,d0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = TO_OFS(&G.L001df6[0]);
#else
	A0 = &G.L001df6[0];
#endif
	D0 = 0x0f;

L000916:;
/*
														clr.b   (a0)+
														dbra    d0,L000916
														clr.b   (L002232)
														moveq.l #$00,d2
														moveq.l #$01,d1
														bsr     L_WRITEOPM
														moveq.l #$0f,d1
														bsr     L_WRITEOPM
														moveq.l #$19,d1
														bsr     L_WRITEOPM
														moveq.l #$80,d2
														bsr     L_WRITEOPM
														moveq.l #$c8,d2
														moveq.l #$12,d1
														move.b  d2,(L001e0c)
														tst.b   (L001e08)
														bne     L00094c
														bsr     L_WRITEOPM
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	*TO_PTR(A0++) = CLR;
#else
	*(A0++) = CLR;
#endif
	if ( D0-- != 0 ) goto L000916;
	G.L002232 = CLR;
	D2 = 0x00;
	D1 = 0x01;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
	D1 = 0x0f;
	L_WRITEOPM( context );
	D1 = 0x19;
	L_WRITEOPM( context );
	D2 = 0x80;
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
	D1 = 0x0f;
	L_WRITEOPM();
	D1 = 0x19;
	L_WRITEOPM();
	D2 = 0x80;
	L_WRITEOPM();
#endif
	D2 = 0xc8;
	D1 = 0x12;
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e0c = (uint8_t)D2;
	G.MUSICTIMER = (uint8_t)D2;
#else
	G.L001e0c = D2;
	G.MUSICTIMER = D2;
#endif
	if ( G.L001e08 != 0 ) goto L00094c;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
#endif

L00094c:;
/*
														bsr     L00056a
														bsr     L000756
														moveq.l #$00,d0
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L00056a( context );
	L000756( context );
#else
	L00056a();
	L000756();
#endif
	D0 = 0;
}


/***************************************************************/

static void L_ERROR(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L_ERROR:;
/*
														moveq.l #$ff,d0
*/
	D0 = 0xffffffff;

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L00095a( context );
#else
	L00095a();
#endif
}


/***************************************************************/

static void L00095a(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L00095a:;
/*
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	(void)context;
#endif
}


/***************************************************************/

static void L_08(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L_08:;
/*
														tst.b   (L002230)
														beq     L000998
														movea.l (L002218),a0
														bra     L00096e
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( G.L002230 == 0 ) { L000998( context ); return; };
#else
	if ( G.L002230 == 0 ) { L000998(); return; };
#endif
	A0 = G.L002218;
	goto L00096e;

L000968:;
/*
														tst.w   (a0)
														beq     L000998
														adda.w  (a0),a0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( GETBWORD(A0) == 0 ) { L000998( context ); return; };
#else
	if ( GETBWORD(A0) == 0 ) { L000998(); return; };
#endif
	A0 += GETBWORD( A0 );

L00096e:;
/*
														dbra    d1,L000968
														adda.w  $0006(a0),a0
														move.l  a0,d0
														rts
*/
	if ( D1-- != 0 ) goto L000968;
	A0 += GETBWORD( A0+6 );
	D0 = (ULONG)A0;
}


/***************************************************************/

static void L_09(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L_09:;
/*
														tst.b   (L002231)
														beq     L000998
														movea.l (L00221c),a0
														bra     L00098c
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( G.L002231 == 0 ) { L000998( context ); return; }
#else
	if ( G.L002231 == 0 ) { L000998(); return; }
#endif
	A0 = G.L00221c;
	goto L00098c;

L000986:;
/*
														tst.l   (a0)
														beq     L000998
														adda.l  (a0),a0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( GETBLONG( A0 ) == 0 ) { L000998( context ); return; }
#else
	if ( GETBLONG( A0 ) == 0 ) { L000998(); return; }
#endif
	A0 += GETBLONG( A0 );

L00098c:;
/*
														dbra    d1,L000986
														adda.w  $0006(a0),a0
														move.l  a0,d0
														rts
*/
	if ( D1-- != 0 ) goto L000986;
	A0 += GETBWORD( A0+6 );
	D0 = (ULONG)A0;
}


/***************************************************************/
/*
L000998:;
		moveq.l #$00,d0
		rts
*/
static void L000998(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	D0 = 0;
}


/***************************************************************/

static void L_OPMINT(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	ULONG d0,d1,d2,d3,d4,d5,d6,d7;
#if MXDRV_ENABLE_PORTABLE_CODE
	uint32_t a0,a1,a2,a3,a4,a5,a6;
#else
	UBYTE volatile *a0,*a1,*a2,*a3,*a4,*a5;
	MXWORK_CH volatile *a6;
#endif
	UWORD volatile *a0_w;

	if ( G.FATALERROR ) {
		return;
	}

#if LOGINT
	FILE *fout;
	fout = fopen( "c:\\temp\\int.log", "ab+" );
	fprintf( fout, "%lu\n", timeGetTime() );
	fclose( fout );
#endif

//L_OPMINT:;
/*
														andi.b  #$f7,$00e88015
														move.l  a6,-(sp)
														movea.l $0006(sp),a6
														cmpa.l  $01a8.w,a6
														bne     L0009be
														pea.l   L0009b8(pc)
														move.w  sr,-(sp)
														jmp     (a6)
*/


//L0009b8:;
/*
														movea.l (sp)+,a6
														addq.w  #6,sp
														move.l  a6,-(sp)
*/


//L0009be:;
/*
														andi.w  #$faff,sr
														movem.l d0-d7/a0-a5,-(sp)
														lea.l   L00220c(pc),a5
														st.b    (L002245)
														tst.b   (L001e12)
														bne     L000a66
														lea.l   L001e1e(pc),a0
														lea.l   L001e17(pc),a1
														tst.b   (a1)
														beq     L000a66
														bpl     L0009ee
														move.b  #$7f,(a1)
														move.w  (a0),$0002(a0)
*/
	d0=D0, d1=D1, d2=D2, d3=D3, d4=D4, d5=D5, d6=D6, d7=D7, a0=A0, a1=A1, a2=A2, a3=A3, a4=A4, a5=A5, a6=A6;
	G.L002245 = SET;
	if ( G.L001e12 != 0 ) goto L000a66;
	a0_w = &G.L001e1e[0];
#if MXDRV_ENABLE_PORTABLE_CODE
	A1 = TO_OFS(&G.L001e17);
	if ( *TO_PTR(A1) == 0 ) goto L000a66;
	if ( (SBYTE)*TO_PTR(A1) >= 0 ) goto L0009ee;
	*TO_PTR(A1) = 0x7f;
#else
	A1 = &G.L001e17;
	if ( *(A1) == 0 ) goto L000a66;
	if ( (SBYTE)*(A1) >= 0 ) goto L0009ee;
	*(A1) = 0x7f;
#endif
	a0_w[1] = a0_w[0];

L0009ee:;
/*
														tst.w   $0002(a0)
														bmi     L0009fa
														subq.w  #2,$0002(a0)
														bra     L000a66
*/
	if ( (SWORD)a0_w[1] < 0 ) goto L0009fa;
	a0_w[1] -= 2;
	goto L000a66;

L0009fa:;
/*
														lea.l   L001e14(pc),a1
														cmpi.b  #$0a,(a1)
														bge     L000a12
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A1 = TO_OFS(&G.L001e14);
	if ( (SBYTE)*TO_PTR(A1) >= 0x0a ) goto L000a12;
#else
	A1 = &G.L001e14;
	if ( (SBYTE)*(A1) >= 0x0a ) goto L000a12;
#endif

L000a04:;
/*
														cmpi.b  #$3e,(a1)               ;'>'
														bge     L000a18
														addq.b  #1,(a1)
														move.w  (a0),$0002(a0)
														bra     L000a66
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( (SBYTE)*TO_PTR(A1) >= 0x3e ) goto L000a18;
	*TO_PTR(A1) += 1;
#else
	if ( (SBYTE)*(A1) >= 0x3e ) goto L000a18;
	*(A1) += 1;
#endif
	a0_w[1] = a0_w[0];
	goto L000a66;

L000a12:;
/*
														st.b    (L001e15)
														bra     L000a04
*/
	G.L001e15 = SET;
	goto L000a04;

L000a18:;
/*
														tst.b   (L001e18)
														beq     L000a26
														bsr     L00077a
														bra     L000c40
*/
	if ( G.L001e18 == 0 ) goto L000a26;
#if MXDRV_ENABLE_PORTABLE_CODE
	L00077a( context );
#else
	L00077a();
#endif
	goto L000c40;

L000a26:;
/*
														move.b  #$7f,(a1)
														clr.b   (L001e17)
														move.b  #$01,(L001e13)
														bsr     L_PAUSE
														movea.l $0088.w,a0
														move.l  -$0008(a0),d0
														cmp.l   #$50434d34,d0           ;'PCM4'
														beq     L000a50
														cmp.l   #$50434d38,d0           ;'PCM8'
														bne     L000a56
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	*TO_PTR(A1) = 0x7f;
#else
	*(A1) = 0x7f;
#endif
	G.L001e17 = CLR;
	G.L001e13 = 0x01;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_PAUSE_( context );  /* L_PAUSE()のタイマーを止めない */
#else
	L_PAUSE_();  /* L_PAUSE()のタイマーを止めない */
#endif
	if ( !PCM8 ) goto L000a56;

//L000a50:;
/*
														move.w  #$0100,d0
														trap    #2
*/
	D0 = 0x0100;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif

L000a56:;
/*
														tst.b   (L001df4)
														beq     L000a66
														move.w  #$01ff,d0
														trap    #2
														clr.b   (L001df4)
*/
	if ( G.L001df4 == 0 ) goto L000a66;
	D0 = 0x01ff;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif
	G.L001df4 = CLR;

L000a66:;
/*
														lea.l   L001e0c(pc),a0
														move.b  (a0),d2
														moveq.l #$12,d1
														tst.b   (L001e0a)
														bne     L000bd0
														move.b  $080e.w,d7
														and.b   #$0f,d7
														cmp.b   #$05,d7  ; OPT1+SHIFT
														beq     L000b16
														cmp.b   #$09,d7  ; OPT2+SHIFT
														beq.s   L000b0a
														cmp.b   #$06,d7  ; OPT1+CTRL
														beq     L000b04
														cmp.b   #$0a,d7  ; OPT2+CTRL
														beq     L000afc
														cmp.b   #$04,d7  ; OPT1
														beq     L000b6c
														cmp.b   #$08,d7  ; OPT2
														beq     L000b20
														cmp.b   #$02,d7  ; CTRL
														bne     L000bd0
														move.b  $080b.w,d7
														and.b   #$03,d7
														cmpi.b  #$80,$080a.w  ; XF3
														beq     L000aec
														cmp.b   #$02,d7  ; XF5
														beq     L000adc
														cmp.b   #$01,d7  ; XF4
														bne     L000bd0
														tst.b   (L001e10)
														bne     L000bca
														subq.b  #1,$0002(a0)  ; L001e0e
														bra     L000bca
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = TO_OFS(&G.L001e0c);
	D2 = *TO_PTR(A0);
#else
	A0 = &G.L001e0c;
	D2 = *(A0);
#endif
	D1 = 0x12;
	if ( G.L001e0a != 0 ) goto L000bd0;
	if ( KEY.OPT1 && KEY.SHIFT ) goto L000b16;
	if ( KEY.OPT2 && KEY.SHIFT ) goto L000b0a;
	if ( KEY.OPT1 && KEY.CTRL ) goto L000b04;
	if ( KEY.OPT2 && KEY.CTRL ) goto L000afc;
	if ( KEY.OPT1 ) goto L000b6c;
	if ( KEY.OPT2 ) goto L000b20;
	if ( !KEY.CTRL ) goto L000bd0;
	if ( KEY.XF3 ) goto L000aec;
	if ( KEY.XF5 ) goto L000adc;
	if ( !KEY.XF4 ) goto L000bd0;
	if ( G.L001e10 != 0 ) goto L000bca;
	G.L001e0e--;
	goto L000bca;

L000adc:;
/*
														tst.b   (L001e10)
														bne     L000bca
														addq.b  #1,$0002(a0)  ; L001e0e
														bra     L000bca
*/
	if ( G.L001e10 != 0 ) goto L000bca;
	G.L001e0e++;
	goto L000bca;

L000aec:;
/*
														tst.b   (L001e10)
														bne     L000bca
														clr.b   $0002(a0)  ; L001e0e
														bra     L000bca
*/
	if ( G.L001e10 != 0 ) goto L000bca;
	G.L001e0e = CLR;
	goto L000bca;

L000afc:;
/*
														move.b  #$ff,d2
														bra     L000bd8
*/
	D2 = 0xff;
	goto L000bd8;

L000b04:;
/*
														moveq.l #$00,d2
														bra     L000bd8
*/
	D2 = 0x00;
	goto L000bd8;

L000b0a:;
/*
														neg.b   d2
														lsr.b   #2,d2
														addq.b  #1,d2
														neg.b   d2
														bra     L000bd8
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D2 = (UBYTE)(-(int32_t)D2);
#else
	D2 = (UBYTE)-D2;
#endif
	D2 >>= 2;
	D2++;
#if MXDRV_ENABLE_PORTABLE_CODE
	D2 = (UBYTE)(-(int32_t)D2);
#else
	D2 = (UBYTE)-D2;
#endif
	goto L000bd8;

L000b16:;
/*
														neg.b   d2
														add.b   d2,d2
														neg.b   d2
														bra     L000bd8
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D2 = (UBYTE)(-(int32_t)D2);
#else
	D2 = (UBYTE)-D2;
#endif
	D2 += D2;
#if MXDRV_ENABLE_PORTABLE_CODE
	D2 = (UBYTE)(-(int32_t)D2);
#else
	D2 = (UBYTE)-D2;
#endif
	goto L000bd8;

L000b20:;
/*
														btst.b  #$00,$080b.w  ; XF4
														bne     L000b56
														btst.b  #$01,$080b.w  ; XF5
														beq     L000bd0
														tst.b   (L001e12)
														beq     L000b4a
														bpl     L000bbe
														tst.b   (L001e10)
														bne     L000bca
														bsr     L_CONT
														bra     L000bbe
*/
	if ( KEY.XF4 ) goto L000b56;
	if ( !KEY.XF5 ) goto L000bd0;
	if ( G.L001e12 == 0 ) goto L000b4a;
	if ( (SBYTE)G.L001e12 >= 0 ) goto L000bbe;
	if ( G.L001e10 != 0 ) goto L000bca;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_CONT( context );
#else
	L_CONT();
#endif
	goto L000bbe;

L000b4a:;
/*
														tst.b   (L001e10)
														bne     L000bca
														bsr     L_PAUSE
														bra     L000bbe
*/
	if ( G.L001e10 != 0 ) goto L000bca;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_PAUSE( context );
#else
	L_PAUSE();
#endif
	goto L000bbe;

L000b56:;
/*
														tst.b   (L001e12)
														beq     L000bca
														tst.b   (L001e10)
														bne     L000bca
														st.b    (L001e10)
														moveq.l #$00,d2
														bra     L000bec
*/
	if ( G.L001e12 == 0 ) goto L000bca;
	if ( G.L001e10 != 0 ) goto L000bca;
	G.L001e10 = SET;
	D2 = 0;
	goto L000bec;

L000b6c:;
/*
														move.b  $080b.w,d7
														beq     L000b82  ; !XF4 && !XF5
														and.b   #$03,d7
														cmp.b   #$02,d7  ; XF5
														beq     L000bae
														cmp.b   #$01,d7  ; XF4
														beq     L000b9c
*/
	if ( (!KEY.XF4) && (!KEY.XF5) ) goto L000b82;
	if ( KEY.XF4 ) goto L000bae;
	if ( KEY.XF5 ) goto L000b9c;

L000b82:;
/*
														cmpi.b  #$80,$080a.w ; XF3
														bne     L000bd0
														tst.b   (L001e10)
														bne     L000bca
*/
	if ( !KEY.XF3 ) goto L000bd0;
	if ( G.L001e10 != 0 ) goto L000bca;

//L000b90:;
/*
														move.w  #$0011,(L001e1e)
														st.b    (L001e17)
														bra     L000bca
*/
	G.L001e1e[0] = 0x0011;
	G.L001e17 = SET;
	goto L000bca;

L000b9c:;
/*
														tst.b   (L001e10)
														bne     L000bca
														tst.b   (L001e18)
														beq     L000bc4
														bsr     L000766
														bra     L000bbe
*/
	if ( G.L001e10 != 0 ) goto L000bca;
	if ( G.L001e18 == 0 ) goto L000bc4;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000766( context );
#else
	L000766();
#endif
	goto L000bbe;

L000bae:;
/*
														tst.b   (L001e10)
														bne     L000bca
														tst.b   (L001e18)
														beq     L000bc4
														bsr     L00077a
*/
	if ( G.L001e10 != 0 ) goto L000bca;
	if ( G.L001e18 == 0 ) goto L000bc4;
#if MXDRV_ENABLE_PORTABLE_CODE
	L00077a( context );
#else
	L00077a();
#endif

L000bbe:;
/*
														st.b    (L001e10)
														bra     L000c40
*/
	G.L001e10 = SET;
	goto L000c40;

L000bc4:;
/*
														bsr     L_PLAY
														bra     L000bbe
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L_PLAY( context );
#else
	L_PLAY();
#endif
	goto L000bbe;

L000bca:;
/*
														st.b    (L001e10)
														bra     L000bd4
*/
	G.L001e10 = SET;
	goto L000bd4;

L000bd0:;
/*
														clr.b   (L001e10)
*/
	G.L001e10 = CLR;

L000bd4:;
/*
														add.b   $0002(a0),d2  ; L001e0e
*/
	D2 += G.L001e0e;

L000bd8:;
/*
														tst.b   (L001e12)
														bne     L000be4
														tst.b   (L001e13)
														beq     L000bec
*/
	if ( G.L001e12 != 0 ) goto L000be4;
	if ( G.L001e13 == 0 ) goto L000bec;

L000be4:;
/*
														moveq.l #$00,d2
														bsr     L_WRITEOPM
														bra     L000c40
*/
	D2 = 0x00;
#if MXDRV_ENABLE_PORTABLE_CODE
	G.MUSICTIMER = (uint8_t)D2;
	L_WRITEOPM( context );
#else
	G.MUSICTIMER = D2;
	L_WRITEOPM();
#endif
	goto L000c40;

L000bec:;
/*
														bsr     L_WRITEOPM
														addq.w  #1,(L001ba6)
														lea.l   CHBUF_FM(pc),a6
														moveq.l #$00,d7
*/
//	G.MUSICTIMER = D2; /* ここは入れない */
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
#endif
	G.L001ba6++;
#if MXDRV_ENABLE_PORTABLE_CODE
	A6 = TO_OFS(&MXWORK_CHBUF_FM[0]);
#else
	A6 = &MXWORK_CHBUF_FM[0];
#endif
	D7 = 0x00;

L000bfa:;
/*
														bsr     L001050
														bsr     L0011b4
														move.w  L001e1c(pc),d0
														btst.l  d7,d0
														bne     L000c0c
														bsr     L000c66
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L001050( context );
	L0011b4( context );
#else
	L001050();
	L0011b4();
#endif
	D0 = G.L001e1c;
	if ( D0 & (1<<D7) ) goto L000c0c;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000c66( context );
#else
	L000c66();
#endif

L000c0c:;
/*
														lea.l   $0050(a6),a6
														addq.w  #1,d7
														cmp.w   #$0009,d7
														bcs     L000bfa
														tst.b   (L001df4)
														beq     L000c40
														lea.l   CHBUF_PCM(pc),a6
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A6+=sizeof(MXWORK_CH);
#else
	A6++;
#endif
	D7++;
	if ( D7 < 0x0009 ) goto L000bfa;
	if ( G.L001df4 == 0 ) goto L000c40;
#if MXDRV_ENABLE_PORTABLE_CODE
	A6 = TO_OFS(&MXWORK_CHBUF_PCM[0]);
#else
	A6 = &MXWORK_CHBUF_PCM[0];
#endif

L000c22:;
/*
														bsr     L001050
														bsr     L0011b4
														move.w  L001e1c(pc),d0
														btst.l  d7,d0
														bne     L000c34
														bsr     L000c66
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L001050( context );
	L0011b4( context );
#else
	L001050();
	L0011b4();
#endif
	D0 = G.L001e1c;
	if ( D0 & (1<<D7) ) goto L000c34;
#if MXDRV_ENABLE_PORTABLE_CODE
	L000c66( context );
#else
	L000c66();
#endif

L000c34:;
/*
														lea.l   $0050(a6),a6
														addq.w  #1,d7
														cmp.w   #$0010,d7
														bcs     L000c22
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A6+=sizeof(MXWORK_CH);
#else
	A6++;
#endif
	D7++;
	if ( D7 < 0x0010 ) goto L000c22;

L000c40:;
/*
														bsr     L000756
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L000756( context );
#else
	L000756();
#endif

//L000c44:;
/*
														tst.b   $00e90003
														bmi     L000c44
														move.b  #$1b,$00e90001
														clr.b   (L002245)
														movem.l (sp)+,d0-d7/a0-a6
														ori.b   #$08,$00e88015
														rte
*/
	D1 = 0x14;
	D2 = 0x1b;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
#endif
	G.L002245 = CLR;
	D0=d0, D1=d1, D2=d2, D3=d3, D4=d4, D5=d5, D6=d6, D7=d7, A0=a0, A1=a1, A2=a2, A3=a3, A4=a4, A5=a5, A6=a6;

}


/***************************************************************/

static void L000c66(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L000c66:;
/*
														btst.b  #$00,$0016(a6)
														beq     L000cce
														tst.b   $0020(a6)
														bne     L000cca
														tst.b   $0018(a6)
														bmi     L000cbe
														bsr     L000d84
														bsr     L000e66
														btst.b  #$03,$0016(a6)
														bne     L000cb4
														move.b  $0024(a6),$0025(a6)
														beq     L000c9e
														clr.l   $0036(a6)
														clr.w   $004a(a6)
														bsr     L001094
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( !(p->S0016 & (1<<0) ) ) goto L000cce;
		if ( p->S0020 ) goto L000cca;
		if ( (SBYTE)(p->S0018) < 0 ) goto L000cbe;
		L000d84( context );
		L000e66( context );
		if ( p->S0016 & (1<<3) ) goto L000cb4;
		p->S0025 = p->S0024;
		if ( p->S0025 == 0 ) goto L000c9e;
		p->S0036 = CLR;
		p->S004a = CLR;
		L001094( context );
	}
#else
	if ( !(A6->S0016 & (1<<0) ) ) goto L000cce;
	if ( A6->S0020 ) goto L000cca;
	if ( (SBYTE)(A6->S0018) < 0 ) goto L000cbe;
	L000d84();
	L000e66();
	if ( A6->S0016 & (1<<3) ) goto L000cb4;
	A6->S0025 = A6->S0024;
	if ( A6->S0025 == 0 ) goto L000c9e;
	A6->S0036 = CLR;
	A6->S004a = CLR;
	L001094();
#endif

L000c9e:;
/*
														btst.b  #$01,$0016(a6)
														beq     L000cb4
														moveq.l #$01,d1
														moveq.l #$02,d2
														bsr     L_WRITEOPM
														moveq.l #$00,d2
														bsr     L_WRITEOPM
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( !(p->S0016 & (1<<1) ) ) goto L000cb4;
	}
#else
	if ( !(A6->S0016 & (1<<1) ) ) goto L000cb4;
#endif
	D1 = 0x01;
	D2 = 0x02;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
	D2 = 0;
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
	D2 = 0;
	L_WRITEOPM();
#endif

L000cb4:;
/*
														clr.l   $000c(a6)
														bsr     L000cdc
														bsr     L000dfe
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S000c = CLR;
	}
	L000cdc( context );
	L000dfe( context );
#else
	A6->S000c = CLR;
	L000cdc();
	L000dfe();
#endif

L000cbe:;
/*
														bsr     L000e7e
														andi.b  #$fe,$0016(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L000e7e( context );
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0016 &= 0xfe;
	}
#else
	L000e7e();
	A6->S0016 &= 0xfe;
#endif
	return;

L000cca:;
/*
														subq.b  #1,$0020(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0020--;
	}
#else
	A6->S0020--;
#endif

L000cce:;
/*
														tst.b   $0018(a6)
														bmi     L000cda
														bsr     L000cdc
														bsr     L000dfe
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( (SBYTE)(p->S0018) < 0 ) goto L000cda;
	}
	L000cdc( context );
	L000dfe( context );
#else
	if ( (SBYTE)(A6->S0018) < 0 ) goto L000cda;
	L000cdc();
	L000dfe();
#endif

L000cda:;
/*
														rts
*/
}


/***************************************************************/

static void L000cdc(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

/*
L000d24:;
		.dc.b   $00,$01,$02,$04,$05,$06,$08,$09
		.dc.b   $0a,$0c,$0d,$0e,$10,$11,$12,$14
		.dc.b   $15,$16,$18,$19,$1a,$1c,$1d,$1e
		.dc.b   $20,$21,$22,$24,$25,$26,$28,$29
		.dc.b   $2a,$2c,$2d,$2e,$30,$31,$32,$34
		.dc.b   $35,$36,$38,$39,$3a,$3c,$3d,$3e
		.dc.b   $40,$41,$42,$44,$45,$46,$48,$49
		.dc.b   $4a,$4c,$4d,$4e,$50,$51,$52,$54
		.dc.b   $55,$56,$58,$59,$5a,$5c,$5d,$5e
		.dc.b   $60,$61,$62,$64,$65,$66,$68,$69
		.dc.b   $6a,$6c,$6d,$6e,$70,$71,$72,$74
		.dc.b   $75,$76,$78,$79,$7a,$7c,$7d,$7e
*/

	static UBYTE KeyCode[] = {
		0x00,0x01,0x02,0x04,0x05,0x06,0x08,0x09,
		0x0a,0x0c,0x0d,0x0e,0x10,0x11,0x12,0x14,
		0x15,0x16,0x18,0x19,0x1a,0x1c,0x1d,0x1e,
		0x20,0x21,0x22,0x24,0x25,0x26,0x28,0x29,
		0x2a,0x2c,0x2d,0x2e,0x30,0x31,0x32,0x34,
		0x35,0x36,0x38,0x39,0x3a,0x3c,0x3d,0x3e,
		0x40,0x41,0x42,0x44,0x45,0x46,0x48,0x49,
		0x4a,0x4c,0x4d,0x4e,0x50,0x51,0x52,0x54,
		0x55,0x56,0x58,0x59,0x5a,0x5c,0x5d,0x5e,
		0x60,0x61,0x62,0x64,0x65,0x66,0x68,0x69,
		0x6a,0x6c,0x6d,0x6e,0x70,0x71,0x72,0x74,
		0x75,0x76,0x78,0x79,0x7a,0x7c,0x7d,0x7e,
	};

//L000cdc:;
/*
														move.w  $0012(a6),d2
														add.w   $000c(a6),d2	; long
														add.w   $0036(a6),d2	; long
														cmp.w   $0014(a6),d2
														beq     L000d22
														move.w  d2,$0014(a6)
														move.w  #$17ff,d1
														cmp.w   d1,d2
														bls     L000d04
														tst.w   d2
														bpl     L000d02
														moveq.l #$00,d2
														bra     L000d04
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D2 = p->S0012;
		D2 = (UWORD)((UWORD)D2 + (UWORD)(p->S000c>>16));
		D2 = (UWORD)((UWORD)D2 + (UWORD)(p->S0036>>16));
		if ( D2 == p->S0014 ) goto L000d22;
		p->S0014 = (UWORD)D2;
	}
#else
	D2 = A6->S0012;
	D2 = (UWORD)((UWORD)D2 + (UWORD)(A6->S000c>>16));
	D2 = (UWORD)((UWORD)D2 + (UWORD)(A6->S0036>>16));
	if ( D2 == A6->S0014 ) goto L000d22;
	A6->S0014 = D2;
#endif
	D1 = 0x17ff;
	if ( D1 >= D2 ) goto L000d04;
	if ( (SWORD)D2 >= 0 ) goto L000d02;
	D2 = 0;
	goto L000d04;

L000d02:;
/*
														move.w  d1,d2
*/
	D2 = (UWORD)D1;

L000d04:;
/*
														add.w   d2,d2
														add.w   d2,d2
														moveq.l #$30,d1
														add.b   $0018(a6),d1
														bsr     L_WRITEOPM
														subq.b  #8,d1
														move.w  d2,-(sp)
														moveq.l #$00,d2
														move.b  (sp)+,d2
														move.b  L000d24(pc,d2.w),d2
														bsr     L_WRITEOPM
*/
	D2 = (UWORD)(D2*4);
	D1 = 0x30;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D1 += p->S0018;
	}
	L_WRITEOPM( context );
#else
	D1 += A6->S0018;
	L_WRITEOPM();
#endif
	D1 -= 8;
	D2 >>= 8;
	D2 = KeyCode[D2];
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
#endif

L000d22:;
/*
														rts
*/

}

/***************************************************************/

static void L000d84(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	UBYTE c0;

	static UBYTE CarrierSlot[] = {
		0x08,0x08,0x08,0x08,0x0c,0x0e,0x0e,0x0f,
	};

//L000d84:;
/*
														bclr.b  #$01,$0017(a6)
														beq     L000df4
														movea.l $0004(a6),a0
														andi.b  #$c0,$001c(a6)
														move.b  (a0)+,d0
														or.b    d0,$001c(a6)
														and.w   #$0007,d0
														move.b  L000df6(pc,d0.w),d3
														move.b  d3,$0019(a6)
														move.b  (a0)+,d0
														lsl.b   #3,d0
														or.b    $0018(a6),d0
														move.b  d0,$001d(a6)
														moveq.l #$40,d1
														add.b   $0018(a6),d1
														moveq.l #$03,d0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		c0 = p->S0017 & (1<<1); p->S0017 &= ~(1<<1);
		if ( !c0 ) goto L000df4;
		A0 = p->S0004;
		if ( A0 == 0 ) A0 = TO_OFS(&FAKEA6S0004[0]);
		p->S001c &= 0xc0;
		D0 = *TO_PTR(A0++);
		p->S001c |= D0;
		D0 &= 0x0007;
		D3 = CarrierSlot[D0];
		p->S0019 = (UBYTE)D3;
		D0 = *TO_PTR(A0++);
		D0 <<= 3;
		D0 |= p->S0018;
		p->S001d = (UBYTE)D0;
		D1 = 0x40;
		D1 += p->S0018;
		D0 = 0x03;
	}
#else
	c0 = A6->S0017 & (1<<1); A6->S0017 &= ~(1<<1);
	if ( !c0 ) goto L000df4;
	A0 = A6->S0004;
	if ( A0 == NULL ) A0 = &FAKEA6S0004[0];
	A6->S001c &= 0xc0;
	D0 = *(A0++);
	A6->S001c |= D0;
	D0 &= 0x0007;
	D3 = CarrierSlot[D0];
	A6->S0019 = D3;
	D0 = *(A0++);
	D0 <<= 3;
	D0 |= A6->S0018;
	A6->S001d = D0;
	D1 = 0x40;
	D1 += A6->S0018;
	D0 = 0x03;
#endif

L000dbc:;
/*
														move.b  (a0)+,d2
														bsr     L_WRITEOPM
														addq.b  #8,d1
														dbra    d0,L000dbc
														moveq.l #$03,d0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D2 = *TO_PTR(A0++);
	L_WRITEOPM( context );
#else
	D2 = *(A0++);
	L_WRITEOPM();
#endif
	D1 += 8;
	if ( D0-- != 0 ) goto L000dbc;
	D0 = 0x03;

L000dca:;
/*
														move.b  (a0)+,d2
														lsr.b   #1,d3
														bcc     L000dd2
														moveq.l #$7f,d2
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D2 = *TO_PTR(A0++);
#else
	D2 = *(A0++);
#endif
	c0 = D3&1; D3 >>= 1;
	if ( c0 == 0 ) goto L000dd2;
	D2 = 0x7f;

L000dd2:;
/*
														bsr     L_WRITEOPM
														addq.b  #8,d1
														dbra    d0,L000dca
														moveq.l #$0f,d0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
#endif
	D1 += 8;
	if ( D0-- != 0 ) goto L000dca;
	D0 = 0x0f;

L000dde:;
/*
														move.b  (a0)+,d2
														bsr     L_WRITEOPM
														addq.b  #8,d1
														dbra    d0,L000dde
														st.b    $0023(a6)
														ori.b   #$64,$0017(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D2 = *TO_PTR(A0++);
	L_WRITEOPM( context );
#else
	D2 = *(A0++);
	L_WRITEOPM();
#endif
	D1 += 8;
	if ( D0-- != 0 ) goto L000dde;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0023 = SET;
		p->S0017 |= 0x64;
	}
#else
	A6->S0023 = SET;
	A6->S0017 |= 0x64;
#endif

L000df4:;
/*
														rts
L000df6:;
		.dc.b   $08,$08,$08,$08,$0c,$0e,$0e,$0f
*/
}


/***************************************************************/

static void L000dfe(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	ULONG c0;

/*
L000e56:;
														.dc.b   $2a,$28,$25,$22,$20,$1d,$1a,$18
														.dc.b   $15,$12,$10,$0d,$0a,$08,$05,$02
*/
	static UBYTE Volume[] = {
		0x2a,0x28,0x25,0x22,0x20,0x1d,0x1a,0x18,
		0x15,0x12,0x10,0x0d,0x0a,0x08,0x05,0x02,
	};

//L000dfe:;
/*
														moveq.l #$00,d0
														move.b  $0022(a6),d0
														bclr.l  #$07,d0
														bne     L000e0e
														move.b  L000e56(pc,d0.w),d0
*/
	D0 = 0x00;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D0 = p->S0022;
	}
#else
	D0 = A6->S0022;
#endif
	c0 = D0 & (1<<7); D0 &= ~(1<<7);
	if ( c0 ) goto L000e0e;
	D0 = Volume[D0];

L000e0e:;
/*
														add.b   L001e14(pc),d0
														bcs     L000e16
														bpl     L000e18
*/
	D0 += G.L001e14;
	if ( D0 > 0xff ) goto L000e16;
	if ( (SBYTE)D0 >= 0 ) goto L000e18;

L000e16:;
/*
														moveq.l #$7f,d0
*/
	D0 = 0x7f;

L000e18:;
/*
														add.b   $004a(a6),d0	; word
														bcs     L000e20
														bpl     L000e22
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D0 += p->S004a>>8;
	}
#else
	D0 += A6->S004a>>8;
#endif
	if ( D0 > 0xff ) goto L000e20;
	if ( (SBYTE)D0 >= 0 ) goto L000e22;

L000e20:;
/*
														moveq.l #$7f,d0
*/
	D0 = 0x7f;

L000e22:;
/*
														cmp.b   $0023(a6),d0
														beq     L000e54
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( p->S0023 == D0 ) {
			return;
		}
	}
#else
	if ( A6->S0023 == D0 ) {
		return;
	}
#endif

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L000e28( context );
#else
	L000e28();
#endif
}

static void L000e28(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	UBYTE c0;

//L000e28:;
/*
														move.b  d0,$0023(a6)
														movea.l $0004(a6),a0
														addq.w  #6,a0
														move.b  $0019(a6),d3
														moveq.l #$60,d1
														add.b   $0018(a6),d1
														moveq.l #$03,d4
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0023 = (UBYTE)D0;
		A0 = p->S0004;
		if ( A0 == 0 ) A0 = TO_OFS(&FAKEA6S0004[0]);
		A0 += 6;
		D3 = p->S0019;
		D1 = 0x60;
		D1 += p->S0018;
		D4 = 0x03;
	}
#else
	A6->S0023 = D0;
	A0 = A6->S0004;
	if ( A0 == NULL ) A0 = &FAKEA6S0004[0];
	A0 += 6;
	D3 = A6->S0019;
	D1 = 0x60;
	D1 += A6->S0018;
	D4 = 0x03;
#endif

L000e3e:;
/*
														move.b  (a0)+,d2
														lsr.b   #1,d3
														bcc     L000e4e
														add.b   d0,d2
														bpl     L000e4a
														moveq.l #$7f,d2
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D2 = *TO_PTR(A0++);
#else
	D2 = *(A0++);
#endif
	c0 = D3&1; D3 >>= 1;
	if ( !c0 ) goto L000e4e;
	D2 += D0;
	if ( (SBYTE)D2 >= 0 ) goto L000e4a;
	D2 = 0x7f;

L000e4a:;
/*
														bsr     L_WRITEOPM
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
#endif

L000e4e:;
/*
														addq.b  #8,d1
														dbra    d4,L000e3e
*/
	D1 += 8;
	if ( D4-- != 0 ) goto L000e3e;

//L000e54:;
/*
														rts
*/
}


/***************************************************************/

static void L000e66(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	UBYTE c0;

//L000e66:;
/*
														bclr.b  #$02,$0017(a6)
														beq     L000e7c
														move.b  $001c(a6),d2
														moveq.l #$20,d1
														add.b   $0018(a6),d1
														bra     L_WRITEOPM
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		c0 = p->S0017 & (1<<2); p->S0017 &= ~(1<<2);
		if ( !c0 ) goto L000e7c;
		D2 = p->S001c;
		D1 = 0x20;
		D1 += p->S0018;
	}
	L_WRITEOPM( context ); return;
#else
	c0 = A6->S0017 & (1<<2); A6->S0017 &= ~(1<<2);
	if ( !c0 ) goto L000e7c;
	D2 = A6->S001c;
	D1 = 0x20;
	D1 += A6->S0018;
	L_WRITEOPM(); return;
#endif

L000e7c:;
/*
														rts
*/
}


/***************************************************************/


static void L000e7e(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	UBYTE c0;

/*
L000fba:;
														.dc.b   $0f,$0f,$0f,$0e,$0e,$0e,$0d,$0d
														.dc.b   $0d,$0c,$0c,$0b,$0b,$0b,$0a,$0a
														.dc.b   $0a,$09,$09,$08,$08,$08,$07,$07
														.dc.b   $07,$06,$06,$05,$05,$05,$04,$04
														.dc.b   $04,$03,$03,$02,$02,$02,$01,$01
														.dc.b   $01,$00,$00,$ff
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	/* MxdrvContextImpl 構造体上に移動 */
#else
	static UBYTE Volume[] = {
		0x2a,0x28,0x25,0x22,0x20,0x1d,0x1a,0x18,
		0x15,0x12,0x10,0x0d,0x0a,0x08,0x05,0x02,
	};
#endif
	static UBYTE PCMVolume[] = {
		0x0f,0x0f,0x0f,0x0e,0x0e,0x0e,0x0d,0x0d,
		0x0d,0x0c,0x0c,0x0b,0x0b,0x0b,0x0a,0x0a,
		0x0a,0x09,0x09,0x08,0x08,0x08,0x07,0x07,
		0x07,0x06,0x06,0x05,0x05,0x05,0x04,0x04,
		0x04,0x03,0x03,0x02,0x02,0x02,0x01,0x01,
		0x01,0x00,0x00,0xff,
	};

//L000e7e:;
/*
														bset.b  #$03,$0016(a6)
														bne     L000e7c
														btst.b  #$04,$0016(a6)
														beq     L000e92
														bsr     L000ff6
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		c0 = p->S0016&(1<<3); p->S0016 |= (1<<3);
		if ( c0 ) return;
		if ( !(p->S0016&(1<<4)) ) goto L000e92;
	}
	L000ff6( context );
#else
	c0 = A6->S0016&(1<<3); A6->S0016 |= (1<<3);
	if ( c0 ) return;
	if ( !(A6->S0016&(1<<4)) ) goto L000e92;
	L000ff6();
#endif


L000e92:;
/*
														tst.b   $0018(a6)
														bmi     L000eb2
														move.b  $001d(a6),d2
														lea.l   L00223c(pc),a2
														move.b  d2,$00(a2,d7.w)
														lea.l   L001bb4(pc),a2
														move.b  d2,$00(a2,d7.w)
														moveq.l #$08,d1
														bra     L_WRITEOPM
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( (SBYTE)p->S0018 < 0 ) goto L000eb2;
		D2 = p->S001d;
		A2 = TO_OFS(&G.L00223c[0]);
		TO_PTR(A2)[D7] = (UBYTE)D2;
		A2 = TO_OFS(&G.L001bb4[0]);
		TO_PTR(A2)[D7] = (UBYTE)D2;
	}
	D1 = 0x08;
	L_WRITEOPM( context ); return;
#else
	if ( (SBYTE)A6->S0018 < 0 ) goto L000eb2;
	D2 = A6->S001d;
	A2 = &G.L00223c[0];
	A2[D7] = D2;
	A2 = &G.L001bb4[0];
	A2[D7] = D2;
	D1 = 0x08;
	L_WRITEOPM(); return;
#endif

L000eb2:;
/*
														move.b  (L002231),d0
														beq     L000f26
														tst.b   (L001e09)
														bne     L000f26
														moveq.l #$00,d0
														move.w  $0012(a6),d0
														lsr.w   #6,d0
														move.b  $001c(a6),d2
														move.b  d2,d1
														and.w   #$0003,d1
														beq     L000ed8
														cmp.w   #$0003,d1
														bne     L000edc
*/
	D0 = G.L002231;
	if ( D0 == 0 ) goto L000f26;
	if ( G.L001e09 != 0 ) goto L000f26;
	D0 = 0x00;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D0 = p->S0012;
		D0 >>= 6;
		D2 = p->S001c;
	}
#else
	D0 = A6->S0012;
	D0 >>= 6;
	D2 = A6->S001c;
#endif
	D1 = D2;
	D1 &= 0x0003;
	if ( D1 == 0 ) goto L000ed8;
	if ( D1 != 0x0003 ) goto L000edc;

L000ed8:;
/*
														eori.w  #$0003,d1
*/
	D1 ^= 0x0003;

L000edc:;
/*
														and.w   #$001c,d2
														lsl.w   #6,d2
														or.w    d1,d2
														tst.b   (L001df4)
														bne     L000f28
														tst.b   (L001e15)
														beq     L000ef4
														andi.b  #$fc,d2
*/
	D2 &= 0x001c;
	D2 <<= 6;
	D2 |= D1;
	if ( G.L001df4 ) goto L000f28;
	if ( G.L001e15 == 0 ) goto L000ef4;
	D2 &= 0xfc;

L000ef4:;
/*
														lsl.w   #3,d0
														movea.l (L00222c),a1
														lea.l   $00(a1,d0.w),a0
														adda.l  (a0)+,a1
														addq.w  #2,a0
														move.w  (a0)+,d3
														beq     L000f26
														moveq.l #$67,d0
														moveq.l #$00,d1
														trap    #15
														moveq.l #$60,d0
														move.w  d2,d1
														moveq.l #$00,d2
														move.w  d3,d2
														trap    #15
														lea.l   L00223c(pc),a2
														clr.b   $00(a2,d7.w)
														lea.l   L001bb4(pc),a2
														clr.b   $00(a2,d7.w)
*/
	D0 <<= 3;
	A1 = G.L00222c;
	A0 = A1+D0;
#if MXDRV_ENABLE_PORTABLE_CODE
	/*
		COSMIC_.MDX にてシーケンスログに 0x80 が出現する。
		このとき p->S0012 に 5 が格納され、ここで NULL ポインタアクセスになる。
		対症療法で回避。
	*/
	if (A0 == 0) {
		A0 += 8;
		D3 = 0;
	} else {
		A1 += GETBLONG(A0); A0 += 4;
		A0 += 2;
		D3 = GETBWORD(A0); A0 += 2;
	}
#else
	A1 += GETBLONG(A0); A0 += 4;
	A0 += 2;
	D3 = GETBWORD(A0); A0 += 2;
#endif
	if ( D3 == 0x0000 ) goto L000f26;
#if MXDRV_ENABLE_PORTABLE_CODE
	ADPCMMOD_END( context );
#else
	ADPCMMOD_END();
#endif
	D1 = D2;
	D2 = D3;
	if ( D2 > 0xff00 ) D2 = 0xff00;  /* DMAサイズ制限 */
#if MXDRV_ENABLE_PORTABLE_CODE
	ADPCMOUT( context );
	A2 = TO_OFS(&G.L00223c[0]);
	TO_PTR(A2)[D7] = CLR;
	A2 = TO_OFS(&G.L001bb4[0]);
	TO_PTR(A2)[D7] = CLR;
#else
	ADPCMOUT();
	A2 = &G.L00223c[0];
	A2[D7] = CLR;
	A2 = &G.L001bb4[0];
	A2[D7] = CLR;
#endif

L000f26:;
/*
														rts
*/
	return;

L000f28:;
/*
														moveq.l #$00,d1
														move.b  $0004_b(a6),d1
														lsl.l   #5,d1
														add.l   d1,d0
														add.l   d1,d1
														add.l   d1,d0
														lsl.l   #3,d0
														movea.l (L00222c),a1
														lea.l   $00(a1,d0.l),a0
														move.l  $0004(a0),d3
														beq     L000f26
														adda.l  (a0),a1
														lea.l   L002248(pc),a0	; 不要
														cmpa.l  a0,a1			; 不要
														bcs     L000fb8			; 不要
														move.l  a1,d0			; 不要
														add.l   d3,d0			; 不要
														bcs     L000fb8			; 不要
														cmp.l   (L001bb0),d0	; 不要
														bcc     L000fb8			; 不要
														move.b  $0018(a6),d0
														and.w   #$0007,d0
														moveq.l #$00,d1
														move.b  $0022(a6),d1
														bclr.l  #$07,d1
														bne     L000f78
														lea.l   L000e56(pc),a2
														move.b  $00(a2,d1.w),d1
*/
	D1 = 0x00;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D1 = p->S0004_b;
	}
#else
	D1 = A6->S0004_b;
#endif
	D1 <<= 0x05;
	D0 += D1;
	D1 += D1;
	D0 += D1;
	D0 <<= 3;
	A1 = G.L00222c;
	A0 = A1+D0;
	D3 = GETBLONG( A0+4 );
	if ( D3 == 0 ) goto L000f26;
	A1 += GETBLONG( A0 );
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D0 = p->S0018;
		D0 &= 0x0007;
		D1 = 0x00;
		D1 = p->S0022;
	}
#else
	D0 = A6->S0018;
	D0 &= 0x0007;
	D1 = 0x00;
	D1 = A6->S0022;
#endif
	c0 = D1&(1<<7); D1 &= ~(1<<7);
	if ( c0 ) goto L000f78;
#if MXDRV_ENABLE_PORTABLE_CODE
	A2 = TO_OFS(&L000e7eVolume[0]);
	D1 = TO_PTR(A2)[D1];
#else
	A2 = &Volume[0];
	D1 = A2[D1];
#endif

L000f78:;
/*
														add.b   L001e14(pc),d1
														bmi     L000f84
														cmp.b   #$2b,d1                 ;'+'
														bcs     L000f8a
*/
	D1 += G.L001e14;
	if ( (SBYTE)D1 < 0 ) goto L000f84;
	if ( D1 < 0x2b ) goto L000f8a;

L000f84:;
/*
														moveq.l #$00,d1
														clr.b   d2
														bra     L000f8e
*/
	D1 = 0x00;
	D2 &= 0xffffff00;
	goto L000f8e;

L000f8a:;
/*
														move.b  L000fba(pc,d1.w),d1
*/
	D1 = PCMVolume[D1];

L000f8e:;
/*
														swap.w  d1
														move.w  d2,d1
														moveq.l #$00,d2
														trap    #2
														move.b  $0018(a6),d0
														and.w   #$0007,d0
														move.l  d3,d2
														andi.l  #$00ffffff,d2
														trap    #2
														lea.l   L00223c(pc),a2
														clr.b   $0008(a2)
														lea.l   L001bb4(pc),a2
														clr.b   $00(a2,d7.w)
*/
	D1 <<= 16;
	D1 |= (D2&0xffff);
	D2 = 0x00;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D0 = p->S0018;
	}
#else
	PCM8_SUB();
	D0 = A6->S0018;
#endif
	D0 &= 0x07;
	D2 = D3;
	D2 &= 0xffffff;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
	A2 = TO_OFS(&G.L00223c[0]);
	TO_PTR(A2)[0x0008] = CLR;
	A2 = TO_OFS(&G.L001bb4[0]);
	TO_PTR(A2)[D7] = CLR;
#else
	PCM8_SUB();
	A2 = &G.L00223c[0];
	A2[0x0008] = CLR;
	A2 = &G.L001bb4[0];
	A2[D7] = CLR;
#endif

//L000fb8:;
/*
														rts
*/
}


/***************************************************************/

static void L000fe6(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	UBYTE c0;

//L000fe6:;
/*
														bclr.b  #$03,$0016(a6)
														beq     L00103a
														btst.b  #$04,$0016(a6)
														bne     L00103a
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		c0 = p->S0016 & (1<<3); p->S0016 &= ~(1<<3);
		if ( c0 == 0 ) return;
		if ( p->S0016 & (1<<4) ) return;
	}
#else
	c0 = A6->S0016 & (1<<3); A6->S0016 &= ~(1<<3);
	if ( c0 == 0 ) return;
	if ( A6->S0016 & (1<<4) ) return;
#endif

/*
														; fall down;
*/

#if MXDRV_ENABLE_PORTABLE_CODE
	L000ff6( context ); return;
#else
	L000ff6(); return;
#endif

}

static void L000ff6(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L000ff6:;
/*
														move.b  $0018(a6),d2
														bmi     L001012
														moveq.l #$08,d1
														lea.l   L00223c(pc),a2
														move.b  d2,$00(a2,d7.w)
														lea.l   L001bb4(pc),a2
														move.b  d2,$00(a2,d7.w)
														bra     L_WRITEOPM
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D2 = p->S0018;
		if ( (SBYTE)D2 < 0 ) goto L001012;
		D1 = 0x08;
		A2 = TO_OFS(&G.L00223c[0]);
		TO_PTR(A2)[D7] = (UBYTE)D2;
		A2 = TO_OFS(&G.L001bb4[0]);
		TO_PTR(A2)[D7] = (UBYTE)D2;
	}
	L_WRITEOPM( context ); return;
#else
	D2 = A6->S0018;
	if ( (SBYTE)D2 < 0 ) goto L001012;
	D1 = 0x08;
	A2 = &G.L00223c[0];
	A2[D7] = D2;
	A2 = &G.L001bb4[0];
	A2[D7] = D2;
	L_WRITEOPM(); return;
#endif

L001012:;
/*
														move.b  (L002231),d0
														beq     L00103a
														tst.b   (L001e09)
														bne     L00103a
														tst.b   (L001df4)
														beq     L00103c
														move.b  $0018(a6),d0
														and.w   #$0007,d0
														moveq.l #$00,d1
														move.b  $0022(a6),d1
														swap.w  d1
														move.w  d2,d1
														moveq.l #$00,d2
														trap    #2
*/
	D0 = G.L002231;
	if ( D0 == 0 ) goto L00103a;
	if ( G.L001e09 ) goto L00103a;
	if ( G.L001df4 == 0 ) goto L00103c;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D0 = p->S0018;
		D0 &= 0x0007;
		D1 = 0x00;
		D1 = p->S0022;
	}
#else
	D0 = A6->S0018;
	D0 &= 0x0007;
	D1 = 0x00;
	D1 = A6->S0022;
#endif
	D1 <<= 16;
	D1 |= ( D2&0xffff );
	D2 = 0;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif

L00103a:;
/*
														rts
*/
	return;

L00103c:;
/*
														tst.b   $0017(a6)
														bne     L001048
														moveq.l #$67,d0
														moveq.l #$01,d1
														trap    #15
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( p->S0017 ) goto L001048;
	}
	ADPCMMOD_STOP( context );
#else
	if ( A6->S0017 ) goto L001048;
	ADPCMMOD_STOP();
#endif

L001048:;
/*
														moveq.l #$67,d0
														moveq.l #$00,d1
														trap    #15
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	ADPCMMOD_END( context );
#else
	ADPCMMOD_END();
#endif

}

/***************************************************************/

static void L001050(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001050:;
/*
														tst.b   $0018(a6)
														bmi     L001092
														tst.b   $0016(a6)
														bpl     L00106a
														tst.b   $0020(a6)
														bne     L00106a
														move.l  $0008(a6),d0
														add.l   d0,$000c(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( (SBYTE)p->S0018 < 0 ) goto L001092;
		if ( (SBYTE)p->S0016 >= 0 ) goto L00106a;
		if ( p->S0020 ) goto L00106a;
		D0 = p->S0008;
		p->S000c += D0;
	}
#else
	if ( (SBYTE)A6->S0018 < 0 ) goto L001092;
	if ( (SBYTE)A6->S0016 >= 0 ) goto L00106a;
	if ( A6->S0020 ) goto L00106a;
	D0 = A6->S0008;
	A6->S000c += D0;
#endif

L00106a:;
/*
														tst.b   $0024(a6)
														beq     L00107c
														tst.b   $0020(a6)
														bne     L001092
														tst.b   $0025(a6)
														bne     L001094
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( p->S0024 == 0 ) goto L00107c;
		if ( p->S0020 ) goto L001092;
		if ( p->S0025 ) { L001094( context ); return; }
	}
#else
	if ( A6->S0024 == 0 ) goto L00107c;
	if ( A6->S0020 ) goto L001092;
	if ( A6->S0025 ) { L001094(); return; }
#endif

L00107c:;
/*
														btst.b  #$05,$0016(a6)
														beq     L001086
														bsr     L0010b4
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( !(p->S0016 & (1<<5)) ) goto L001086;
	}
	L0010b4( context );
#else
	if ( !(A6->S0016 & (1<<5)) ) goto L001086;
	L0010b4();
#endif

L001086:;
/*
														btst.b  #$06,$0016(a6)
														beq     L001092
														bsr     L001116
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( !(p->S0016 & (1<<6)) ) goto L001092;
	}
	L001116( context );
#else
	if ( !(A6->S0016 & (1<<6)) ) goto L001092;
	L001116();
#endif


L001092:;
/*
														rts

*/
	return;
}

static void L001094(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001094:;
/*
														subq.b  #1,$0025(a6)
														bne     L0010b2
														btst.b  #$05,$0016(a6)
														beq     L0010a6
														bsr     L001562
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0025--;
		if ( p->S0025 ) goto L0010b2;
		if ( !(p->S0016 & (1<<5)) ) goto L0010a6;
	}
#else
	A6->S0025--;
	if ( A6->S0025 ) goto L0010b2;
	if ( !(A6->S0016 & (1<<5)) ) goto L0010a6;
#endif
//	L001562();

//L001562:;
/*
														move.w  $003a(a6),$003e(a6)
														move.l  $002e(a6),$0032(a6)
														move.l  $002a(a6),$0036(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S003e = p->S003a;
		p->S0032 = p->S002e;
		p->S0036 = p->S002a;
	}
#else
	A6->S003e = A6->S003a;
	A6->S0032 = A6->S002e;
	A6->S0036 = A6->S002a;
#endif

L0010a6:;
/*
														btst.b  #$06,$0016(a6)
														beq     L0010b2
														bsr     L0015d0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( !(p->S0016 & (1<<6)) ) goto L0010b2;
	}
	L0015d0( context );
#else
	if ( !(A6->S0016 & (1<<6)) ) goto L0010b2;
	L0015d0();
#endif


L0010b2:;
/*
														rts
*/
	return;

}

/***************************************************************/

static void L0010b4(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	static void (*Table[])( MxdrvContext * ) = {
#else
	static void (*Table[])(void) = {
#endif
		L00095a,
		L0010be,
		L0010d4,
		L0010ea,
		L001100,
	};


//L0010b4:;
/*
														move.l  $0032(a6),d1
														movea.l $0026(a6),a0
														jmp     (a0)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D1 = p->S0032;
		A0 = p->S0026;
	}
#else
	D1 = A6->S0032;
	A0 = A6->S0026;
#endif
	if ( (ULONG)A0 < 0x05 ) {
#if MXDRV_ENABLE_PORTABLE_CODE
		Table[(ULONG)A0]( context );
#else
		Table[(ULONG)A0]();
#endif
	}
#if MXDRV_ENABLE_PORTABLE_CODE
	MX_ABORT( context );
#else
	MX_ABORT();
#endif

}

/***************************************************************/

static void L0010be(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0010be:;
/*
														add.l   d1,$0036(a6)
														subq.w  #1,$003e(a6)
														bne     L0010d2
														move.w  $003c(a6),$003e(a6)
														neg.l   $0036(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0036 += D1;
		p->S003e--;
		if ( p->S003e ) goto L0010d2;
		p->S003e = p->S003c;
		p->S0036 = -(SLONG)(p->S0036);
	}
#else
	A6->S0036 += D1;
	A6->S003e--;
	if ( A6->S003e ) goto L0010d2;
	A6->S003e = A6->S003c;
	A6->S0036 = -A6->S0036;
#endif

L0010d2:;
/*
														rts
*/


}

/***************************************************************/

static void L0010d4(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
//L0010d4:;
/*
														move.l  d1,$0036(a6)
														subq.w  #1,$003e(a6)
														bne     L0010e8
														move.w  $003c(a6),$003e(a6)
														neg.l   $0032(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0036 = D1;
		p->S003e--;
		if ( p->S003e ) goto L0010e8;
		p->S003e = p->S003c;
		p->S0032 = -(SLONG)(p->S0032);
	}
#else
	A6->S0036 = D1;
	A6->S003e--;
	if ( A6->S003e ) goto L0010e8;
	A6->S003e = A6->S003c;
	A6->S0032 = -A6->S0032;
#endif

L0010e8:;
/*
														rts
*/

}

/***************************************************************/

static void L0010ea(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0010ea:;
/*
														add.l   d1,$0036(a6)
														subq.w  #1,$003e(a6)
														bne     L0010fe
														move.w  $003c(a6),$003e(a6)
														neg.l   $0032(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0036 += D1;
		p->S003e--;
		if ( p->S003e ) goto L0010fe;
		p->S003e = p->S003c;
		p->S0032 = -(SLONG)(p->S0032);
	}
#else
	A6->S0036 += D1;
	A6->S003e--;
	if ( A6->S003e ) goto L0010fe;
	A6->S003e = A6->S003c;
	A6->S0032 = -A6->S0032;
#endif

L0010fe:;
/*
														rts
*/

}

/***************************************************************/

static void L001100(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001100:;
/*
														subq.w  #1,$003e(a6)
														bne     L001114
														bsr     L00117a
														muls.w  d1,d0
														move.l  d0,$0036(a6)
														move.w  $003c(a6),$003e(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S003e--;
		if ( p->S003e ) goto L001114;
		L00117a( context );
		D0 = (SWORD)D0 * (SWORD)D1;
		p->S0036 = D0;
		p->S003e = p->S003c;
	}
#else
	A6->S003e--;
	if ( A6->S003e ) goto L001114;
	L00117a();
	D0 = (SWORD)D0 * (SWORD)D1;
	A6->S0036 = D0;
	A6->S003e = A6->S003c;
#endif

L001114:;
/*
														rts
*/

}

/***************************************************************/

static void L001116(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	static void (*Table[])( MxdrvContext * ) = {
#else
	static void (*Table[])(void) = {
#endif
		L00095a,
		L001120,
		L001138,
		L00114e,
		L001164,
	};

//L001116:;
/*
														move.w  $0048(a6),d1
														movea.l $0040(a6),a0
														jmp     (a0)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D1 = p->S0048;
		A0 = p->S0040;
	}
	if ( (ULONG)A0 < 0x05 ) {
		Table[(ULONG)A0]( context );
	}
	MX_ABORT( context );
#else
	D1 = A6->S0048;
	A0 = A6->S0040;
	if ( (ULONG)A0 < 0x05 ) {
		Table[(ULONG)A0]();
	}
	MX_ABORT();
#endif
}

/***************************************************************/

static void L001120(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001120:;
/*
														add.w   d1,$004a(a6)
														subq.w  #1,$004e(a6)
														bne     L001136
														move.w  $004c(a6),$004e(a6)
														move.w  $0046(a6),$004a(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S004a += (UWORD)D1;
		p->S004e--;
		if ( p->S004e ) goto L001136;
		p->S004e = p->S004c;
		p->S004a = p->S0046;
	}
#else
	A6->S004a += D1;
	A6->S004e--;
	if ( A6->S004e ) goto L001136;
	A6->S004e = A6->S004c;
	A6->S004a = A6->S0046;
#endif

L001136:;
/*
														rts
*/

}


/***************************************************************/

static void L001138(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001138:;
/*
														subq.w  #1,$004e(a6)
														bne     L00114c
														move.w  $004c(a6),$004e(a6)
														add.w   d1,$004a(a6)
														neg.w   $0048(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S004e--;
		if ( p->S004e ) goto L00114c;
		p->S004e = p->S004c;
		p->S004a += (UWORD)D1;
		p->S0048 = -p->S0048;
	}
#else
	A6->S004e--;
	if ( A6->S004e ) goto L00114c;
	A6->S004e = A6->S004c;
	A6->S004a += D1;
	A6->S0048 = -A6->S0048;
#endif

L00114c:;
/*
														rts
*/

}

/***************************************************************/

static void L00114e(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L00114e:;
/*
														add.w   d1,$004a(a6)
														subq.w  #1,$004e(a6)
														bne     L001162
														move.w  $004c(a6),$004e(a6)
														neg.w   $0048(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S004a += (UWORD)D1;
		p->S004e--;
		if ( p->S004e ) goto L001162;
		p->S004e = p->S004c;
		p->S0048 = -p->S0048;
	}
#else
	A6->S004a += D1;
	A6->S004e--;
	if ( A6->S004e ) goto L001162;
	A6->S004e = A6->S004c;
	A6->S0048 = -A6->S0048;
#endif

L001162:;
/*
														rts
*/

}

/***************************************************************/

static void L001164(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001164:;
/*
														subq.w  #1,$004e(a6)
														bne     L001178
														bsr     L00117a
														muls.w  d0,d1
														move.w  $004c(a6),$004e(a6)
														move.w  d1,$004a(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S004e--;
		if ( p->S004e ) goto L001178;
		L00117a( context );
		D1 = (SWORD)D1 * (SWORD)D0;
		p->S004e = p->S004c;
		p->S004a = (UWORD)D1;
	}
#else
	A6->S004e--;
	if ( A6->S004e ) goto L001178;
	L00117a();
	D1 = (SWORD)D1 * (SWORD)D0;
	A6->S004e = A6->S004c;
	A6->S004a = D1;
#endif

L001178:;
/*
														rts
*/

}

/***************************************************************/

static void L00117a(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
/*
L001190:;
														.dc.w   $1234
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	/* MxdrvContextImpl 構造体上に移動 */
#else
	static UWORD L001190 = 0x1234;
#endif

//L00117a:;
/*
														move.w  (L001190),d0
														mulu.w  #$c549,d0
														add.l   #$0000000c,d0
														move.w  d0,(L001190)
														lsr.l   #8,d0
														rts
*/
	D0 = L001190;
	D0 *= 0xc549;
	D0 += 0x0c;
#if MXDRV_ENABLE_PORTABLE_CODE
	L001190 = (UWORD)D0;
#else
	L001190 = D0;
#endif
	D0 >>= 8;

}

/***************************************************************/

static void L001192(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001192:;
/*
														lea.l   L001df6(pc),a0
														tst.b   $00(a0,d7.w)
														bne     L00119e
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = TO_OFS(&G.L001df6[0]);
	if ( TO_PTR(A0)[D7] ) goto L00119e;
#else
	A0 = &G.L001df6[0];
	if ( A0[D7] ) goto L00119e;
#endif
	return;

L00119e:;
/*
														clr.b   $00(a0,d7.w)
														cmp.w   #$0009,d7
														bcc     L0011ac
														clr.b   $27(a5,d7.w)	; L002233(d7.w)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	TO_PTR(A0)[D7] = CLR;
#else
	A0[D7] = CLR;
#endif
	if ( D7 >= 0x09 ) goto L0011ac;
	G.L002233[D7] = CLR;

L0011ac:;
/*
														andi.b  #$f7,$0017(a6)
														bra     L0011d4
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0017 &= 0xf7;
	}
	L0011d4( context ); return;
#else
	A6->S0017 &= 0xf7;
	L0011d4(); return;
#endif

}

/***************************************************************/

static void L0011b4(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0011b4:;
/*
														btst.b  #$03,$0017(a6)
														bne     L001192
														btst.b  #$02,$0016(a6)
														bne     L0011ce
														subq.b  #1,$001b(a6)
														bne     L0011ce
														bsr     L000fe6
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( p->S0017 & (1<<3) ) { L001192( context ); return; }
		if ( p->S0016 & (1<<2) ) { L0011ce( context ); return; }
		p->S001b--;
		if ( p->S001b ) { L0011ce( context ); return; }
	}
	L000fe6( context );
	L0011ce( context ); return;
#else
	if ( A6->S0017 & (1<<3) ) { L001192(); return; }
	if ( A6->S0016 & (1<<2) ) { L0011ce(); return; }
	A6->S001b--;
	if ( A6->S001b ) { L0011ce(); return; }
	L000fe6();
	L0011ce(); return;
#endif

}

/***************************************************************/

static void L0011ce(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0011ce:;
/*
														subq.b  #1,$001a(a6)
														bne     L001224
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S001a--;
		if ( p->S001a ) return;
	}
	L0011d4( context ); return;
#else
	A6->S001a--;
	if ( A6->S001a ) return;
	L0011d4(); return;
#endif
}

/***************************************************************/

/*
L001252:;
		.dc.w   L001292-L001252
		.dc.w   L0012a6-L001252
		.dc.w   L0012be-L001252
		.dc.w   L0012e6-L001252
		.dc.w   L00131c-L001252
		.dc.w   L001328-L001252
		.dc.w   L001344-L001252
		.dc.w   L001364-L001252
		.dc.w   L00136a-L001252
		.dc.w   L001372-L001252
		.dc.w   L001376-L001252
		.dc.w   L00139a-L001252
		.dc.w   L0013ba-L001252
		.dc.w   L0013c6-L001252
		.dc.w   L0013dc-L001252
		.dc.w   L001492-L001252
		.dc.w   L001498-L001252
		.dc.w   L0014b0-L001252
		.dc.w   L0014dc-L001252
		.dc.w   L0014fc-L001252
		.dc.w   L001590-L001252
		.dc.w   L0015fe-L001252
		.dc.w   L001656-L001252
		.dc.w   L00165c-L001252
		.dc.w   L001694-L001252
		.dc.w   L001442-L001252
		.dc.w   L001442-L001252
		.dc.w   L001442-L001252
		.dc.w   L001442-L001252
		.dc.w   L001442-L001252
		.dc.w   L001442-L001252
		.dc.w   L001442-L001252
*/

#if MXDRV_ENABLE_PORTABLE_CODE
static void (*L001252[])( MxdrvContext * ) = {
#else
static void (*L001252[])(void) = {
#endif
	L001292,	// @@ @t
	L0012a6,
	L0012be,	// @@ @
	L0012e6,	// @@ p
	L00131c,	// @@ v
	L001328,
	L001344,
	L001364,
	L00136a,
	L001372,
	L001376,
	L00139a,
	L0013ba,	// @@ D
	L0013c6,
	L0013dc,
	L001492,
	L001498,
	L0014b0,
	L0014dc,
	L0014fc,
	L001590,
	L0015fe,
	L001656,
	L00165c,
	L001694,
	L001442,
	L001442,
	L001442,
	L001442,
	L001442,
	L001442,
	L001442,
};

/***************************************************************/

static void L0011d4(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0011d4:;
/*
														movea.l (a6),a4
														andi.b  #$7b,$0016(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		A4 = p->S0000;
		p->S0016 &= 0x7b;
	}
#else
	A4 = A6->S0000;
	A6->S0016 &= 0x7b;
#endif

L0011dc:;
/*
														moveq.l #$00,d0
														moveq.l #$00,d1
														move.b  (a4)+,d0
														move.b  d0,d1
														bpl     L001216
														cmp.b   #$e0,d0
														bcc     L00122e
														and.w   #$007f,d0
														lsl.w   #6,d0
														addq.w  #5,d0
														add.w   $0010(a6),d0
														move.w  d0,$0012(a6)
														ori.b   #$01,$0016(a6)
														move.b  $001f(a6),$0020(a6)
														moveq.l #$00,d0
														move.b  (a4)+,d0
														move.b  $001e(a6),d1
														bmi     L001226
														mulu.w  d0,d1
														lsr.w   #3,d1
*/

#if LOGSEQ
	FILE *fout;
	fout = fopen( "c:\\temp\\seq.log", "ab+" );
	fprintf( fout, "%2d %08lX %02X\n", A6->S0018, (DWORD)A4, *(A4) );
	fclose( fout );
#endif

	D0 = 0x00;
	D1 = 0x00;
#if MXDRV_ENABLE_PORTABLE_CODE
	D0 = *TO_PTR(A4++);
#else
	D0 = *(A4++);
#endif
	D1 = D0;
	if ( (SBYTE)D1 >= 0 ) goto L001216;
	if ( D0 >= 0xe0 ) goto L00122e;
	D0 &= 0x007f;
	D0 <<= 6;
	D0 += 5;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D0 += p->S0010;
		p->S0012 = (UWORD)D0;
		p->S0016 |= 0x01;
		p->S0020 = p->S001f;
		D0 = 0x00;
		D0 = *TO_PTR(A4++);
		D1 = p->S001e;
	}
#else
	D0 += A6->S0010;
	A6->S0012 = D0;
	A6->S0016 |= 0x01;
	A6->S0020 = A6->S001f;
	D0 = 0x00;
	D0 = *(A4++);
	D1 = A6->S001e;
#endif
	if ( (SBYTE)D1 < 0 ) goto L001226;
	D1 *= D0;
	D1 = (D1&0xffff)>>3;

L001216:;
/*
														addq.w  #1,d1
														move.b  d1,$001b(a6)
														addq.w  #1,d0
														move.b  d0,$001a(a6)
														move.l  a4,(a6)
*/
	D1++;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S001b = (UBYTE)D1;
		D0++;
		p->S001a = (UBYTE)D0;
		p->S0000 = A4;
	}
#else
	A6->S001b = D1;
	D0++;
	A6->S001a = D0;
	A6->S0000 = A4;
#endif

//L001224:;
/*
														rts
*/
	return;

L001226:;
/*
														add.b   d0,d1
														bcs     L001216
														moveq.l #$00,d1
														bra     L001216
*/
	D1 &= 0xff; D1 += (D0&0xff);
	if ( D1 >= 0x100 ) goto L001216;
	D1 = 0x00;
	goto L001216;

L00122e:;
/*
														ext.w   d0
														not.w   d0
														add.w   d0,d0
														move.w  L001252(pc,d0.w),d0
														pea.l   L0011dc(pc)
														jmp     L001252(pc,d0.w)
*/
	D0 ^= 0xff;
	DisposeStack_L00122e = FALSE;
#if MXDRV_ENABLE_PORTABLE_CODE
	L001252[D0]( context );
#else
	L001252[D0]();
#endif
	if ( DisposeStack_L00122e ) return;
	goto L0011dc;
}

/***************************************************************/

#if 0 /* 本体側へ移動 */
static void L001240(
	void
) {

L001240:;
/*
														ext.w   d0
														not.w   d0
														add.w   d0,d0
														move.w  L001252(pc,d0.w),d0
														pea.l   L00177a(pc)
														jmp     L001252(pc,d0.w)
*/
	D0 ^= 0xff;
	DisposeStack_L00122e = FALSE;
	L001252[D0]();
	if ( DisposeStack_L00122e ) return;
	L00177a();
}
#endif

/***************************************************************/

static void L001292(									// @@ @t
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001292:;
/*
														moveq.l #$12,d1
														move.b  (a4)+,d2
														move.b  d2,(L001e0c)
														tst.b   (L001e08)
														bne     L0012a4
														bra     L_WRITEOPM
*/
	D1 = 0x12;
#if MXDRV_ENABLE_PORTABLE_CODE
	D2 = *TO_PTR(A4++);
	G.L001e0c = (uint8_t)D2;
	G.MUSICTIMER = (uint8_t)D2;
#else
	D2 = *(A4++);
	G.L001e0c = D2;
	G.MUSICTIMER = D2;
#endif
	if ( G.L001e08 ) goto L0012a4;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
#else
	L_WRITEOPM();
#endif

L0012a4:;
/*
														rts
*/

}


/***************************************************************/

static void L0012a6(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0012a6:;
/*
														move.b  (a4)+,d1
														move.b  (a4)+,d2
														cmp.b   #$12,d1
														bne     L0012ba
														tst.b   (L001e08)
														bne     L0012a4	; rts
														move.b  d2,(L001e0c)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D1 = *TO_PTR(A4++);
	D2 = *TO_PTR(A4++);
#else
	D1 = *(A4++);
	D2 = *(A4++);
#endif
	if ( D1 != 0x12 ) goto L0012ba;
	if ( G.L001e08 ) return;
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e0c = (uint8_t)D2;
	G.MUSICTIMER = (uint8_t)D2;
#else
	G.L001e0c = D2;
	G.MUSICTIMER = D2;
#endif

L0012ba:;
/*
														bra     L_WRITEOPM
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context ); return;
#else
	L_WRITEOPM(); return;
#endif

}

/***************************************************************/

static void L0012be(									// @@ @
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0012be:;
/*
														tst.b   $0018(a6)
														bmi     L0012e0
														move.b  (a4)+,d0
														movea.l (L002228),a0
														bra     L0012d0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( (SBYTE)p->S0018 < 0 ) goto L0012e0;
		D0 = *TO_PTR(A4++);
	}
#else
	if ( (SBYTE)A6->S0018 < 0 ) goto L0012e0;
	D0 = *(A4++);
#endif
	A0 = G.L002228;
	goto L0012d0;

L0012cc:;
/*
														lea.l   $001a(a0),a0
*/
	A0 += 0x1a;

L0012d0:;
// checker
	if ( (ULONG)A0 >= ((ULONG)G.L001e34)+((ULONG)G.L002220) ) {
//		G.FATALERROR = 0x0012d0;
//		G.FATALERRORADR = (ULONG)A4;
		return;
	}
// checker end
/*
														cmp.b   (a0)+,d0
														bne     L0012cc
														move.l  a0,$0004(a6)
														ori.b   #$02,$0017(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( *TO_PTR(A0++) != (UBYTE)D0 ) goto L0012cc;
		p->S0004 = A0;
		p->S0017 |= 0x02;
	}
#else
	if ( *(A0++) != (UBYTE)D0 ) goto L0012cc;
	A6->S0004 = A0;
	A6->S0017 |= 0x02;
#endif
	return;

L0012e0:;
/*
														move.b  (a4)+,$0004_b(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0004_b = *TO_PTR(A4++);
	}
#else
	A6->S0004_b = *(A4++);
#endif

}

/***************************************************************/

static void L0012e6(									// @@ p
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0012e6:;
/*
														tst.b   $0018(a6)
														bmi     L001302
														move.b  $001c(a6),d0
														ror.w   #6,d0
														move.b  (a4)+,d0
														rol.w   #6,d0
														move.b  d0,$001c(a6)
														ori.b   #$04,$0017(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( (SBYTE)p->S0018 < 0 ) goto L001302;
		D0 = p->S001c;
		D0 &= 0x3f;
		D0 |= (*TO_PTR(A4++))<<6;
		p->S001c = (UBYTE)D0;
		p->S0017 |= 0x04;
	}
#else
	if ( (SBYTE)A6->S0018 < 0 ) goto L001302;
	D0 = A6->S001c;
	D0 &= 0x3f;
	D0 |= (*(A4++))<<6;
	A6->S001c = D0;
	A6->S0017 |= 0x04;
#endif
	return;

L001302:;
/*
														move.b  (a4)+,d0
														beq     L00130c
														cmp.b   #$03,d0
														bne     L001310
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D0 = *TO_PTR(A4++);
#else
	D0 = *(A4++);
#endif
	if ( D0 == 0 ) goto L00130c;
	if ( D0 != 0x03 ) goto L001310;

L00130c:;
/*
														eori.b  #$03,d0
*/
	D0 ^= 0x03;

L001310:;
/*
														andi.b  #$fc,$001c(a6)
														or.b    d0,$001c(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S001c &= 0xfc;
		p->S001c |= D0;
	}
#else
	A6->S001c &= 0xfc;
	A6->S001c |= D0;
#endif

}

/***************************************************************/

static void L00131c(									// @@ v
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L00131c:;
/*
														move.b  (a4)+,$0022(a6)
														ori.b   #$01,$0017(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0022 = *TO_PTR(A4++);
		p->S0017 |= 0x01;
	}
#else
	A6->S0022 = *(A4++);
	A6->S0017 |= 0x01;
#endif

}

/***************************************************************/

static void L001328(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001328:;
/*
														move.b  $0022(a6),d2
														bmi     L00133c
														beq     L00133a
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D2 = p->S0022;
	}
#else
	D2 = A6->S0022;
#endif
	if ( (SBYTE)D2 < 0 ) goto L00133c;
	if ( D2 == 0 ) goto L00133a;

//L001330:;
/*
														subq.b  #1,$0022(a6)
														ori.b   #$01,$0017(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0022--;
		p->S0017 |= 0x01;
	}
#else
	A6->S0022--;
	A6->S0017 |= 0x01;
#endif

L00133a:;
/*
														rts
*/
	return;

L00133c:;
/*
														cmp.b   #-$01,d2
														bne     L001350
														rts
*/
	if ( D2 == 0xff ) return;

//L001350:;
/*
														addq.b  #1,$0022(a6)
														ori.b   #$01,$0017(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0022++;
		p->S0017 |= 0x01;
	}
#else
	A6->S0022++;
	A6->S0017 |= 0x01;
#endif

}

/***************************************************************/

static void L001330(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001330:;
/*
														subq.b  #1,$0022(a6)
														ori.b   #$01,$0017(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0022--;
		p->S0017 |= 0x01;
	}
#else
	A6->S0022--;
	A6->S0017 |= 0x01;
#endif

//L00133a:;
/*
														rts
*/
	return;

//L00133c:;
/*
														cmp.b   #-$01,d2
														bne     L001350
														rts
*/
	if ( D2 == 0xff ) return;

//L001350:;
/*
														addq.b  #1,$0022(a6)
														ori.b   #$01,$0017(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0022++;
		p->S0017 |= 0x01;
	}
#else
	A6->S0022++;
	A6->S0017 |= 0x01;
#endif

}

/***************************************************************/

static void L001344(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001344:;
/*
														move.b  $0022(a6),d2
														bmi     L00135c
														cmp.b   #$0f,d2
														beq     L00135a
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D2 = p->S0022;
	}
#else
	D2 = A6->S0022;
#endif
	if ( (SBYTE)D2 < 0 ) goto L00135c;
	if ( D2 == 0x0f ) goto L00135a;

//L001350:;
/*
														addq.b  #1,$0022(a6)
														ori.b   #$01,$0017(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0022++;
		p->S0017 |= 0x01;
	}
#else
	A6->S0022++;
	A6->S0017 |= 0x01;
#endif

L00135a:;
/*
														rts
*/
	return;

L00135c:;
/*
														cmp.b   #$80,d2
														bne     L001330
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( (UBYTE)D2 != 0x80 ) { L001330( context ); return; }
#else
	if ( (UBYTE)D2 != 0x80 ) { L001330(); return; }
#endif

}

/***************************************************************/

static void L001364(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001364:;
/*
														move.b  (a4)+,$001e(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S001e = *TO_PTR(A4++);
	}
#else
	A6->S001e = *(A4++);
#endif
}


/***************************************************************/

static void L00136a(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L00136a:;
/*
														ori.b   #$04,$0016(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0016 |= 0x04;
	}
#else
	A6->S0016 |= 0x04;
#endif

}

/***************************************************************/

static void L001372(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	UBYTE t0;

//L001372:;
/*
														move.b  (a4)+,(a4)+
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	t0 = *TO_PTR(A4++); *TO_PTR(A4++) = t0;
#else
	t0 = *(A4++); *(A4++) = t0;
#endif

}

/***************************************************************/

static void L001376(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001376:;
/*
														moveq.l #$ff,d0
														move.b  (a4)+,-(sp)
														move.w  (sp)+,d0
														move.b  (a4)+,d0
														subq.b  #1,-$01(a4,d0.l)
														beq     L001398
														tst.b   (L001e0b)
														beq     L001396
														cmpi.b  #$f1,(a4)
														bne     L001396
														tst.b   $0001(a4)
														beq     L0013e6
*/
	D0 = GETBWORD(A4); A4 += 2;
	D0 = ( D0 ^ 0xffff )+1;
#if MXDRV_ENABLE_PORTABLE_CODE
	*TO_PTR(A4-D0-1) -= 1;
	if ( *TO_PTR(A4-D0-1) == 0 ) goto L001398;
	if ( !G.L001e0b ) goto L001396;
	if ( *TO_PTR(A4) != 0xf1 ) goto L001396;
	if ( !(*TO_PTR(A4+1)) ) { L0013e6( context ); return; };
#else
	*(A4-D0-1) -= 1;
	if ( *(A4-D0-1) == 0 ) goto L001398;
	if ( !G.L001e0b ) goto L001396;
	if ( *(A4) != 0xf1 ) goto L001396;
	if ( !(*(A4+1)) ) { L0013e6(); return; };
#endif

L001396:;
// checker
	if ( (ULONG)(A4-D0) < (ULONG)G.L001e34 ) {
		G.FATALERROR = 0x001396;
		G.FATALERRORADR = (ULONG)A4;
		return;
	}
// checker end
/*
														adda.w  d0,a4
*/
	A4 -= D0;

L001398:;
/*
														rts
*/
	return;

}

/***************************************************************/

static void L00139a(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L00139a:;
/*
														moveq.l #$00,d0
														move.b  (a4)+,-(sp)
														move.w  (sp)+,d0
														move.b  (a4)+,d0
														lea.l   $00(a4,d0.l),a0
														moveq.l #$ff,d0
														move.b  (a0)+,-(sp)
														move.w  (sp)+,d0
														move.b  (a0)+,d0
														cmpi.b  #$01,-$01(a0,d0.l)
														bne     L0013b8
														movea.l a0,a4
*/
	D0 = GETBWORD(A4); A4 += 2;
	A0 = A4+D0;
	D0 = 0xffffffff;
	D0 = GETBWORD(A0); A0 += 2;
	D0 = ( D0 ^ 0xffff )+1;
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( *TO_PTR(A0-D0-1) != 0x01 ) goto L0013b8;
#else
	if ( *(A0-D0-1) != 0x01 ) goto L0013b8;
#endif
	A4 = A0;

L0013b8:;
/*
														rts
*/
	return;

}


/***************************************************************/

static void L0013ba(									// @@ D
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0013ba:;
/*
														move.b  (a4)+,-(sp)
														move.w  (sp)+,d0
														move.b  (a4)+,d0
														move.w  d0,$0010(a6)
														rts
*/
	D0 = GETBWORD( A4 ); A4 += 2;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0010 = (UWORD)D0;
	}
#else
	A6->S0010 = D0;
#endif

}


/***************************************************************/

static void L0013c6(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0013c6:;
/*
														move.b  (a4)+,-(sp)
														move.w  (sp)+,d0
														move.b  (a4)+,d0
														ext.l   d0
														asl.l   #8,d0
														move.l  d0,$0008(a6)
														ori.b   #$80,$0016(a6)
														rts
*/
	D0 = 0;
	D0 = GETBWORD( A4 ); A4 += 2;
	D0 = (SLONG)(SWORD)D0;
	D0 <<= 8;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0008 = D0;
		p->S0016 |= 0x80;
	}
#else
	A6->S0008 = D0;
	A6->S0016 |= 0x80;
#endif

}


/***************************************************************/

static void L0013dc(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0013dc:;
/*
														moveq.l #$ff,d0
														move.b  (a4)+,-(sp)
														beq     L001440
														move.w  (sp)+,d0
														move.b  (a4)+,d0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( !(*TO_PTR(A4++)) ) { L001440( context ); return; }
#else
	if ( !(*(A4++)) ) { L001440(); return; }
#endif
	A4--;
	D0 = GETBWORD( A4 ); A4 += 2;
	D0 = ( D0 ^ 0xffff )+1;

/*
														fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L0013e6( context );
#else
	L0013e6();
#endif
}


/***************************************************************/

static void L0013e6(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0013e6:;
// checker
	if ( (ULONG)(A4-D0) < (ULONG)G.L001e34 ) {
		G.FATALERROR = 0x0013e6;
		G.FATALERRORADR = (ULONG)A4;
		return;
	}
// checker end

/*
														adda.l  d0,a4
														move.w  L001e1a(pc),d0
														bclr.l  d7,d0
														move.w  d0,(L001e1a)
														and.w   L001e06(pc),d0
														bne     L00143e
														tst.b   (L001e18)
														bne     L001416
														move.w  #$01ff,(L001e1a)
														tst.b   (L001df4)
														beq     L001410
														ori.w   #$fe00,(L001e1a)
*/
	A4 -= D0;
	D0 = G.L001e1a;
	D0 &= ~(1<<D7);
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e1a = (uint16_t)D0;
#else
	G.L001e1a = D0;
#endif
	D0 &= G.L001e06;
	if ( D0 ) goto L00143e;
	if ( G.L001e18 ) goto L001416;
	G.L001e1a = 0x01ff;
	if ( !G.L001df4 ) goto L001410;
	G.L001e1a |= 0xfe00;

L001410:;
/*
														addq.w  #1,(L002246)
														bra     L00143e
*/
	G.L002246++;
	goto L00143e;

L001416:;
/*
														tst.b   (L001e17)
														bne     L00143e
														move.w  #$01ff,(L001e1a)
														tst.b   (L001df4)
														beq     L00142e
														ori.w   #$fe00,(L001e1a)
*/
	if ( G.L001e17 ) goto L00143e;
	G.L001e1a = 0x01ff;
	if ( !G.L001df4 ) goto L00142e;
	G.L001e1a |= 0xfe00;

L00142e:;
/*
														subq.w  #1,(L001e22)
														bne     L00143e
														move.w  #$0011,(L001e1e)
														st.b    (L001e17)
*/
	G.L001e22--;
	if ( G.L001e22 ) goto L00143e;
	G.L001e1e[0] = 0x0011;
	G.L001e17 = SET;

L00143e:;
/*
														rts
*/
	return;
}


/***************************************************************/

static void L001440(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001440:;
/*
														addq.w  #2,sp
*/
/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L001442( context );
#else
	L001442();
#endif
}

/***************************************************************/

static void L001442(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
#if MXDRV_ENABLE_PORTABLE_CODE
	/* MxdrvContextImpl 構造体上に移動 */
#else
	static UBYTE L0019b2[] = {
		0x7f,0xf1,0x00,
	};
#endif

//L001442:;
/*
														lea.l   L0019b2(pc),a4
														move.w  L001e1a(pc),d0
														bclr.l  d7,d0
														move.w  d0,(L001e1a)
														move.w  L001e06(pc),d0
														bclr.l  d7,d0
														move.w  d0,(L001e06)
														bne     L001490
														move.b  #$01,(L001e13)
														tst.b   (L001df4)
														beq     L001472
														move.w  #$01ff,d0
														trap    #2
														clr.b   (L001df4)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A4 = TO_OFS(L0019b2);
#else
	A4 = L0019b2;
#endif
	D0 = G.L001e1a;
	D0 &= ~(1<<D7);
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e1a = (uint16_t)D0;
#else
	G.L001e1a = D0;
#endif
	D0 = G.L001e06;
	D0 &= ~(1<<D7);
#if MXDRV_ENABLE_PORTABLE_CODE
	G.L001e06 = (uint16_t)D0;
#else
	G.L001e06 = D0;
#endif
	if ( D0 ) goto L001490;
	G.L001e13 = 0x01;
	if ( !G.L001df4 ) goto L001472;
	D0 = 0x01ff;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif
	G.L001df4 = CLR;

L001472:;
/*
														tst.b   (L001e18)
														bne     L001480
														move.w  #$ffff,(L002246)
														bra     L001490
*/
	if ( G.L001e18 ) goto L001480;
	G.L002246 = 0xffff;
	goto L001490;

L001480:;
/*
														move.w  #$ffff,(L001e1e)
														st.b    (L001e17)
														move.w  #$0037,(L001e14)       ;'7'
*/
	G.L001e1e[0] = 0xffff;
	G.L001e17 = SET;
	G.L001e14 = 0x00; G.L001e15 = 0x37;

L001490:;
/*
														rts
*/
	return;

}


/***************************************************************/

static void L001492(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001492:;
/*
		move.b  (a4)+,$001f(a6)
		rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S001f = *(TO_PTR(A4++));
	}
#else
	A6->S001f = *(A4++);
#endif
}


/***************************************************************/

static void L001498(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001498:;
/*
														moveq.l #$00,d0
														move.b  (a4)+,d0
														lea.l   L001df6(pc),a0
														st.b    $00(a0,d0.w)
														cmp.w   #$0009,d0
														bcc     L0014ae
														st.b    $27(a5,d0.w)	; L002233(d7.w)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D0 = *TO_PTR(A4++);
	A0 = TO_OFS(&G.L001df6[0]);
	TO_PTR(A0)[D0] = SET;
#else
	D0 = *(A4++);
	A0 = &G.L001df6[0];
	A0[D0] = SET;
#endif
	if ( D0 >= 0x0009 ) goto L0014ae;
	G.L002233[D7] = SET;

L0014ae:;
/*
														rts
*/
	return;

}


/***************************************************************/

static void L0014b0(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0014b0:;
/*
														lea.l   L001df6(pc),a0
														tst.b   $00(a0,d7.w)
														beq     L0014d0
														clr.b   $00(a0,d7.w)
														cmp.w   #$0009,d7
														bcc     L0014c8
														clr.b   $27(a5,d7.w)	; L002233(d7.w)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = TO_OFS(&G.L001df6[0]);
	if ( !TO_PTR(A0)[D7] ) goto L0014d0;
	TO_PTR(A0)[D7] = CLR;
#else
	A0 = &G.L001df6[0];
	if ( !A0[D7] ) goto L0014d0;
	A0[D7] = CLR;
#endif
	if ( D7 >= 0x0009 ) goto L0014c8;
	G.L002233[D7] = CLR;

L0014c8:;
/*
														andi.b  #$f7,$0017(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0017 &= 0xf7;
	}
#else
	A6->S0017 &= 0xf7;
#endif
	return;

L0014d0:;
/*
														ori.b   #$08,$0017(a6)
														move.l  a4,(a6)
														addq.w  #4,sp
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0017 |= 0x08;
		p->S0000 = A4;
	}
#else
	A6->S0017 |= 0x08;
	A6->S0000 = A4;
#endif
	DisposeStack_L00122e = TRUE;
	return;

}


/***************************************************************/

static void L0014dc(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0014dc:;
/*
														move.b  (a4)+,d2
														tst.b   $0018(a6)
														bmi     L0014ee
														move.b  d2,(L002232)
														moveq.l #$0f,d1
														bra     L_WRITEOPM
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D2 = *TO_PTR(A4++);
		if ( (SBYTE)(p->S0018) < 0x00 ) goto L0014ee;
	}
	G.L002232 = (uint8_t)D2;
#else
	D2 = *(A4++);
	if ( (SBYTE)(A6->S0018) < 0x00 ) goto L0014ee;
	G.L002232 = D2;
#endif
	D1 = 0x0f;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context ); return;
#else
	L_WRITEOPM(); return;
#endif

L0014ee:;
/*
														lsl.b   #2,d2
														andi.b  #$03,$001c(a6)
														or.b    d2,$001c(a6)
														rts
*/
	D2 <<= 2;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S001c &= 0x03;
		p->S001c |= D2;
	}
#else
	A6->S001c &= 0x03;
	A6->S001c |= D2;
#endif

}


/***************************************************************/

static void L0014fc(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	ULONG d1;

/*
L001588:;
														.dc.w   L0010be-L001588
														.dc.w   L0010d4-L001588
														.dc.w   L0010ea-L001588
														.dc.w   L001100-L001588
*/

//L0014fc:;
/*
														ori.b   #$20,$0016(a6)
														moveq.l #$00,d1
														move.b  (a4)+,d1
														bmi     L001576
														move.w  d1,-(sp)
														andi.b  #$03,d1
														add.w   d1,d1
														move.w  L001588(pc,d1.w),d0
														lea.l   L001588(pc,d0.w),a0
														move.l  a0,$0026(a6)
														move.b  (a4)+,-(sp)
														move.w  (sp)+,d2
														move.b  (a4)+,d2
														move.w  d2,$003c(a6)
														cmp.b   #$02,d1
														beq     L001536
														lsr.w   #1,d2
														cmpi.b  #$06,d1
														bne     L001536
														moveq.l #$01,d2
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0016 |= 0x20;
		D1 = *TO_PTR(A4++);
	}
#else
	A6->S0016 |= 0x20;
	D1 = *(A4++);
#endif
	if ( (SBYTE)D1 < 0 ) goto L001576;
	d1 = D1;
	D1 &= 0x03;
	D1 += D1;
#if MXDRV_ENABLE_PORTABLE_CODE
	A0 = (D1/2)+1;
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0026 = A0;
		D2 = GETBWORD( A4 ); A4 += 2;
		p->S003c = (UWORD)D2;
	}
#else
	A0 = (UBYTE *)((D1/2)+1);
	A6->S0026 = A0;
	D2 = GETBWORD( A4 ); A4 += 2;
	A6->S003c = D2;
#endif
	if ( D1 == 0x02 ) goto L001536;
	D2 >>= 1;
	if ( D1 != 0x06 ) goto L001536;
	D2 = 0x01;

L001536:;
/*
														move.w  d2,$003a(a6)
														move.b  (a4)+,-(sp)
														move.w  (sp)+,d0
														move.b  (a4)+,d0
														ext.l   d0
														asl.l   #8,d0
														move.w  (sp)+,d1
														cmpi.b  #$04,d1
														bcs     L001552
														asl.l   #8,d0
														andi.b  #$03,d1
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S003a = (UWORD)D2;
	}
#else
	A6->S003a = D2;
#endif
	D0 = GETBWORD( A4 ); A4 += 2;
	D0 = (SLONG)(SWORD)D0;
	D0 = ((SLONG)D0)<<8;
	D1 = d1;
	if ( D1 < 0x04 ) goto L001552;
	D0 = ((SLONG)D0)<<8;
	D1 &= 0x03;

L001552:;
/*
														move.l  d0,$002e(a6)
														cmp.b   #$02,d1
														beq     L00155e
														moveq.l #$00,d0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S002e = D0;
	}
#else
	A6->S002e = D0;
#endif
	if ( D1 == 0x02 ) goto L00155e;
	D0 = 0;

L00155e:;
/*
														move.l  d0,$002a(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S002a = D0;
	}
#else
	A6->S002a = D0;
#endif

L001562:;
/*
														move.w  $003a(a6),$003e(a6)
														move.l  $002e(a6),$0032(a6)
														move.l  $002a(a6),$0036(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S003e = p->S003a;
		p->S0032 = p->S002e;
		p->S0036 = p->S002a;
	}
#else
	A6->S003e = A6->S003a;
	A6->S0032 = A6->S002e;
	A6->S0036 = A6->S002a;
#endif
	return;

L001576:;
/*
														and.b   #$01,d1
														bne     L001562
														andi.b  #$df,$0016(a6)
														clr.l   $0036(a6)
														rts
*/
	D1 &= 0x01;
	if ( D1 ) goto L001562;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0016 &= 0xdf;
		p->S0036 = CLR;
	}
#else
	A6->S0016 &= 0xdf;
	A6->S0036 = CLR;
#endif
	return;

}


/***************************************************************/

static void L001590(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
/*
L0015f6:;
		.dc.w   L001120-L0015f6
		.dc.w   L001138-L0015f6
		.dc.w   L00114e-L0015f6
		.dc.w   L001164-L0015f6
*/

//L001590:;
/*
														ori.b   #$40,$0016(a6)
														moveq.l #$00,d2
														move.b  (a4)+,d2
														bmi     L0015e4
														add.w   d2,d2
														move.w  L0015f6(pc,d2.w),d0
														lea.l   L0015f6(pc,d0.w),a0
														move.l  a0,$0040(a6)
														move.b  (a4)+,-(sp)
														move.w  (sp)+,d1
														move.b  (a4)+,d1
														move.w  d1,$004c(a6)
														move.b  (a4)+,-(sp)
														move.w  (sp)+,d0
														move.b  (a4)+,d0
														move.w  d0,$0044(a6)
														btst.l  #$01,d2
														bne     L0015c6
														muls.w  d1,d0
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0016 |= 0x40;
		D2 = *(TO_PTR(A4++));
		if ( (SBYTE)D2 < 0 ) { L0015e4( context ); return; }
		D2 += D2;
		A0 = (D2/2)+1;
		p->S0040 = A0;
		D1 = GETBWORD( A4 ); A4 += 2;
		p->S004c = (UWORD)D1;
		D0 = GETBWORD( A4 ); A4 += 2;
		p->S0044 = (UWORD)D0;
	}
#else
	A6->S0016 |= 0x40;
	D2 = *(A4++);
	if ( (SBYTE)D2 < 0 ) { L0015e4(); return; }
	D2 += D2;
	A0 = (UBYTE *)((D2/2)+1);
	A6->S0040 = A0;
	D1 = GETBWORD( A4 ); A4 += 2;
	A6->S004c = D1;
	D0 = GETBWORD( A4 ); A4 += 2;
	A6->S0044 = D0;
#endif
	if ( D2 & (1<<1) ) goto L0015c6;
	D0 = (SWORD)D0 * (SWORD)D1;

L0015c6:;
/*
														neg.w   d0
														bpl     L0015cc
														moveq.l #$00,d0
*/
	D0 = -((SWORD)D0);
	if ( (SWORD)D0 >= 0 ) goto L0015cc;
	D0 = 0;

L0015cc:;
/*
														move.w  d0,$0046(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0046 = (UWORD)D0;
	}
#else
	A6->S0046 = D0;
#endif

/*
														; fall down
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	L0015d0( context );
#else
	L0015d0();
#endif

}


/***************************************************************/

static void L0015d0(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0015d0:;
/*
		move.w  $004c(a6),$004e(a6)
		move.w  $0044(a6),$0048(a6)
		move.w  $0046(a6),$004a(a6)
		rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S004e = p->S004c;
		p->S0048 = p->S0044;
		p->S004a = p->S0046;
	}
#else
	A6->S004e = A6->S004c;
	A6->S0048 = A6->S0044;
	A6->S004a = A6->S0046;
#endif

}


/***************************************************************/

static void L0015e4(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0015e4:;
/*
														and.b   #$01,d2
														bne     L0015d0
														andi.b  #$bf,$0016(a6)
														clr.w   $004a(a6)
														rts
*/
	D2 &= 0x01;
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( D2 ) { L0015d0( context ); return; }
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0016 &= 0xbf;
		p->S004a = CLR;
	}
#else
	if ( D2 ) { L0015d0(); return; }
	A6->S0016 &= 0xbf;
	A6->S004a = CLR;
#endif

}


/***************************************************************/

static void L0015fe(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	UBYTE c0;

//L0015fe:;
/*
														move.b  (a4)+,d2
														bmi     L00164a
														andi.b  #$fd,$0016(a6)
														bclr.l  #$06,d2
														beq     L001614
														ori.b   #$02,$0016(a6)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D2 = *TO_PTR(A4++);
		if ( (SBYTE)D2 < 0 ) goto L00164a;
		p->S0016 &= 0xfd;
		c0 = D2 & (1<<6); D2 &= ~(1<<6);
		if ( !c0 ) goto L001614;
		p->S0016 |= 0x02;
	}
#else
	D2 = *(A4++);
	if ( (SBYTE)D2 < 0 ) goto L00164a;
	A6->S0016 &= 0xfd;
	c0 = D2 & (1<<6); D2 &= ~(1<<6);
	if ( !c0 ) goto L001614;
	A6->S0016 |= 0x02;
#endif

L001614:;
/*
														move.b  $09da.w,d0
														and.b   #$c0,d0
														or.b    d0,d2
														moveq.l #$1b,d1
														bsr     L_WRITEOPM
														moveq.l #$18,d1
														move.b  (a4)+,d2
														bsr     L_WRITEOPM
														moveq.l #$19,d1
														move.b  (a4)+,d2
														bsr     L_WRITEOPM
														move.b  (a4)+,d2
														bsr     L_WRITEOPM
														move.b  (a4)+,d2
														move.b  d2,$0021(a6)
*/
	D0 = OpmReg1B;
	D0 &= 0xc0;
	D2 |= D0;
	D1 = 0x1b;
#if MXDRV_ENABLE_PORTABLE_CODE
	L_WRITEOPM( context );
	D1 = 0x18;
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D2 = *TO_PTR(A4++);
		L_WRITEOPM( context );
		D1 = 0x19;
		D2 = *TO_PTR(A4++);
		L_WRITEOPM( context );
		D2 = *TO_PTR(A4++);
		L_WRITEOPM( context );
		D2 = *TO_PTR(A4++);
		p->S0021 = (UBYTE)D2;
	}
#else
	L_WRITEOPM();
	D1 = 0x18;
	D2 = *(A4++);
	L_WRITEOPM();
	D1 = 0x19;
	D2 = *(A4++);
	L_WRITEOPM();
	D2 = *(A4++);
	L_WRITEOPM();
	D2 = *(A4++);
	A6->S0021 = D2;
#endif

L001640:;
/*
														moveq.l #$38,d1
														add.b   $0018(a6),d1
														bra     L_WRITEOPM
*/
	D1 = 0x38;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D1 += p->S0018;
	}
	L_WRITEOPM( context ); return;
#else
	D1 += A6->S0018;
	L_WRITEOPM(); return;
#endif

L00164a:;
/*
														and.b   #$01,d2
														beq     L001640
														move.b  $0021(a6),d2
														bra     L001640
*/
	D2 &= 0x01;
	if ( !D2 ) goto L001640;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D2 = p->S0021;
	}
#else
	D2 = A6->S0021;
#endif
	goto L001640;

}


/***************************************************************/

static void L001656(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001656:;
/*
														move.b  (a4)+,$0024(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0024 = *(TO_PTR(A4++));
	}
#else
	A6->S0024 = *(A4++);
#endif

}


/***************************************************************/

static void L00165c(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L00165c:;
/*
														movea.l $0088.w,a0
														cmpa.l  #$00f00000,a0
														bcc     L001692
														cmpi.l  #$50434d34,-$0008(a0)   ;'PCM4'
														beq     L00167c
														cmpi.l  #$50434d38,-$0008(a0)   ;'PCM8'
														bne     L001692
*/
	if ( !PCM8 ) goto L001692;

//L00167c:;
/*
														st.b    (L001df4)
														move.w  #$01fe,d0
														trap    #2
														ori.w   #$fe00,(L001e1a)
														ori.w   #$fe00,(L001e06)
*/
	G.L001df4 = SET;
	D0 = 0x01fe;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif
	G.L001e1a |= 0xfe00;
	G.L001e06 |= 0xfe00;

L001692:;
/*
														rts
*/
	return;

}


/***************************************************************/

static void L001694(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

/*
L0016aa:;
		.dc.w   L001442-L0016aa
		.dc.w   L0016b8-L0016aa
		.dc.w   L0016c6-L0016aa
		.dc.w   L0016fa-L0016aa
		.dc.w   L00170e-L0016aa
		.dc.w   L00178a-L0016aa
		.dc.w   L0017a0-L0016aa
*/

#if MXDRV_ENABLE_PORTABLE_CODE
	static void (*L0016aa[])( MxdrvContext * ) = {
#else
	static void (*L0016aa[])(void) = {
#endif
		L001442,
		L0016b8,
		L0016c6,
		L0016fa,
		L00170e,
		L00178a,
		L0017a0,
	};


//L001694:;
/*
		moveq.l #$00,d0
		move.b  (a4)+,d0
		cmp.w   #$0007,d0
		bcc     L001442
		add.w   d0,d0
		move.w  L0016aa(pc,d0.w),d0
		jmp     L0016aa(pc,d0.w)
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D0 = *TO_PTR(A4++);
	if ( D0 > 7 ) { L001442( context ); return; }
	L0016aa[D0]( context );
#else
	D0 = *(A4++);
	if ( D0 > 7 ) { L001442(); return; }
	L0016aa[D0]();
#endif
}

/***************************************************************/

static void L0016b8(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0016b8:;
/*
														moveq.l #$00,d0
														move.b  (a4)+,d0
														move.w  d0,(L001e1e)
														st.b    (L001e17)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D0 = *TO_PTR(A4++);
	G.L001e1e[0] = (uint16_t)D0;
#else
	D0 = *(A4++);
	G.L001e1e[0] = D0;
#endif
	G.L001e17 = SET;

}


/***************************************************************/

static void L0016c6(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0016c6:;
/*
														movea.l $0088.w,a0
														move.l  -$0008(a0),d0
														cmp.l   #$50434d38,d0           ;'PCM8'
														beq     L0016e2
														cmp.l   #$50434d34,d0           ;'PCM4'
														beq     L0016e2
														addq.w  #6,a4
														rts
*/
	if ( PCM8 ) goto L0016e2;
	A4 += 6;
	return;

L0016e2:;
/*
														move.b  (a4)+,-(sp)
														move.w  (sp)+,d0
														move.b  (a4)+,d0
														move.b  (a4)+,-(sp)
														move.w  (sp)+,d1
														move.b  (a4)+,d1
														swap.w  d1
														move.b  (a4)+,-(sp)
														move.w  (sp)+,d1
														move.b  (a4)+,d1
														trap    #2
														rts
*/
	D0 = GETBWORD( A4 ); A4 += 2;
	D1 = GETBLONG( A4 ); A4 += 4;
#if MXDRV_ENABLE_PORTABLE_CODE
	PCM8_SUB( context );
#else
	PCM8_SUB();
#endif

}

/***************************************************************/

static void L0016fa(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0016fa:;
/*
														tst.b   (a4)+
														beq     L001706
														ori.b   #$10,$0016(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( !*TO_PTR(A4++) ) goto L001706;
		p->S0016 |= 0x10;
	}
#else
	if ( !*(A4++) ) goto L001706;
	A6->S0016 |= 0x10;
#endif
	return;

L001706:;
/*
														andi.b  #$ef,$0016(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0016 &= 0xef;
	}
#else
	A6->S0016 &= 0xef;
#endif

}

/***************************************************************/

static void L00170e(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {
	ULONG d1;
#if MXDRV_ENABLE_PORTABLE_CODE
	uint32_t a6;
	ULONG d7;
	uint32_t a6s0000;
#else
	volatile MXWORK_CH *a6;
	ULONG d7;
	UBYTE volatile *a6s0000;
#endif

//L00170e:;
/*
														move.b  (a4)+,d0
														movem.l d7/a6,-(sp)
														lea.l   CHBUF_FM(pc),a6
														move.w  d0,d7
														cmp.b   #$09,d0
														bcs     L001724
														lea.l   CHBUF_PCM-($50*9)(pc),a6
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D0 = *TO_PTR(A4++);
#else
	D0 = *(A4++);
#endif
	a6=A6, d7=D7;
	D7 = D0;
#if MXDRV_ENABLE_PORTABLE_CODE
	if ( D0 < 0x09 ) {
		A6 = TO_OFS(&MXWORK_CHBUF_FM[D0]);
	} else {
		A6 = TO_OFS(&MXWORK_CHBUF_PCM[D0-9]);
	}
#else
	if ( D0 < 0x09 ) {
		A6 = &MXWORK_CHBUF_FM[D0];
	} else {
		A6 = &MXWORK_CHBUF_PCM[D0-9];
	}
#endif

//L001724:;
/*
														mulu.w  #$0050,d0
														adda.w  d0,a6
														move.l  (a6),-(sp)
														andi.b  #$7b,$0016(a6)
														moveq.l #$00,d0
														moveq.l #$00,d1
														move.b  (a4)+,d0
														move.b  d0,d1
														bpl     L00176e
														cmp.b   #$e0,d0
														bcc     L001240
														and.w   #$007f,d0
														lsl.w   #6,d0
														addq.w  #5,d0
														add.w   $0010(a6),d0
														move.w  d0,$0012(a6)
														ori.b   #$01,$0016(a6)
														move.b  $001f(a6),$0020(a6)
														moveq.l #$00,d0
														move.b  (a4)+,d0
														move.b  $001e(a6),d1
														bmi     L001782
														mulu.w  d0,d1
														lsr.w   #3,d1
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		a6s0000 = p->S0000;
		p->S0016 &= 0x7b;
	}
#else
	a6s0000 = A6->S0000;
	A6->S0016 &= 0x7b;
#endif
	D0 = 0;
	D1 = 0;
#if MXDRV_ENABLE_PORTABLE_CODE
	D0 = *TO_PTR(A4++);
#else
	D0 = *(A4++);
#endif
	D1 = D0;
	if ( (SBYTE)D0 >= 0 ) goto L00176e;
	if ( D0 >= 0xe0 ) goto L001240;
	D0 &= 0x007f;
	D0 <<= 6;
	D0 += 0x05;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		D0 += p->S0010;
		p->S0012 = (UWORD)D0;
		p->S0016 |= 0x01;
		p->S0020 = p->S001f;
		D0 = *TO_PTR(A4++);
		D1 = p->S001e;
	}
#else
	D0 += A6->S0010;
	A6->S0012 = D0;
	A6->S0016 |= 0x01;
	A6->S0020 = A6->S001f;
	D0 = *(A4++);
	D1 = A6->S001e;
#endif
	if ( (SBYTE)D1 < 0 ) goto L001782;
	D1 = (UWORD)D1 * (UWORD)D0;
	D1 >>= 3;

L00176e:;
/*
														addq.w  #1,d1
														move.b  d1,$001b(a6)
														addq.w  #1,d0
														move.b  d0,$001a(a6)
*/
	D1++;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S001b = (UBYTE)D1;
		D0++;
		p->S001a = (UBYTE)D0;
	}
#else
	A6->S001b = D1;
	D0++;
	A6->S001a = D0;
#endif

L00177a:;
/*
														move.l  (sp)+,(a6)
														movem.l (sp)+,d7/a6
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0000 = a6s0000;
	}
#else
	A6->S0000 = a6s0000;
#endif
	D7 = d7, A6 = a6;
	return;


L001782:;
/*
														add.b   d0,d1
														bcs     L00176e
														moveq.l #$00,d1
														bra     L00176e
*/
	d1 = (UBYTE)D1;
	d1 += (UBYTE)D0;
	if ( d1 >= 0x100 ) {
		D1 = (D1&0xffffff00)|(d1&0x000000ff);
		goto L00176e;
	}
	D1 = 0x00;
	goto L00176e;

L001240:;
/*
														ext.w   d0
														not.w   d0
														add.w   d0,d0
														move.w  L001252(pc,d0.w),d0
														pea.l   L00177a(pc)
														jmp     L001252(pc,d0.w)
*/
	D0 ^= 0xff;
	DisposeStack_L00122e = FALSE;
#if MXDRV_ENABLE_PORTABLE_CODE
	L001252[D0]( context );
#else
	L001252[D0]();
#endif
	if ( DisposeStack_L00122e ) return;
	goto L00177a;

}


/***************************************************************/

static void L001216(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L001216:;
/*
														addq.w  #1,d1
														move.b  d1,$001b(a6)
														addq.w  #1,d0
														move.b  d0,$001a(a6)
														move.l  a4,(a6)
*/
	D1++;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S001b = (UBYTE)D1;
		D0++;
		p->S001a = (UBYTE)D0;
		p->S0000 = A4;
	}
#else
	A6->S001b = D1;
	D0++;
	A6->S001a = D0;
	A6->S0000 = A4;
#endif

//L001224:;
/*
														rts
*/
	return;

}

static void L00178a(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L00178a:;
/*
														move.b  (a4)+,d0
														move.b  d0,d1
														bsr     L001216
														andi.b  #$fe,$0016(a6)
														bsr     L000e7e
														addq.w  #4,sp
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	D0 = *TO_PTR(A4++);
#else
	D0 = *(A4++);
#endif
	D1 = D0;
#if MXDRV_ENABLE_PORTABLE_CODE
	L001216( context );
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0016 &= 0xfe;
	}
	L000e7e( context );
#else
	L001216();
	A6->S0016 &= 0xfe;
	L000e7e();
#endif
	DisposeStack_L00122e = TRUE;
	return;
}


/***************************************************************/

static void L0017a0(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L0017a0:;
/*
														tst.b   (a4)+
														beq     L0017ac
														ori.b   #$80,$0017(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		if ( !*TO_PTR(A4++) ) goto L0017ac;
		p->S0017 |= 0x80;
	}
#else
	if ( !*(A4++) ) goto L0017ac;
	A6->S0017 |= 0x80;
#endif

L0017ac:;
/*
														andi.b  #$7f,$0017(a6)
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		volatile MXWORK_CH *p = (MXWORK_CH *)TO_PTR(A6);
		p->S0017 &= 0x7f;
	}
#else
	A6->S0017 &= 0x7f;
#endif

}


/***************************************************************/

static void L_WRITEOPM(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context
#else
	void
#endif
) {

//L_WRITEOPM:;
/*
														ori.w   #$0300,sr
														tst.b   $00e90003
														bmi     L_WRITEOPM
														move.b  d1,$00e90001
														and.w   #$00ff,d1
														lea.l   OPMBUF(pc),a2
														move.b  d2,$00(a2,d1.w)
														move.b  d2,$00e90003
														andi.w  #$faff,sr
														cmp.b   #$1b,d1
														beq     L0017e4
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	OPM_SUB( context );
#else
	OPM_SUB();
#endif
	D1 &= 0xff;
#if MXDRV_ENABLE_PORTABLE_CODE
	OPMBUF[D1] = (uint8_t)D2;
#else
	OPMBUF[D1] = D2;
#endif
	if ( D1 == 0x1b ) goto L0017e4;
	return;

L0017e4:;
/*
														move.b  d2,$09da.w
														rts
*/
#if MXDRV_ENABLE_PORTABLE_CODE
	OpmReg1B = (unsigned char)D2;
#else
	OpmReg1B = D2;
#endif

}


/***************************************************************/
/***************************************************************/

static int Initialize(
#if MXDRV_ENABLE_PORTABLE_CODE
	MxdrvContext *context,
#endif
	int mdxbuf,
	int pdxbuf
) {
	G.L002220 = ( mdxbuf ? mdxbuf : 0x10000 );
	G.L002224 = ( pdxbuf ? pdxbuf : 0x100000 );
	G.L001ba8 = 0x600;
#if MXDRV_ENABLE_PORTABLE_CODE
	{
		uint8_t *p;

		MxdrvContextImpl_ResetMemoryPool(context->m_impl);

		p = MxdrvContextImpl_ReserveMemory(context->m_impl, G.L002220);
		if (p == NULL) return (!0);
		G.L001e34 = TO_OFS(p);

		p = MxdrvContextImpl_ReserveMemory(context->m_impl, G.L002224);
		if (p == NULL) return (!0);
		G.L001e38 = TO_OFS(p);

		p = MxdrvContextImpl_ReserveMemory(context->m_impl, G.L001ba8);
		if (p == NULL) return (!0);
		G.L001bac = TO_OFS(p);
	}
	memset( TO_PTR(G.L001e34), 0, G.L002220 );
	memset( TO_PTR(G.L001e38), 0, G.L002224 );
	memset( TO_PTR(G.L001bac), 0, G.L001ba8 );
#else
	G.L001e34 = (UBYTE *)malloc( G.L002220 );
	if ( !G.L001e34 ) {
		return (!0);
	}
	memset( (void *)G.L001e34, 0, G.L002220 );
	G.L001e38 = (UBYTE *)malloc( G.L002224 );
	if ( !G.L001e38 ){
		free( (void *)G.L001e34 );
		G.L001e34 = NULL;
		return (!0);
	}
	memset( (void *)G.L001e38, 0, G.L002224 );
	G.L001bac = (UBYTE *)malloc( G.L001ba8 );
	if ( !G.L001bac ) {
		free( (void *)G.L001e38 );
		G.L001e38 = NULL;
		free( (void *)G.L001e34 );
		G.L001e34 = NULL;
		return (!0);
	}
	memset( (void *)G.L001bac, 0, G.L001ba8 );
#endif

	return (0);
}

/***************************************************************/

#if 0
L0017ea:;
		clr.l   -(sp)
		DOS     _SUPER
		pea.l   L0019b5(pc)
		DOS     _PRINT
		lea.l   L00220c(pc),a5
		move.w  #$0001,(L001e12)
		clr.l   (L001e08)
		move.l  $0008(a0),(L001bb0)
		move.l  #$00010000,(L002220)
		move.l  #$0004e000,(L002224)
		move.l  #$00000600,(L001ba8)
		clr.b   (L002230)
		clr.b   (L002231)
		addq.w  #1,a2
		bsr     L001892
		lea.l   L002248(pc),a4
		lea.l   L001e34(pc),a1
		move.l  a4,(a1)
		adda.l  (L002220),a4
		move.l  a4,$0004(a1)
		adda.l  (L002224),a4
		move.l  a4,(L001bac)
		adda.l  (L001ba8),a4
		cmpa.l  $0008(a0),a4
		bhi     L0019a0
		suba.l  a0,a0
		move.l  $0090(a0),(a5)
		move.l  $010c(a0),$0004(a5)
		lea.l   L00000c(pc),a1
		move.l  a1,$0090(a0)
		lea.l   L000000(pc),a1
		suba.l  a1,a4
		clr.w   -(sp)
		move.l  a4,-(sp)
		DOS     _KEEPPR

L001872:;
		movea.l $0090.w,a4
		cmpa.l  #$00fe0000,a4
		bcc     L001890
		subq.w  #8,a4
		lea.l   L000004(pc),a3
		move.w  #$0004,d0
L001888:;
		cmpm.b  (a3)+,(a4)+
		bne     L001890
		dbra    d0,L001888
L001890:;
		rts

L001892:;
		move.b  (a2)+,d0
		bne     L00189e
		bsr     L001872
		beq     L00199a
		rts

L00189e:;
		cmp.b   #$20,d0                 ;' '
		beq     L001892
		cmp.b   #$09,d0
		beq     L001892
		cmp.b   #$2d,d0                 ;'-'
		beq     L0018b8
		cmp.b   #$2f,d0                 ;'/'
		bne     L0019a6
L0018b8:;
		move.b  (a2)+,d0
		beq     L0019a6
		or.b    #$20,d0
		cmp.b   #$72,d0                 ;'r'
		bne     L0018e8
		bsr     L001872
		bne     L001994
		moveq.l #$00,d0
		trap    #4
		tst.l   d0
		bne     L0018e2
		pea.l   L001b2e(pc)
L0018da:;
		DOS     _PRINT
		addq.w  #4,sp
		clr.w   -(sp)
		DOS     _EXIT2

L0018e2:;
		pea.l   L001b87(pc)
		bra     L0018da
L0018e8:;
		bsr     L001872
		beq     L00199a
		cmp.b   #$6d,d0                 ;'m'
		bne     L001904
		cmpi.b  #$3a,(a2)               ;':'
		bne     L0018fc
		addq.w  #1,a2
L0018fc:;
		bsr     L001966
		move.l  d0,(L002220)
		bra     L001892
L001904:;
		cmp.b   #$70,d0                 ;'p'
		bne     L00191c
		cmpi.b  #$3a,(a2)               ;':'
		bne     L001912
		addq.w  #1,a2
L001912:;
		bsr     L001966
		move.l  d0,(L002224)
		bra     L001892
L00191c:;
		cmp.b   #$62,d0                 ;'b'
		bne     L00193c
		cmpi.b  #$3a,(a2)               ;':'
		bne     L00192a
		addq.w  #1,a2
L00192a:;
		bsr     L001972
		move.l  d0,-(sp)
		add.l   d0,d0
		add.l   (sp)+,d0
		lsl.l   #8,d0
		move.l  d0,(L001ba8)
		bra     L001892
L00193c:;
		cmpi.b  #$66,d0                 ;'f'
		bne     L0019a6
		cmpi.b  #$3a,(a2)               ;':'
		bne     L00194a
		addq.w  #1,a2
L00194a:;
		move.l  a2,-(sp)
		bsr     L001972
		cmpa.l  (sp)+,a2
		beq     L00195a
		andi.l  #$00007fff,d0
		bra     L00195c
L00195a:;
		moveq.l #$11,d0
L00195c:;
		move.w  d0,L000b90+$000002
		bra     L001892
L001966:;
		bsr     L001972
		tst.l   d0
		bmi     L0019a6
		moveq.l #$0a,d1
		lsl.l   d1,d0
		rts

L001972:;
		moveq.l #$00,d0
L001974:;
		moveq.l #$00,d1
		move.b  (a2),d1
		sub.b   #$30,d1                 ;'0'
		bcs     L001992
		cmp.b   #$09,d1
		bhi     L001992
		add.l   d0,d0
		add.l   d0,d1
		add.l   d0,d0
		add.l   d0,d0
		add.l   d1,d0
		addq.w  #1,a2
		bra     L001974
L001992:;
		rts

L001994:;
		pea.l   L001b68(pc)
		bra     L0019aa
L00199a:;
		pea.l   L001b45(pc)
		bra     L0019aa
L0019a0:;
		pea.l   L001b14(pc)
		bra     L0019aa
L0019a6:;
		pea.l   L001a15(pc)
L0019aa:;
		DOS     _PRINT
		move.w  #$ffff,-(sp)
		DOS     _EXIT2

L0019b2:;
		.dc.b   $7f,$f1,$00
L0019b5:;
		.dc.b   'X68k MXDRV music driver version 2.06+17 Rel.X5-S (c)1988-92'
		.dc.b   ' milk.,K.MAEKAWA, Missy.M, Yatsube',$0d,$0a,$00
L001a15:;
		.dc.b   '使い方: mxdrv [switch]',$0d,$0a
		.dc.b   $09,'-m:<num> MMLバッファサイズ(Kbytes)',$09,$09,'[省略時 64'
		.dc.b   ']',$0d,$0a
		.dc.b   $09,'-p:<num> PCMバッファサイズ(Kbytes)',$09,$09,'[省略時312'
		.dc.b   ']',$0d,$0a
		.dc.b   $09,'-b:<num> LINKバッファサイズ(banks)',$09,$09,'[省略時  2'
		.dc.b   ']',$0d,$0a
		.dc.b   $09,'-f:<num> FADEOUT SPEED (FAST 0 - 32767 SLOW)',$09,'[省略'
		.dc.b   '時 23]',$0d,$0a
		.dc.b   $09,'-r       mxdrv解除',$0d,$0a,$00
L001b14:;
		.dc.b   $09,'メモリが不足しています',$0d,$0a,$00
L001b2e:;
		.dc.b   $09,'mxdrvを解除しました',$0d,$0a,$00
L001b45:;
		.dc.b   $09,'mxdrvはすでに組み込まれています',$0d,$0a,$00
L001b68:;
		.dc.b   $09,'mxdrvは組み込まれていません',$0d,$0a,$00
L001b87:;
		.dc.b   $09,'mxdrvを解除出来ませんでした',$0d,$0a,$00

		.data


		.bss

L001ba6:;
		.ds.b   10
L001bb0:;
		.ds.l   1
L001bb4:;
		.ds.b   16
CHBUF_PCM:;
		.ds.b   562
L001df6:;
		.ds.b   16
L001e06:;
		.ds.w   1
L001e08:;
		.ds.b   1
L001e09:;
		.ds.b   3
L001e0c:;
		.ds.b   2
L001e0e:;
		.ds.b   6
L001e14:;
		.ds.b   3
L001e17:;
		.ds.b   2
L001e19:;
		.ds.b   1
L001e1a:;
		.ds.w   1
L001e1c:;
		.ds.w   1
L001e1e:;
		.ds.b   6
L001e24:;
		.ds.l   1
L001e28:;
		.ds.l   1
L001e2c:;
		.ds.l   1
L001e30:;
		.ds.l   1
L001e34:;
		.ds.l   1
L001e38:;
		.ds.l   1
OPMBUF:;
		.ds.b   256
CHBUF_FM:;
		.ds.b   720
L00220c:;
		.ds.b   39
L002233:;
		.ds.b   9
L00223c:;
		.ds.b   12
L002248:;

L001190:;
		.dc.w   $1234

		.end    L0017ea
#endif

