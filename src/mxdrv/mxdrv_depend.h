// MXDRV.DLL X68000-depend header
// Copyright (C) 2000 GORRY.

#ifndef __MXDRV_DEPEND_H__
#define __MXDRV_DEPEND_H__

#include "mxdrv_config.h"

#if MXDRV_ENABLE_PORTABLE_CODE
	#ifdef _WIN32
		#include <windows.h>
		#include <stdint.h>
	#else
		#include <stdlib.h>
		#define BOOL bool
		#include <stdint.h>
		typedef uint32_t UINT;
		typedef uint8_t BYTE;
		typedef uint32_t DWORD;
		typedef int64_t LONGLONG;
	#endif
#endif

#ifdef _WIN32
	typedef unsigned char UBYTE;
	typedef unsigned short UWORD;
	typedef unsigned long ULONG;
	typedef signed char SBYTE;
	typedef signed short SWORD;
	typedef signed long SLONG;
#else
	typedef uint8_t UBYTE;
	typedef uint16_t UWORD;
	typedef uint32_t ULONG;
	typedef int8_t SBYTE;
	typedef int16_t SWORD;
	typedef int32_t SLONG;
#endif

#define FALSE 0
#define TRUE 1

#if MXDRV_ENABLE_PORTABLE_CODE
	/* mxdrv_context.h に移動 */
#else
typedef struct __X68REG {
    ULONG d0;
    ULONG d1;
    ULONG d2;
    ULONG d3;
    ULONG d4;
    ULONG d5;
    ULONG d6;
    ULONG d7;
    UBYTE *a0;
    UBYTE *a1;
    UBYTE *a2;
    UBYTE *a3;
    UBYTE *a4;
    UBYTE *a5;
    UBYTE *a6;
    UBYTE *a7;
} X68REG;
#endif

#define SET 255
#define CLR 0
#if MXDRV_ENABLE_PORTABLE_CODE
	#define GETBWORD(a) ((((UBYTE *)TO_PTR(a))[0]*256)+(((UBYTE *)TO_PTR(a))[1]))
	#define GETBLONG(a) ((((UBYTE *)TO_PTR(a))[0]*16777216)+(((UBYTE *)TO_PTR(a))[1]*65536)+(((UBYTE *)TO_PTR(a))[2]*256)+(((UBYTE *)TO_PTR(a))[3]))
	#define PUTBWORD(a,b) ((((UBYTE *)TO_PTR(a))[0]=(UBYTE)((b)>> 8)),(((UBYTE *)TO_PTR(a))[1]=(UBYTE)((b)>> 0)))
	#define PUTBLONG(a,b) ((((UBYTE *)TO_PTR(a))[0]=(UBYTE)((b)>>24)),(((UBYTE *)TO_PTR(a))[1]=(UBYTE)((b)>>16)), (((UBYTE *)TO_PTR(a))[2]=(UBYTE)((b)>> 8)), (((UBYTE *)TO_PTR(a))[3]=(UBYTE)((b)>> 0)))
#else
	#define GETBWORD(a) ((((UBYTE *)(a))[0]*256)+(((UBYTE *)(a))[1]))
	#define GETBLONG(a) ((((UBYTE *)(a))[0]*16777216)+(((UBYTE *)(a))[1]*65536)+(((UBYTE *)(a))[2]*256)+(((UBYTE *)(a))[3]))
	#define PUTBWORD(a,b) ((((UBYTE *)(a))[0]=((b)>> 8)),(((UBYTE *)(a))[1]=((b)>> 0)))
	#define PUTBLONG(a,b) ((((UBYTE *)(a))[0]=((b)>>24)),(((UBYTE *)(a))[1]=((b)>>16)), (((UBYTE *)(a))[2]=((b)>> 8)), (((UBYTE *)(a))[3]=((b)>> 0)))
#endif










#endif //__MXDRV_DEPEND_H__
