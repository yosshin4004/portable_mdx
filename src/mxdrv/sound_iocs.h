#ifndef __SOUND_IOCS_H__
#define __SOUND_IOCS_H__

#include "mxdrv_config.h"

#if MXDRV_ENABLE_PORTABLE_CODE
	#include <mxdrv_context.h>
#endif

#if !MXDRV_ENABLE_PORTABLE_CODE
unsigned short bswapw(unsigned short data);
void *bswapl(void *adrs);

// アレイチェインテーブル構造体を定義
// 構造体のアライメントは2にする
#pragma pack(2)
struct _chain {
	void *addr;
	unsigned short len;
};

// リンクアレイチェインテーブル構造体を定義
// 構造体のアライメントは2にする
#pragma pack(2)
struct _chain2 {
	void *addr;
	unsigned short len;
	struct _chain2 *next;
};
#endif


#if MXDRV_ENABLE_PORTABLE_CODE
void sound_iocs_init(MxdrvContext *context);

void _iocs_opmset(MxdrvContext *context, int addr, int data);
int _iocs_opmsns(MxdrvContext *context);
intptr_t _iocs_opmintst(MxdrvContext *context, void *addr, void *arg);
void _iocs_adpcmout(MxdrvContext *context, void *addr, int mode, int len);
//void _iocs_adpcmaot(MxdrvContext *context, struct _chain *tbl, int mode, int cnt);	/* 未使用 */
//void _iocs_adpcmlot(MxdrvContext *context, struct _chain2 *tbl, int mode);	/* 未使用 */
void _iocs_adpcmcot(MxdrvContext *context, void *addr, int mode, int len);
int _iocs_adpcmsns(MxdrvContext *context);
void _iocs_adpcmmod(MxdrvContext *context, int mode);
#else
void sound_iocs_init();

void _iocs_opmset(int addr, int data);
int _iocs_opmsns();
int _iocs_opmintst(void *addr);
void _iocs_adpcmout(void *addr, int mode, int len);
void _iocs_adpcmaot(struct _chain *tbl, int mode, int cnt);
void _iocs_adpcmlot(struct _chain2 *tbl, int mode);
void _iocs_adpcmcot(void *addr, int mode, int len);
int _iocs_adpcmsns();
void _iocs_adpcmmod(int mode);
#endif


#endif //__SOUND_IOCS_H__
