/* Copyright (C) 2018 Yosshin(@yosshin4004) */

#ifndef __MDX_UTIL_H__
#define __MDX_UTIL_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* チャンクタイプ */
typedef enum {
	MDX_CHUNK_TYPE_TITLE,
	MDX_CHUNK_TYPE_PDX_FILE_NAME,
	MDX_CHUNK_TYPE_MDX_BODY,
} MDX_CHUNK_TYPE;

/* MDX ファイルイメージのシーク */
bool MdxSeekFileImage(
	const void *mdxFileImage,
	uint32_t mdxFileImageSizeInBytes,
	MDX_CHUNK_TYPE chunkType,
	/* out */ uint32_t *ofs
);

/* タイトル文字列の取得 */
bool MdxGetTitle(
	const void *mdxFileImage,
	uint32_t mdxFileImageSizeInBytes,
	/* out */ char *titleBuffer,
	size_t titleBufferSizeInBytes
);

/* PDX ファイルを要求するか？ */
bool MdxHasPdxFileName(
	const void *mdxFileImage,
	uint32_t mdxFileImageSizeInBytes,
	/* out */ bool *hasPdxFileName
);

/* PDX ファイル名の取得 */
bool MdxGetPdxFileName(
	const void *mdxFileImage,
	uint32_t mdxFileImageSizeInBytes,
	/* out */ char *pdxFileNameBuffer,
	size_t pdxFileNameBufferSizeInBytes
);

/* 必要なデータバッファサイズを求める */
bool MdxGetRequiredBufferSize(
	const void *mdxFileImage,
	uint32_t mdxFileImageSizeInBytes,
	uint32_t pdxFileImageSizeInBytes,					/* 省略する場合は 0 */
	/* out */ uint32_t *requiredMdxBufferSizeInBytes,	/* 省略する場合は NULL */
	/* out */ uint32_t *requiredPdxBufferSizeInBytes	/* 省略する場合は NULL */
);

/* MDX PDX バッファを作成 */
bool MdxUtilCreateMdxPdxBuffer(
	const void *mdxFileImage,
	uint32_t mdxFileImageSizeInBytes,
	const void *pdxFileImage,			/* 省略する場合は NULL */
	uint32_t pdxFileImageSizeInBytes,	/* 省略する場合は 0 */
	/* out */ void *mdxBuffer,
	uint32_t mdxBufferSizeInBytes,
	/* out */ void *pdxBuffer,			/* 省略する場合は NULL */
	uint32_t pdxBufferSizeInBytes		/* 省略する場合は 0 */
);

#ifdef __cplusplus
}
#endif

#endif //__MDX_UTIL_H__

