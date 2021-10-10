/* Copyright (C) 2018 Yosshin(@yosshin4004) */

#include <string.h>		/* for memcpy */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <mdx_util.h>

/* MDX ファイルイメージのシーク */
bool MdxSeekFileImage(
	const void *mdxFileImage,
	uint32_t mdxFileImageSizeInBytes,
	MDX_CHUNK_TYPE chunkType,
	/* out */ uint32_t *ofs
){
	if (mdxFileImage == NULL) return false;				/* 不正な引数 */
	if (mdxFileImageSizeInBytes == 0) return false;		/* 不正な引数 */
	if (ofs == 0) return false;							/* 不正な引数 */

	const uint8_t *src = (const uint8_t *)mdxFileImage;
	uint32_t ofsSrc = 0;

	/* タイトル文字列までシーク？ */
	if (chunkType == MDX_CHUNK_TYPE_TITLE) {
		*ofs = ofsSrc;
		return true;
	}

	/* CR か LF が見つかるまでタイトル文字列をスキップ */
	uint8_t c;
	for (;;) {
		if (ofsSrc >= mdxFileImageSizeInBytes) return false;	/* 不正なデータ */
		c = src[ofsSrc++];
		if (c == 0x0d /* CR */ || c == 0x0a /* LF */) break;
		if (c < 0x20 && c != 0x09 /* TAB */ && c != 0x1b /* ESC */) {
			return false;										/* 不正なデータ */
		}
	}
	if (ofsSrc >= mdxFileImageSizeInBytes) return false;		/* 不正なデータ */

	/* 2byte アライン */
	ofsSrc += ofsSrc & 1;
	if (ofsSrc >= mdxFileImageSizeInBytes) return false;		/* 不正なデータ */

	/* CR が見つかるまでスキップ */
	if (c != 0x0d /* CR */) {
		for (;;) {
			if (ofsSrc >= mdxFileImageSizeInBytes) return false;/* 不正なデータ */
			if (src[ofsSrc++] == 0x0d /* CR */) break;
		}
	}
	if (ofsSrc >= mdxFileImageSizeInBytes) return false;		/* 不正なデータ */

	/* 0x1a が見つかるまでスキップ */
	for (;;) {
		if (ofsSrc >= mdxFileImageSizeInBytes) return false;	/* 不正なデータ */
		if (src[ofsSrc++] == 0x1a) break;
	}
	if (ofsSrc >= mdxFileImageSizeInBytes) return false;		/* 不正なデータ */

	/* PDX ファイル名文字列までシーク？ */
	if (chunkType == MDX_CHUNK_TYPE_PDX_FILE_NAME) {
		*ofs = ofsSrc;
		return true;
	}

	/* \0 が見つかるまで PDX ファイル名文字列をスキップ */
	for (;;) {
		if (ofsSrc >= mdxFileImageSizeInBytes) return false;	/* 不正なデータ */
		c = src[ofsSrc++];
		if (c == '\0') break;
	}
	if (ofsSrc >= mdxFileImageSizeInBytes) return false;		/* 不正なデータ */

	/* MDX BODY までシーク？ */
	if (chunkType == MDX_CHUNK_TYPE_MDX_BODY) {
		*ofs = ofsSrc;
		return true;
	}

	return false;		/* 不正な引数 */
}

/* タイトル文字列の取得 */
bool MdxGetTitle(
	const void *mdxFileImage,
	uint32_t mdxFileImageSizeInBytes,
	/* out */ char *titleBuffer,
	size_t titleBufferSizeInBytes
){
	if (mdxFileImage == NULL) return false;				/* 不正な引数 */
	if (mdxFileImageSizeInBytes == 0) return false;		/* 不正な引数 */
	if (titleBuffer == NULL) return false;				/* 不正な引数 */
	if (titleBufferSizeInBytes == 0) return false;		/* 不正な引数 */

	const uint8_t *src = (const uint8_t *)mdxFileImage;
	uint32_t ofsSrc = 0;

	/* タイトル文字列までシーク */
	if (
		MdxSeekFileImage(
			mdxFileImage,
			mdxFileImageSizeInBytes,
			MDX_CHUNK_TYPE_TITLE,
			&ofsSrc
		) == false
	) return false;		/* 不正なデータ */

	/* CR か LF が見つかるまでタイトル文字列をコピー */
	uint8_t *dst = (uint8_t *)titleBuffer;
	uint8_t *dstEnd = (uint8_t *)titleBuffer + titleBufferSizeInBytes;
	for (;;) {
		if (dst == dstEnd - 1) {
			*dst++ = '\0';
			return false;		/* バッファ容量不足 */
		}
		if (ofsSrc >= mdxFileImageSizeInBytes) {
			*dst++ = '\0';
			return false;		/* 不正なデータ */
		}
		uint8_t c = src[ofsSrc++];
		if (c == 0x0d /* CR */ || c == 0x0a /* LF */) {
			*dst++ = '\0';
			break;
		}
		if (c < 0x20 && c != 0x09 /* TAB */ && c != 0x1b /* ESC */) {
			*dst++ = '\0';
			return false;		/* 不正なデータ */
		}
		*dst++ = c;
	}

	return true;
}

/* PDX ファイルを要求するか？ */
bool MdxHasPdxFileName(
	const void *mdxFileImage,
	uint32_t mdxFileImageSizeInBytes,
	/* out */ bool *hasPdxFileName
){
	if (mdxFileImage == NULL) return false;				/* 不正な引数 */
	if (mdxFileImageSizeInBytes == 0) return false;		/* 不正な引数 */
	if (hasPdxFileName == NULL) return false;			/* 不正な引数 */

	const uint8_t *src = (const uint8_t *)mdxFileImage;
	uint32_t ofsSrc = 0;

	/* PDX ファイル名文字列までシーク */
	if (
		MdxSeekFileImage(
			mdxFileImage,
			mdxFileImageSizeInBytes,
			MDX_CHUNK_TYPE_PDX_FILE_NAME,
			&ofsSrc
		) == false
	) return false;		/* 不正なデータ */

	/* 0x1a の次の文字が \0 ？ */
	if (src[ofsSrc] == '\0') {
		*hasPdxFileName = false;
		return true;
	}

	*hasPdxFileName = true;
	return true;
}

/* PDX ファイル名の取得 */
bool MdxGetPdxFileName(
	const void *mdxFileImage,
	uint32_t mdxFileImageSizeInBytes,
	/* out */ char *pdxFileNameBuffer,
	size_t pdxFileNameBufferSizeInBytes
){
	if (mdxFileImage == NULL) return false;					/* 不正な引数 */
	if (mdxFileImageSizeInBytes == 0) return false;			/* 不正な引数 */
	if (pdxFileNameBuffer == NULL) return false;			/* 不正な引数 */
	if (pdxFileNameBufferSizeInBytes == 0) return false;	/* 不正な引数 */

	const uint8_t *src = (const uint8_t *)mdxFileImage;
	uint32_t ofsSrc = 0;

	/* PDX ファイル名文字列までシーク */
	if (
		MdxSeekFileImage(
			mdxFileImage,
			mdxFileImageSizeInBytes,
			MDX_CHUNK_TYPE_PDX_FILE_NAME,
			&ofsSrc
		) == false
	) return false;		/* 不正なデータ */

	/* 0x1a の次の文字が \0 ？ */
	if (src[ofsSrc] == '\0') {
		*pdxFileNameBuffer = '\0';
		return false;		/* PDX を持たない */
	}

	/* \0 が見つかるまで PDX ファイル名文字列をコピー */
	uint8_t *dstBegin = (uint8_t *)pdxFileNameBuffer;
	uint8_t *dst = dstBegin;
	uint8_t *dstEnd = (uint8_t *)pdxFileNameBuffer + pdxFileNameBufferSizeInBytes;
	for (;;) {
		if (dst == dstEnd - 1) {
			*dst++ = '\0';
			return false;		/* バッファ容量不足 */
		}
		if (ofsSrc >= mdxFileImageSizeInBytes) {
			*dst++ = '\0';
			return false;		/* 不正なデータ */
		}
		uint8_t c = src[ofsSrc++];
		if (c == '\0') {
			*dst++ = '\0';
			break;
		}
		*dst++ = c;
	}

	/* 拡張子が省略されている場合は修正 */
	size_t length = dst - dstBegin;
	bool hasExt = false;
	if (length >= 5){
		if (
			(
				dstBegin[length-5] == '.'
			&&	(dstBegin[length-4] == 'P' || dstBegin[length-4] == 'p')
			&&	(dstBegin[length-3] == 'D' || dstBegin[length-3] == 'd')
			&&	(dstBegin[length-2] == 'X' || dstBegin[length-2] == 'x')
			&&	dstBegin[length-1] == '\0'
			)
		) {
			hasExt = true;
		}
	}
	if (hasExt == false) {
		const uint8_t *ext = ".PDX";
		dst--;
		for (;;) {
			if (dst == dstEnd - 1) {
				*dst++ = '\0';
				return false;		/* バッファ容量不足 */
			}
			uint8_t c = *ext++;
			if (c == '\0') {
				*dst++ = '\0';
				break;
			}
			*dst++ = c;
		}
	}

	return true;
}

/* 必要なデータバッファサイズを求める */
bool MdxGetRequiredBufferSize(
	const void *mdxFileImage,
	uint32_t mdxFileImageSizeInBytes,
	uint32_t pdxFileImageSizeInBytes,					/* 省略する場合は 0 */
	/* out */ uint32_t *requiredMdxBufferSizeInBytes,	/* 省略する場合は NULL */
	/* out */ uint32_t *requiredPdxBufferSizeInBytes	/* 省略する場合は NULL */
){
	if (mdxFileImage == NULL) return false;				/* 不正な引数 */
	if (mdxFileImageSizeInBytes == 0) return false;		/* 不正な引数 */

	const uint8_t *src = (const uint8_t *)mdxFileImage;
	uint32_t ofsSrc = 0;

	/* MDX バッファのサイズを決定 */
	if (requiredMdxBufferSizeInBytes != NULL) {
		*requiredMdxBufferSizeInBytes = mdxFileImageSizeInBytes + 8;
	}

	/* PDX を持つか？ */
	bool hasPdx;
	if (
		MdxHasPdxFileName(
			mdxFileImage,
			mdxFileImageSizeInBytes,
			&hasPdx
		) == false
	) return false;			/* 不正なデータ */
	if (hasPdx) {
		/* PDX ファイル名文字列までシーク */
		if (
			MdxSeekFileImage(
				mdxFileImage,
				mdxFileImageSizeInBytes,
				MDX_CHUNK_TYPE_PDX_FILE_NAME,
				&ofsSrc
			) == false
		) return false;		/* 不正なデータ */

		/* \0 が見つかるまで PDX ファイル名文字列をスキップ */
		uint32_t pdxFileNameSizeInBytes = 0;
		for (;;) {
			if (ofsSrc >= mdxFileImageSizeInBytes) return false;		/* 不正なデータ */
			uint8_t c = src[ofsSrc++];
			if (c == '\0') break;
			pdxFileNameSizeInBytes++;
		}
		pdxFileNameSizeInBytes++ ;	/* 末端 \0 分 */

		/* PDX BODY のオフセットを決定（2byte アライン）*/
		uint32_t pdxBodyOffset = 8 + pdxFileNameSizeInBytes;
		pdxBodyOffset += pdxBodyOffset & 1;

		/* PDX バッファのサイズを決定 */
		if (requiredPdxBufferSizeInBytes != NULL) {
			*requiredPdxBufferSizeInBytes = pdxBodyOffset + pdxFileImageSizeInBytes;
		}
	} else {
		/* PDX バッファは不要 */
		if (requiredPdxBufferSizeInBytes != NULL) {
			*requiredPdxBufferSizeInBytes = 0;
		}
	}

	return true;
}

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
){
	uint8_t *uint8MdxBuffer = (uint8_t *)mdxBuffer;
	uint8_t *uint8PdxBuffer = (uint8_t *)pdxBuffer;
	const uint8_t *src = (const uint8_t *)mdxFileImage;
	uint32_t ofsSrc = 0;

	/* MDX バッファの作成 */
	{
		/* MDX BODY までシーク */
		if (
			MdxSeekFileImage(
				mdxFileImage,
				mdxFileImageSizeInBytes,
				MDX_CHUNK_TYPE_MDX_BODY,
				&ofsSrc
			) == false
		) return false;		/* 不正なデータ */
		uint32_t mdxBodyOffset = 8 + ofsSrc;

		/* ヘッダ（8 バイト）以降に MDX ファイルイメージをコピー */
		if (mdxFileImageSizeInBytes + 8 > mdxBufferSizeInBytes) return false;	/* バッファサイズ不足 */
		memcpy(
			uint8MdxBuffer + 8,
			mdxFileImage,
			mdxFileImageSizeInBytes
		);

		/* PDX を持つか？ */
		bool hasPdx;
		if (
			MdxHasPdxFileName(
				mdxFileImage,
				mdxFileImageSizeInBytes,
				&hasPdx
			) == false
		) return false;		/* 不正なデータ */

		/* MDX バッファ先頭 8 バイトにヘッダ情報作成 */
		uint8MdxBuffer[0] = 0;
		uint8MdxBuffer[1] = 0;
		uint8MdxBuffer[2] = (hasPdx ? 0 : 0xff);
		uint8MdxBuffer[3] = (hasPdx ? 0 : 0xff);
		uint8MdxBuffer[4] = (uint8_t)(mdxBodyOffset >> 8);
		uint8MdxBuffer[5] = (uint8_t)(mdxBodyOffset);
		uint8MdxBuffer[6] = 0;
		uint8MdxBuffer[7] = 0;
	}

	/* PDX バッファの作成 */
	if (
		pdxFileImage != NULL
	&&	pdxFileImageSizeInBytes != 0
	&&	pdxBuffer != NULL
	&&	pdxBufferSizeInBytes != 0
	) {
		/* PDX ファイル名文字列までシーク */
		if (
			MdxSeekFileImage(
				mdxFileImage,
				mdxFileImageSizeInBytes,
				MDX_CHUNK_TYPE_PDX_FILE_NAME,
				&ofsSrc
			) == false
		) return false;		/* 不正なデータ */

		/* ヘッダ（8 バイト）以降に PDX ファイル名をコピー */
		uint8_t *dst = uint8PdxBuffer;
		uint32_t ofsDst = 8;
		while (ofsDst < pdxBufferSizeInBytes) {
			if (ofsSrc >= mdxFileImageSizeInBytes) {
				dst[ofsDst++] = '\0';
				return false;		/* 不正なデータ */
			}
			uint8_t c = src[ofsSrc++];
			if (c == '\0') {
				dst[ofsDst++] = '\0';
				break;
			}
			dst[ofsDst++] = c;
		}
		uint32_t pdxNameLength = ofsDst - 8;

		/* 2byte アライン */
		if (ofsDst & 1) {
			if (ofsDst >= pdxBufferSizeInBytes) return false;		/* バッファサイズ不足 */
			dst[ofsDst++] = '\0';
		}
		uint32_t pdxBodyOffset = ofsDst;

		/* PDX ファイル名文字列以降に PDX ファイルイメージをコピー */
		if (ofsDst + pdxFileImageSizeInBytes > pdxBufferSizeInBytes) return false;	/* バッファサイズ不足 */
		memcpy(dst + ofsDst, pdxFileImage, pdxFileImageSizeInBytes);

		/* PDX バッファ先頭 8 バイトにヘッダ情報作成 */
		uint8PdxBuffer[0] = 0;
		uint8PdxBuffer[1] = 0;
		uint8PdxBuffer[2] = 0;
		uint8PdxBuffer[3] = 0;
		uint8PdxBuffer[4] = (uint8_t)(pdxBodyOffset >> 8);
		uint8PdxBuffer[5] = (uint8_t)(pdxBodyOffset);
		uint8PdxBuffer[6] = (uint8_t)(pdxNameLength >> 8);
		uint8PdxBuffer[7] = (uint8_t)(pdxNameLength);
	}

	return true;
}
