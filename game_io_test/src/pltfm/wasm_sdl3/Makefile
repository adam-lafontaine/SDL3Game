# ~/Repos/zRepos/emsdk
# git pull
# ./emsdk install latest
# ./emsdk activate latest

# Activate PATH and other environment variables in the current terminal
# source ~/Repos/zRepos/emsdk/emsdk_env.sh

# python3 -m http.server 8080


EPP := em++

EPPFLAGS := -std=c++23
EPPFLAGS += -DMATH_NO_SIMD
EPPFLAGS += -DAPP_ASSERT_LOG
#EPPFLAGS += -DNDEBUG -O3

EPPFLAGS += -sUSE_SDL=3
EPPFLAGS += -sUSE_SDL_MIXER=3
EPPFLAGS += -sUSE_OGG=1
EPPFLAGS += -sFETCH=1
EPPFLAGS += -sALLOW_MEMORY_GROWTH=1

HTMLFLAGS := 
HTMLFLAGS += --shell-file out_template.html

LDFLAGS :=

EXE := io_test

ROOT := := ../../../..

APP := $(ROOT)/game_io_test
BUILD := $(APP)/build/wasm_sdl3

OUT_WASM := $(BUILD)/$(EXE).wasm
OUT_JS   := $(BUILD)/$(EXE).js

RES := $(APP)/res

BIN_DATA := $(RES)/io_test_data.bin

SRC := ./io_test_wasm_main.cpp

