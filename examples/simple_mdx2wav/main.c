/* Copyright (C) 2018 Yosshin(@yosshin4004) */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#ifdef _WIN32
	#include <Windows.h>
#else
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
	int wcharFileNameSize = MultiByteToWideChar(CP_UTF8, 0, fileName, -1, NULL, 0);
	wchar_t *wcharFileName = calloc(wcharFileNameSize, sizeof(wchar_t));
	if (MultiByteToWideChar(CP_UTF8, 0, fileName, -1, wcharFileName, wcharFileNameSize) == 0) {
		free(wcharFileName);
		return NULL;
	}
	FILE *fd; _wfopen_s(&fd, wcharFileName, L"rb");
	free(wcharFileName);
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
	int wcharFileNameSize = MultiByteToWideChar(CP_UTF8, 0, fileName, -1, NULL, 0);
	wchar_t *wcharFileName = calloc(wcharFileNameSize, sizeof(wchar_t));
	if (MultiByteToWideChar(CP_UTF8, 0, fileName, -1, wcharFileName, wcharFileNameSize) == 0) {
		free(wcharFileName);
		return false;
	}
	FILE *fd; _wfopen_s(&fd, wcharFileName, L"wb");
	free(wcharFileName);
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
		uint16_t	bitsPerSampleComponent;
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

int main(
	int		argc,
	char	**argv
){
	char *mdxFilePath = NULL;
	char *wavFilePath = NULL;

	/* 引数解析 */
	if (argc == 1) {
		printf(
			"Simple mdx -> wav converter\n"
			"usage:\n"
			"	%s [options]\n"
			"option:\n"
			"	-i <mdxfilepath>:\n"
			"		Specify a input mdx filepath.\n"
			"	-o <wavfilepath>:\n"
			"		Specify a output wav filepath.\n",
			argv[0]
		);
		exit(EXIT_SUCCESS);
	} else {
		int i = 1;
		while (i < argc) {
			if (argv[i][0] == '-') {
				if (strcmp(argv[i], "-i") == 0) {
					i++;
					if (i >= argc) {
						printf("ERROR : No arg for '%s'.\n", argv[i - 1]);
						return EXIT_FAILURE;
					}
					mdxFilePath = argv[i];
				} else
				if (strcmp(argv[i], "-o") == 0) {
					i++;
					if (i >= argc) {
						printf("ERROR : No arg for '%s'.\n", argv[i - 1]);
						return EXIT_FAILURE;
					}
					wavFilePath = argv[i];
				} else {
					printf("ERROR : Invalid arg '%s'.\n", argv[i]);
					return EXIT_FAILURE;
				}
			} else {
				printf("ERROR : Invalid arg '%s'.\n", argv[i]);
				return EXIT_FAILURE;
			}
			i++;
		}
	}

	/* 引数エラーチェック */
	if (mdxFilePath == NULL) {
		printf("ERROR : Please specify a input mdx filepath.\n");
		exit(EXIT_FAILURE);
	}
	if (wavFilePath == NULL) {
		printf("ERROR : Please specify a output wav filepath.\n");
		exit(EXIT_FAILURE);
	}

	/* MDX ファイルパスが "" で括られている場合の補正 */
	size_t len = strlen(mdxFilePath);
	if (len > 0) {
		if (mdxFilePath[0] == '\"' && mdxFilePath[len - 1] == '\"') {
			mdxFilePath[len - 1] = '\0';
			mdxFilePath++;
		}
	}

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

	/* PDX ファイルを要求するか？ */
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
		char pdxFileName[FILENAME_MAX] = {0};
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

#ifdef _WIN32
		char mdxDirName[FILENAME_MAX];
		_splitpath_s(mdxFilePath, NULL, 0, mdxDirName, sizeof(mdxDirName), NULL, 0, NULL, 0);

		char pdxFilePath[FILENAME_MAX];
		sprintf_s(pdxFilePath, sizeof(pdxFilePath), "%s%s", mdxDirName, pdxFileName);
		printf("read %s ... ", pdxFilePath);
		pdxFileImage = mallocReadFile(pdxFilePath, &pdxFileImageSizeInBytes);
		if (pdxFileImage != NULL) {
			printf("scceeded.\n");
		} else {
			printf("failed.\n");
		}
#else
		const char *mdxDirName = dirname(mdxFilePath);

		/*
			ファイル名の大文字小文字が区別される環境では
				大文字ファイル名 + 大文字拡張子
				大文字ファイル名 + 小文字拡張子
				小文字ファイル名 + 大文字拡張子
				小文字ファイル名 + 小文字拡張子
			の 4 通りで PDX ファイル読み込みを試す必要がある。
		*/
		for (int retryCount = 0; retryCount < 4; retryCount++) {
			char modifiedPdxFileName[FILENAME_MAX];
			memcpy(modifiedPdxFileName, pdxFileName, FILENAME_MAX);
			if (retryCount & 1) {
				/* ファイル名部分の大文字小文字反転 */
				for (char *p = modifiedPdxFileName; *p != '\0' && *p != '.'; p++) {
					if ('a' <= *p && *p <= 'z' || 'A' <= *p && *p <= 'Z') *p ^= 0x20;
				}
			}
			if (retryCount & 2) {
				/* 拡張子部分の大文字小文字反転 */
				char *p = modifiedPdxFileName;
				while (strchr(p, '.') != NULL) p = strchr(p, '.') + 1;
				for (; *p != '\0'; p++) {
					if ('a' <= *p && *p <= 'z' || 'A' <= *p && *p <= 'Z') *p ^= 0x20;
				}
			}

			char pdxFilePath[FILENAME_MAX];
			sprintf(pdxFilePath, "%s/%s", mdxDirName, modifiedPdxFileName);
			printf("read %s ... ", pdxFilePath);
			pdxFileImage = mallocReadFile(pdxFilePath, &pdxFileImageSizeInBytes);
			if (pdxFileImage != NULL) {
				printf("scceeded.\n");
				break;
			} else {
				printf("failed.\n");
			}
		}
#endif
	}

	/* MDX PDX バッファの要求サイズを求める */
	uint32_t mdxBufferSizeInBytes = 0;
	uint32_t pdxBufferSizeInBytes = 0;
	if (
		MdxGetRequiredBufferSize(
			mdxFileImage,
			mdxFileImageSizeInBytes, pdxFileImageSizeInBytes,
			&mdxBufferSizeInBytes, &pdxBufferSizeInBytes
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

	/* この時点でファイルイメージは破棄してよい */
	if (pdxFileImage != NULL) free(pdxFileImage);
	free(mdxFileImage);

	/* コンテキストの初期化 */
	#define MDX_BUFFER_SIZE		1 * 1024 * 1024
	#define PDX_BUFFER_SIZE		2 * 1024 * 1024
	#define MEMORY_POOL_SIZE	8 * 1024 * 1024
	MxdrvContext context;
	if (MxdrvContext_Initialize(&context, MEMORY_POOL_SIZE) == false) {
		printf("MxdrvContext_Initialize failed.\n");
		exit(EXIT_FAILURE);
	}

	/* MXDRV の初期化 */
	#define NUM_SAMPLES_PER_SEC 48000
	{
		int ret = MXDRV_Start(
			&context,
			NUM_SAMPLES_PER_SEC,
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
	float songDurationInSeconds = MXDRV_MeasurePlayTime(
		&context,
		mdxBuffer, mdxBufferSizeInBytes,
		pdxBuffer, pdxBufferSizeInBytes,
		1, 0
	) / 1000.0f;
	printf("song duration %.1f(sec)\n", songDurationInSeconds);

	/* MDX 再生 */
	MXDRV_Play(
		&context,
		mdxBuffer, mdxBufferSizeInBytes,
		pdxBuffer, pdxBufferSizeInBytes
	);

	/* wav 保存バッファの確保 */
	int numSamples = (int)(NUM_SAMPLES_PER_SEC * songDurationInSeconds);
	int numChannels = 2;
	int wavBufferSizeInBytes = sizeof(int16_t) * numSamples * numChannels;
	int16_t *wavBuffer = (int16_t *)malloc(wavBufferSizeInBytes);
	if (wavBuffer == NULL) {
		printf("malloc wavBuffer(%d bytes) failed.\n", wavBufferSizeInBytes);
		exit(EXIT_FAILURE);
	}

	/* wav 保存バッファにデコード */
	MXDRV_GetPCM(&context, wavBuffer, numSamples);

	/* wav ファイルに保存 */
	if (
		serializeAsWav(
			wavFilePath,
			wavBuffer,
			numChannels,
			numSamples,
			NUM_SAMPLES_PER_SEC,
			1 /* WAVE_FORMAT_PCM */,
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

