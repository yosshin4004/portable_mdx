/* Copyright (C) m_puusan */
/* Copyright (C) 2018 Yosshin(@yosshin4004) */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#ifndef _WIN32
	#include <libgen.h>		/* for dirname() */
#endif
#include <string.h>

#include <mdx_util.h>
#include <mxdrv.h>
#include <mxdrv_context.h>

void *mallocReadFile(
	const char *fileName,
	uint32_t *sizeRet
){
#ifdef _WIN32
	FILE *fd; fopen_s(&fd, fileName, "rb");
#else
	FILE *fd = fopen(fileName, "rb");
#endif
	if (fd == NULL) return NULL;
	struct stat stbuf;
#ifdef _WIN32
	if (fstat(_fileno(fd), &stbuf) == -1) {
#else
	if (fstat(fileno(fd), &stbuf) == -1) {
#endif
		fclose(fd);
		return NULL;
	}
	assert(stbuf.st_size < 0x100000000LL);
	uint32_t size = (uint32_t)stbuf.st_size;
	void *buffer = malloc(size);
	if (buffer == NULL) {
		fclose(fd);
		return NULL;
	}
	fread(buffer, 1, size, fd);
	*sizeRet = size;
	return buffer;
}

bool serializeAsWav(
	const char *fileName,
	const void *buffer,
	int numChannels,
	int numSamples,
	int samplesPerSec,
	uint16_t formatID,
	int	bitsPerSampleComponent
){
#ifdef _WIN32
	FILE *fd; fopen_s(&fd, fileName, "wb");
#else
	FILE *fd = fopen(fileName, "wb");
#endif
	if (fd == NULL) return false;

	int	bytesPerSampleComponent = bitsPerSampleComponent >> 3;
	int	dataChunkSizeInBytes = bytesPerSampleComponent * numChannels * numSamples;

	typedef struct {
		uint8_t		chunkName[4];
		uint32_t	chunkSize;
	} WavChunkHeader;
	typedef struct {
		uint8_t		riff[4];
		uint32_t	fileSizeMinus8;
		uint8_t		wave[4];
	} WavHeader;
	typedef struct {
		uint16_t	formatID;
		uint16_t	channelCount;
		uint32_t	samplesPerSec;
		uint32_t	bytesPerSec;
		uint16_t	bytesPerSample;
		uint16_t	bitsPerSample;
	} WavFormatChunk;
	typedef struct {
		WavHeader		wavHeader;
		WavChunkHeader	formatChunkHeader;
		WavFormatChunk	formatChunk;
		WavChunkHeader	dataChunkHeader;
	} Header;
	Header header = {
		{
			{'R' , 'I', 'F', 'F'},
			(uint32_t)(sizeof(header) + dataChunkSizeInBytes - 8),
			{'W', 'A', 'V', 'E'}
		},{
			{'f', 'm', 't', ' '},
			(uint32_t)sizeof(WavFormatChunk)
		},{
			formatID,
			(uint16_t)numChannels,
			(uint32_t)samplesPerSec,
			(uint32_t)(bytesPerSampleComponent * numChannels * samplesPerSec),
			(uint16_t)(bytesPerSampleComponent * numChannels),
			(uint16_t)bitsPerSampleComponent
		},{
			{'d', 'a', 't', 'a'},
			(uint32_t)dataChunkSizeInBytes
		}
	};

	fwrite(&header, 1, sizeof(header), fd);
	fwrite(buffer, 1, bytesPerSampleComponent * numChannels * numSamples, fd);
	fclose(fd);
	return true;
}

int
main(
	int		argc
,	char	**argv
){
	/* 引数解析 */
	if (argc == 1) {
		printf(
			"mdx -> wav converter\n"
			"usage : %s mdxfilename\n",
			argv[0]
		);
		exit(EXIT_SUCCESS);
	}
	char *mdxFilePath = argv[1];

	/* MDX ファイルの読み込み */
	uint32_t mdxFileImageSizeInBytes = 0;
	void *mdxFileImage = mallocReadFile(mdxFilePath, &mdxFileImageSizeInBytes);
	if (mdxFileImage == NULL) {
		printf("mallocReadFile '%s' failed.\n", mdxFilePath);
		exit(EXIT_FAILURE);
	}

	/* MDX タイトルの取得 */
	char mdxTitle[256];
	if (
		MdxGetTitle(
			mdxFileImage, mdxFileImageSizeInBytes,
			mdxTitle, sizeof(mdxTitle)
		) == false
	) {
		printf("MdxGetTitle failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("mdx title = %s\n", mdxTitle);

	/* PDX ファイルを要求するか */
	bool hasPdx;
	if (
		MdxHasPdxFileName(
			mdxFileImage, mdxFileImageSizeInBytes,
			&hasPdx
		) == false
	) {
		printf("MdxHasPdxFileName failed.\n");
		exit(EXIT_FAILURE);
	}

	/* PDX ファイルの読み込み */
	uint32_t pdxFileImageSizeInBytes = 0;
	void *pdxFileImage = NULL;
	if (hasPdx) {
		char pdxFileName[FILENAME_MAX];
		if (
			MdxGetPdxFileName(
				mdxFileImage, mdxFileImageSizeInBytes,
				pdxFileName, sizeof(pdxFileName)
			) == false
		) {
			printf("MdxGetPdxFileName failed.\n");
			exit(EXIT_FAILURE);
		}
		printf("pdx filename = %s\n", pdxFileName);

		char pdxFilePath[FILENAME_MAX];
#ifdef _WIN32
		char mdxDirName[FILENAME_MAX];
		_splitpath_s(mdxFilePath, NULL, 0, mdxDirName, sizeof(mdxDirName), NULL, 0, NULL, 0);
		sprintf_s(pdxFilePath, sizeof(pdxFilePath), "%s%s", mdxDirName, pdxFileName);
#else
		sprintf(pdxFilePath, "%s/%s", dirname(mdxFilePath), pdxFileName);
#endif
		printf("pdx filepath = %s\n", pdxFilePath);

		pdxFileImage = mallocReadFile(pdxFilePath, &pdxFileImageSizeInBytes);
		if (pdxFileImage == NULL) {
			printf("mallocReadFile '%s' failed.\n", mdxFilePath);
			exit(EXIT_FAILURE);
		}
	}

	/* MDX PDX バッファの要求サイズを求める */
	uint32_t mdxBufferSizeInBytes = 0;
	uint32_t pdxBufferSizeInBytes = 0;
	if (
		MdxGetRequiredBufferSize(
			mdxFileImage,
			mdxFileImageSizeInBytes,
			pdxFileImageSizeInBytes,
			&mdxBufferSizeInBytes,
			&pdxBufferSizeInBytes
		) == false
	) {
		printf("MdxGetRequiredBufferSize failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("mdxBufferSizeInBytes = %d\n", mdxBufferSizeInBytes);
	printf("pdxBufferSizeInBytes = %d\n", pdxBufferSizeInBytes);

	/* MDX PDX バッファの確保 */
	void *mdxBuffer = NULL;
	mdxBuffer = (uint8_t *)malloc(mdxBufferSizeInBytes);
	if (mdxBuffer == NULL) {
		printf("malloc mdxBuffer failed.\n");
		exit(EXIT_FAILURE);
	}
	void *pdxBuffer = NULL;
	if (hasPdx) {
		pdxBuffer = (uint8_t *)malloc(pdxBufferSizeInBytes);
		if (pdxBuffer == NULL) {
			printf("malloc pdxBuffer failed.\n");
			exit(EXIT_FAILURE);
		}
	}

	/* MDX PDX バッファを作成 */
	if (
		MdxUtilCreateMdxPdxBuffer(
			mdxFileImage, mdxFileImageSizeInBytes,
			pdxFileImage, pdxFileImageSizeInBytes,
			mdxBuffer, mdxBufferSizeInBytes,
			pdxBuffer, pdxBufferSizeInBytes
		) == false
	) {
		printf("MdxUtilCreateMdxPdxBuffer failed.\n");
		exit(EXIT_FAILURE);
	}

	/* この時点で、ファイルイメージは破棄してよい */
	if (pdxFileImage != NULL) free(pdxFileImage);
	free(mdxFileImage);

	/* コンテキストの初期化 */
	#define MDX_BUFFER_SIZE		1 * 1024 * 1024
	#define PDX_BUFFER_SIZE		2 * 1024 * 1024
	#define MEMORY_POOL_SIZE	4 * 1024 * 1024
	MxdrvContext context;
	if (
		MxdrvContext_Initialize(
			&context,
			MEMORY_POOL_SIZE
		) == false
	) {
		printf("MxdrvContext_Initialize failed.\n");
		exit(EXIT_FAILURE);
	}

	/* MXDRV の初期化 */
	#define SAMPLES_PER_SEC 48000
	{
		int ret = MXDRV_Start(
			&context,
			SAMPLES_PER_SEC,
			0, 0, 0,
			MDX_BUFFER_SIZE,
			PDX_BUFFER_SIZE,
			0
		);
		if (ret != 0) {
			printf("MXDRV_Start failed. return code = %d\n", ret);
			exit(EXIT_FAILURE);
		}
	}

	/* PCM8 を有効化 */
	uint8_t *pcm8EnableFlag = (uint8_t *)MXDRV_GetWork(&context, MXDRV_WORK_PCM8);
	*(pcm8EnableFlag) = 1;

	/* 音量設定 */
	MXDRV_TotalVolume(&context, 256);

	/* 再生時間を求める */
	int loop = 1;
	int fadeout = 0;
	float songDuration = MXDRV_MeasurePlayTime(
		&context,
		mdxBuffer, mdxBufferSizeInBytes,
		pdxBuffer, pdxBufferSizeInBytes,
		loop,
		fadeout
	) / 1000.0f;
	printf("songDuration %.1f(sec)\n", songDuration);

	/* MDX 再生 */
	MXDRV_Play(
		&context,
		mdxBuffer, mdxBufferSizeInBytes,
		pdxBuffer, pdxBufferSizeInBytes
	);

	/* wav 保存バッファの確保 */
	int numSamples = (int)(SAMPLES_PER_SEC * songDuration);
	int numChannels = 2;
	int wavBufferSizeInBytes = sizeof(int16_t) * numSamples * numChannels;
	int16_t *wavBuffer = (int16_t *)malloc(wavBufferSizeInBytes);
	if (wavBuffer == NULL) {
		printf("malloc wavBuffer(%d bytes) failed.\n", wavBufferSizeInBytes);
		exit(EXIT_FAILURE);
	}

	/* wav 保存バッファにレンダリング */
	MXDRV_GetPCM(
		&context,
		wavBuffer,
		numSamples
	);

	/* wav ファイルに保存 */
	if (
		serializeAsWav(
			"out.wav",
			wavBuffer,
			numChannels,
			numSamples,
			SAMPLES_PER_SEC,
			1 /*WAVE_FORMAT_PCM*/,
			16
		) == false
	) {
		printf("serializeAsWav failed.\n");
		exit(EXIT_FAILURE);
	}

	/* 終了処理 */
	free(wavBuffer);
	MXDRV_End(&context);
	MxdrvContext_Terminate(&context);
	if (pdxBuffer != NULL) free(pdxBuffer);
	free(mdxBuffer);

	exit(EXIT_SUCCESS);
}

