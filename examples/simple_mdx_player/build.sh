#!/bin/sh

mkdir -p objs/
g++ -c -std=c++11 -I../../include ../../src/mxdrv/mxdrv.cpp -o objs/mxdrv.o
g++ -c -std=c++11 -I../../include ../../src/mxdrv/mxdrv_context.cpp -o objs/mxdrv_context.o
g++ -c -std=c++11 -I../../include ../../src/mxdrv/sound_iocs.cpp -o objs/sound_iocs.o
g++ -c -std=c++11 -I../../include ../../src/x68sound/x68sound_adpcm.cpp -o objs/x68sound_adpcm.o
g++ -c -std=c++11 -I../../include ../../src/x68sound/x68sound_lfo.cpp -o objs/x68sound_lfo.o
g++ -c -std=c++11 -I../../include ../../src/x68sound/x68sound_op.cpp -o objs/x68sound_op.o
g++ -c -std=c++11 -I../../include ../../src/x68sound/x68sound_opm.cpp -o objs/x68sound_opm.o
g++ -c -std=c++11 -I../../include ../../src/x68sound/x68sound_pcm8.cpp -o objs/x68sound_pcm8.o
g++ -c -std=c++11 -I../../include ../../src/x68sound/x68sound.cpp -o objs/x68sound.o
g++ -c -std=c++11 -I../../include ../../src/x68sound/x68sound_context.cpp -o objs/x68sound_context.o
gcc -c -I../../include ../../src/mdx_util.c -o objs/mdx_util.o
gcc -c -I../../include main.c -o objs/main.o

g++ objs/main.o objs/mxdrv.o objs/mxdrv_context.o objs/sound_iocs.o objs/x68sound_adpcm.o objs/x68sound_lfo.o objs/x68sound_op.o objs/x68sound_opm.o objs/x68sound_pcm8.o objs/x68sound.o objs/x68sound_context.o objs/mdx_util.o -lSDL2 -o simple_mdx_player
