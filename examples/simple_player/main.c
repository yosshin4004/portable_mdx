/* Copyright (C) 2018 Yosshin(@yosshin4004) */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>

#include <mdx_util.h>
#include <mxdrv.h>
#include <mxdrv_context.h>

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
	#include <libgen.h>		/* for dirname() */
#endif

/* メモリ確保した領域にファイルを読み込む */
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

/*
	windows 環境の場合、SDL AUDIO コールバックに与えられた処理時間が非常に短い。
	コールバック内で MDX デコードを行うと、処理時間オーバーによりノイズが発生
	することがある。
	この問題を回避するため、MDX デコードスレッドを作成し、コールバックに先行
	してデコードを完了させておき、コールバック内ではデコード結果を出力バッファ
	にコピーするのみの短時間な処理を行うようにする。
*/

/* SDL AUDIO コールバック 1 回あたりの出力サンプル数 */
#define NUM_SDL_AUDIO_CALLBACK_SAMPLES 512

/* オーディオ出力ダブルバッファ */
static int16_t s_audioDoubleBuffer[2][NUM_SDL_AUDIO_CALLBACK_SAMPLES * 2];

/* オーディオ出力ダブルバッファの排他制御用セマフォ */
static SDL_sem *s_audioReadableSem = NULL;
static SDL_sem *s_audioWritableSem = NULL;

/* SDL AUDIO コールバック処理 */
static void sdlAudioCallback(
	void	*userdata,
	uint8_t	*stream,
	int		len
){
	assert(len == NUM_SDL_AUDIO_CALLBACK_SAMPLES * sizeof(int16_t) * 2);
	static int s_audioReadBufferIndex = 0;
	SDL_SemWait(s_audioReadableSem);
	memcpy(stream, &s_audioDoubleBuffer[s_audioReadBufferIndex][0], len);
	s_audioReadBufferIndex ^= 1;
	SDL_SemPost(s_audioWritableSem);
}

/* MDX デコードスレッド */
static SDL_Thread *s_mdxDecodeThread = NULL;
static int mdxDecodeThread(void *arg){
	static int s_audioWriteBufferIndex = 0;
	for (;;) {
		SDL_SemWait(s_audioWritableSem);
		MXDRV_GetPCM(
			(MxdrvContext *)arg,
			&s_audioDoubleBuffer[s_audioWriteBufferIndex][0],
			NUM_SDL_AUDIO_CALLBACK_SAMPLES
		);
		s_audioWriteBufferIndex ^= 1;
		SDL_SemPost(s_audioReadableSem);
	}
}

/* MDX デコードスレッドの初期化と開始 */
static void startMdxDecodeThread(MxdrvContext *context){
	s_audioReadableSem = SDL_CreateSemaphore(0);
	s_audioWritableSem = SDL_CreateSemaphore(2);
	s_mdxDecodeThread = SDL_CreateThread(mdxDecodeThread, "mdxDecodeThread", context);
}

/* メイン */
int main(
	int		argc,
	char	**argv
){
	char *mdxFilePath = NULL;		/* 再生対象のファイルパス */
	int maxLoops = 0;				/* ループ数上限。0 は無制限を意味する */
	float maxDuration = 0.0f;		/* 再生時間（秒）上限。0 は無制限を意味する */
	bool enableFadeout = true;		/* ループ数か再生時間上限に達したらフェードアウトするか？ */
	bool enableVisualizer = true;	/* ビジュアライザを利用するか？ */
	float quitTimeInSeconds = 0.0f;
	bool fadeouted = false;

	/* 引数解析 */
	if (argc == 1) {
		printf(
			"Simple mdx player\n"
			"usage:\n"
			"	%s [options] <mdxfilename>\n"
			"option:\n"
			"	-maxLoops <number>\n"
			"		Specify the maximum number of loops.\n"
			"		0 means infinite.\n"
			"	-maxDuration <seconds>\n"
			"		Specify the maximum playback length in seconds.\n"
			"		0 means infinite.\n"
			"	-noFadeout\n"
			"		Disable fadeout.\n"
			"	-noVisualize\n"
			"		Disable visualizer.\n",
			argv[0]
		);
		exit(EXIT_SUCCESS);
	} else {
		int i = 1;
		while (i < argc) {
			if (argv[i][0] == '-') {
				if (strcmp(argv[i], "-maxLoops") == 0) {
					i++;
					if (i >= argc) {
						printf("ERROR : No arg for '%s'.\n", argv[i - 1]);
						return EXIT_FAILURE;
					}
					char *endptr = NULL;
					maxLoops = strtol(argv[i], &endptr, 0);
					if (*endptr != '\0') {
						printf("ERROR : Invalid arg for '%s'.\n", argv[i - 1]);
						return EXIT_FAILURE;
					}
				} else
				if (strcmp(argv[i], "-maxDuration") == 0) {
					i++;
					if (i >= argc) {
						printf("ERROR : No arg for '%s'.\n", argv[i - 1]);
						return EXIT_FAILURE;
					}
					char *endptr = NULL;
					maxDuration = strtof(argv[i], &endptr);
					if (*endptr != '\0') {
						printf("ERROR : Invalid arg for '%s'.\n", argv[i - 1]);
						return EXIT_FAILURE;
					}
				} else
				if (strcmp(argv[i], "-noFadeout") == 0) {
					enableFadeout = false;
				} else
				if (strcmp(argv[i], "-noVisualize") == 0) {
					enableVisualizer = false;
				} else {
					printf("ERROR : Invalid arg '%s'.\n", argv[i]);
					return EXIT_FAILURE;
				}
			} else {
				if (mdxFilePath != NULL) {
					printf("ERROR : '%s' is already specified as a mdxfilename.\n", mdxFilePath);
					return EXIT_FAILURE;
				}
				mdxFilePath = argv[i];
			}
			i++;
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
			printf("mallocReadFile '%s' failed.\n", pdxFilePath);
			exit(EXIT_FAILURE);
		}
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
	#define MEMORY_POOL_SIZE	4 * 1024 * 1024
	MxdrvContext context;
	if (MxdrvContext_Initialize(&context, MEMORY_POOL_SIZE) == false) {
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
	{
		float songDurationInSeconds = MXDRV_MeasurePlayTime(
			&context,
			mdxBuffer, mdxBufferSizeInBytes,
			pdxBuffer, pdxBufferSizeInBytes,
			1, 0
		) / 1000.0f;
		printf("song duration %.1f(sec)\n", songDurationInSeconds);
	}

	/* MDX 再生 */
	MXDRV_Play(
		&context,
		mdxBuffer, mdxBufferSizeInBytes,
		pdxBuffer, pdxBufferSizeInBytes
	);

	/* MDX デコードスレッドの初期化と開始 */
	startMdxDecodeThread(&context);

	/* SDL 初期化 */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init(SDL_INIT_EVERYTHING) failed: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	/* メインウィンドウ作成 */
	#define WINDOW_WIDTH	512
	#define WINDOW_HEIGHT	512
	SDL_Window *window = NULL;
	SDL_Surface *surface = NULL;
	if (enableVisualizer) {
		window = SDL_CreateWindow(
			"Simple mdx player",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			SDL_WINDOW_FULLSCREEN_DESKTOP * 0);
		if (window == NULL) {
			printf("SDL_CreateWindow() failed: %s", SDL_GetError());
			exit(EXIT_FAILURE);
		}
		surface = SDL_GetWindowSurface(window);
	}

	/* SDL AUDIO 初期化 */
	{
		SDL_AudioSpec fmt;
		memset(&fmt, 0, sizeof(fmt));
		fmt.freq		= SAMPLES_PER_SEC;
		fmt.format		= AUDIO_S16SYS;
		fmt.channels	= 2;
		fmt.samples		= NUM_SDL_AUDIO_CALLBACK_SAMPLES;
		fmt.callback	= sdlAudioCallback;
		fmt.userdata	= &context;
		if (SDL_OpenAudio(&fmt, NULL) < 0) {
			printf("SDL_OpenAudio() failed: %s", SDL_GetError());
			exit(EXIT_FAILURE);
		}
		SDL_PauseAudio(0);
	}

	/* メッセージループ */
	bool quitMessageLoop = false;
	while (quitMessageLoop == false) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN: {
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						quitMessageLoop = true;
					}
				} break;
				case SDL_QUIT: {
					quitMessageLoop = true;
				} break;
			}
		}

		/* ビジュアライザ */
		if (enableVisualizer) {
			/* 画面消去 */
			SDL_FillRect(surface, NULL, 0);

			/* OPM レジスタの可視化 */
			{
				static int levelMeters[256];
				#define NUM_PIXELS_PER_COLUMN	64
				#define NUM_PIXELS_PER_ROW		8
				#define NUM_PIXELS_PER_BIT		7
				int regIndex, bit;
				for (regIndex = 0; regIndex < 256; regIndex++) {
					uint8_t regVal = 0;
					bool updated = false;
					MxdrvContext_GetOpmReg(&context, regIndex, &regVal, &updated);
					for (bit = 0; bit < 8; bit++) {
						int x = (regIndex & 7) * NUM_PIXELS_PER_COLUMN + bit * NUM_PIXELS_PER_BIT;
						int y = (regIndex / 8) * NUM_PIXELS_PER_ROW;
						SDL_Rect rect = {x, y, NUM_PIXELS_PER_BIT - 1, NUM_PIXELS_PER_ROW - 1};
						uint32_t color = 0x202020;
						levelMeters[regIndex] = levelMeters[regIndex] * 15 / 16;
						if (updated) levelMeters[regIndex] = 0x80;
						if (regVal & (1 << bit)) color = 0x70607F;
						color += levelMeters[regIndex] * 0x010101;
						SDL_FillRect(surface, &rect, color);
					}
				}
			}

			/* KEY の可視化 */
			static int keyOnLevelMeters[16] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
			static int keyOffLevelMeters[16] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
			{
				#define KEY_DISPLAY_X	32
				#define KEY_DISPLAY_Y	256
				#define KEY_WIDTH		5
				#define KEY_HEIGHT		16
				int i;
				const MXWORK_CH *fmChannels = (MXWORK_CH *)MXDRV_GetWork(&context, MXDRV_WORK_FM);
				const MXWORK_CH *pcmChannels = (MXWORK_CH *)MXDRV_GetWork(&context, MXDRV_WORK_PCM);

				/* FM 0~7 */
				for (i = 0; i < 8; i++) {
					int key = (fmChannels[i].S0012 + 27) / 64;
					int volume = (fmChannels[i].S0022);
					if (volume & 0x80) {
						volume = (0x7F - (volume & 0x7F)) * 2;
					} else {
						volume = (volume & 0xF) * 0x11;
					}

					if (fmChannels[i].S0016 & (1 << 3)) {
						{
							int x = key * KEY_WIDTH + ((fmChannels[i].S0014 - fmChannels[i].S0012) * KEY_WIDTH / 64);
							int y = i * KEY_HEIGHT;
							SDL_Rect rect = {x + KEY_DISPLAY_X, y + KEY_DISPLAY_Y, KEY_WIDTH, KEY_HEIGHT - 1};
							uint32_t color = 0x008000;
							SDL_FillRect(surface, &rect, color);
						}
						{
							int x = key * KEY_WIDTH;
							int y = i * KEY_HEIGHT;
							SDL_Rect rect = {x + KEY_DISPLAY_X, y + KEY_DISPLAY_Y, KEY_WIDTH, KEY_HEIGHT - 1};
							uint32_t color = 0x0000FF | (volume * 0x010100);
							SDL_FillRect(surface, &rect, color);
						}
					}

					bool currentKeyOn = false;
					bool logicalSumOfKeyOn = false;
					MxdrvContext_GetFmKeyOn(&context, i, &currentKeyOn, &logicalSumOfKeyOn);
					if (currentKeyOn == false) {
						keyOffLevelMeters[i] = keyOffLevelMeters[i] * 127 / 128;
					}
					if (logicalSumOfKeyOn) {
						keyOnLevelMeters[i] = volume;
						keyOffLevelMeters[i] = volume;
					}
				}

				/* PCM 0~7 */
				for (i = 8; i < 16; i++) {
					const MXWORK_CH *pcmChannel;
					if (i == 8) {
						pcmChannel = &fmChannels[8];
					} else {
						pcmChannel = &pcmChannels[i - 9];
					}

					int key = (pcmChannel->S0012 + 27) / 64;
					int volume = (pcmChannel->S0022);
					if (volume & 0x80) {
						volume = (0x7F - (volume & 0x7F)) * 2;
					} else {
						volume = (volume & 0xF) * 0x11;
					}

					if (pcmChannel->S0016 & (1 << 3)) {
						int x = key * KEY_WIDTH;
						int y = i * KEY_HEIGHT;
						SDL_Rect rect = {x + KEY_DISPLAY_X, y + KEY_DISPLAY_Y, KEY_WIDTH, KEY_HEIGHT - 1};
						uint32_t color = 0xFF0000 | (volume * 0x000101);
						SDL_FillRect(surface, &rect, color);
					}

					bool logicalSumOfKeyOn = false;
					MxdrvContext_GetPcmKeyOn(&context, i - 8, &logicalSumOfKeyOn);
					if (logicalSumOfKeyOn) {
						keyOnLevelMeters[i] = volume;
						keyOffLevelMeters[i] = volume;
					}
				}

				#define LEVEL_METER_DISPLAY_X	0
				#define LEVEL_METER_DISPLAY_Y	256
				#define LEVEL_METER_WIDTH		32
				#define LEVEL_METER_HEIGHT		16
				for (i = 0; i < 16; i++) {
					{
						int x = 0;
						int y = i * LEVEL_METER_HEIGHT;
						int w = keyOffLevelMeters[i] * LEVEL_METER_WIDTH / 0xFF;
						SDL_Rect rect = {x + LEVEL_METER_DISPLAY_X, y + LEVEL_METER_DISPLAY_Y, w, LEVEL_METER_HEIGHT - 1};
						uint32_t color = 0x404040;
						SDL_FillRect(surface, &rect, color);
					}
					{
						int x = 0;
						int y = i * LEVEL_METER_HEIGHT;
						int w = keyOnLevelMeters[i] * LEVEL_METER_WIDTH / 0xFF;
						SDL_Rect rect = {x + LEVEL_METER_DISPLAY_X, y + LEVEL_METER_DISPLAY_Y, w, LEVEL_METER_HEIGHT - 1};
						uint32_t color = 0xFFFFFF;
						SDL_FillRect(surface, &rect, color);
					}
					keyOnLevelMeters[i] = keyOnLevelMeters[i] * 31 / 32;
				}
			}

			/* 画面更新 */
			SDL_UpdateWindowSurface(window);
			SDL_Delay(10);
		}

		/* フェードアウトおよび再生完了処理 */
		{
			const MXWORK_GLOBAL *global = (MXWORK_GLOBAL *)MXDRV_GetWork(&context, MXDRV_WORK_GLOBAL);
			float elapsedTimeInSeconds = (global->PLAYTIME * 1024LL / 4000) / 1000.0f;
			int numLoops = global->L002246;
			int finished = global->L001e13;

			/* ループ数上限を超えるか再生時間上限を超えたらフェードアウト開始 */
			if (finished == false) {
				if ((maxLoops != 0 && numLoops >= maxLoops)
				||	(maxDuration != 0.0f && elapsedTimeInSeconds > maxDuration)
				) {
					if (enableFadeout) {
						if (fadeouted == false) {
							MXDRV_Fadeout(&context);
							printf("fadeout...\n");
							fadeouted = true;
						}
					} else {
						break;
					}
				}
			}

			/* 再生完了を検出したら終了 */
			if (finished) {
				/* フェードアウト後の再生完了の場合は直ちに終了 */
				if (fadeouted) break;

				/* フェードアウト無しの再生完了の場合は 2 秒待って終了 */
				if (fadeouted == false) {
					if (quitTimeInSeconds == 0.0f) {
						printf("finish.\n");
						quitTimeInSeconds = elapsedTimeInSeconds + 2.0f;
					} else {
						if (elapsedTimeInSeconds > quitTimeInSeconds) break;
					}
				}
			}
		}
	}

	/* 終了処理 */
	printf("done.\n");
	if (window != NULL) SDL_DestroyWindow(window);
	SDL_Quit();
	MXDRV_End(&context);
	MxdrvContext_Terminate(&context);
	if (pdxBuffer != NULL) free(pdxBuffer);
	free(mdxBuffer);

	exit(EXIT_SUCCESS);
}

