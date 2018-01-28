#
# Makefile
#

CXX = g++
CC = gcc
STRIP = strip

TARGET_DIR = objs
TARGET = simple_mdx2wav

ifeq ($(OS),Windows_NT)
EXE_PREFIX = .exe
else
EXE_PREFIX =
endif

TARGET_FILE = $(TARGET)$(EXE_PREFIX)


DIR_ROOT = ../..
DIR_MXDRV = $(DIR_ROOT)/src/mxdrv
DIR_UTIL = $(DIR_ROOT)/src
DIR_X68SOUND = $(DIR_ROOT)/src/x68sound
DIR_INCLUDE = $(DIR_ROOT)/include
DIR_OBJS = objs

COMMON_FLAGS = -O3 -I$(DIR_INCLUDE)

CXXFLAGS = $(COMMON_FLAGS) --std=c++11
CFLAGS = $(COMMON_FLAGS)

SRC_MXDRV = mxdrv.cpp mxdrv_context.cpp sound_iocs.cpp
SRC_X68SOUND = x68sound_adpcm.cpp x68sound_lfo.cpp x68sound_op.cpp 
SRC_X68SOUND += x68sound_opm.cpp x68sound_pcm8.cpp x68sound.cpp x68sound_context.cpp
SRC_UTIL = mdx_util.c
SRC_MAIN = main.c

LDLAGS = 
LIBS = 

SRCS = $(addprefix $(DIR_MXDRV),$(SRC_MXDRV))
SRCS += $(addprefix $(DIR_X68SOUND),$(SRC_X68SOUND))
SRCS += $(addprefix $(DIR_UTIL),$(SRC_UTIL))
SRCS += $(SRC_MAIN)

OBJS_FILE = $(patsubst %.cpp,%.o,$(SRC_MXDRV))
OBJS_FILE += $(patsubst %.cpp,%.o,$(SRC_X68SOUND))
OBJS_FILE += $(patsubst %.c,%.o,$(SRC_UTIL))
OBJS_FILE += $(patsubst %.c,%.o,$(SRC_MAIN))

OBJS = $(addprefix $(DIR_OBJS)/,$(OBJS_FILE))

all : $(TARGET_DIR) $(TARGET)

clean : 
	rm -rf $(TARGET_DIR)
	rm -f $(TARGET_FILE)

$(TARGET_DIR) : 
	mkdir -p $(TARGET_DIR)

$(TARGET) : $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(STRIP) $(TARGET_FILE)

$(DIR_OBJS)/%.o : $(DIR_MXDRV)/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(DIR_OBJS)/%.o : $(DIR_X68SOUND)/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(DIR_OBJS)/%.o : $(DIR_UTIL)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(DIR_OBJS)/%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<
