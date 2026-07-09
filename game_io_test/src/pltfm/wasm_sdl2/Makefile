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
EPPFLAGS += -DNO_FILESYSTEM
EPPFLAGS += -DIMAGE_READ

#EPPFLAGS += -DAPP_ASSERT_LOG
#EPPFLAGS += -DNDEBUG -O3
#EPPFLAGS += -sASSERTIONS=1

EPPFLAGS += -sUSE_SDL=2

# SDL3_Mixer not available
#EPPFLAGS += -DNO_AUDIO
EPPFLAGS += -sUSE_SDL_MIXER=2
EPPFLAGS += -sUSE_OGG=1

EPPFLAGS += -sFETCH=1
EPPFLAGS += -sALLOW_MEMORY_GROWTH=1

HTMLFLAGS := 
HTMLFLAGS += --shell-file out_template.html

LDFLAGS :=

EXE := io_test

ROOT := ../../../..

APP := $(ROOT)/game_io_test
BUILD := $(APP)/build/wasm_sdl2

OUT := $(BUILD)/$(EXE).html

OUT_WASM := $(BUILD)/$(EXE).wasm
OUT_JS   := $(BUILD)/$(EXE).js

RES := $(APP)/src/res

BIN_DATA := $(RES)/io_test_data.bin

SRC := ./io_test_wasm_sdl2.cpp


#**********************

build:
	$(EPP) $(EPPFLAGS) $(HTMLFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)
	cp $(BIN_DATA) $(BUILD)


clean:
	rm -rfv $(BUILD)/*


setup:
	mkdir -p $(BUILD)


delete:
	rm -rfv $(BUILD)


source:
	source ~/Repos/zRepos/emsdk/emsdk_env.sh
	

serve:
	python3 -m http.server --directory $(BUILD) 8222

