GPP := g++-15

GPP += -std=c++20
GPP += -mavx -mavx2 -mfma
#GPP += -O3
#GPP += -DNDEBUG

#GPP += -DALLOC_COUNT

#GPP += -DAPP_FULLSCREEN

NO_FLAGS := 
#SDL2   := `sdl3-config --cflags --libs`
SDL3 := -lSDL3
SDL3 += -lSDL3_mixer


ALL_LFLAGS := $(SDL3)


root       := ../../../..

app   := $(root)/game_io_test
build := $(app)/build/ubuntu_sdl3
src   := $(app)/src

pltfm := $(src)/pltfm/sdl3

libs := $(root)/libs

exe := io_test

program_exe := $(build)/$(exe)


#*** main cpp ***

main_c := $(pltfm)/main_o.cpp
main_o := $(build)/main.o
obj    := $(main_o)

main_dep := 

#*************


#*** main ***

$(main_o): $(main_c) $(main_dep)
	@echo "\n  main"
	$(GPP) -o $@ -c $< $(ALL_LFLAGS)

#**************


$(program_exe): $(obj)
	@echo "\n  program_exe"
	$(GPP) -o $@ $+ $(ALL_LFLAGS)


build: $(program_exe)


run: build
	$(program_exe)
	@echo "\n"


clean:
	rm -fv $(build)/*


clean_main:
	rm -fv $(build)/main.o

setup:
	mkdir -p $(build)