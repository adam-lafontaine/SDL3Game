GPP := g++-15

GPP += -std=c++20
GPP += -mavx -mavx2 -mfma
GPP += -DIMAGE_READ
GPP += -DAPP_ASSERT_LOG

#GPP += -O3
#GPP += -DNDEBUG

#GPP += -DALLOC_COUNT

#GPP += -DAPP_FULLSCREEN

NO_FLAGS := 

SDL2 := -lSDL2 -lSDL2_mixer

ALL_LFLAGS := $(SDL2)


root       := ../../../..

app   := $(root)/game_io_test
build := $(app)/build/ubuntu_sdl2
src   := $(app)/src
res := $(src)/res

pltfm := $(src)/pltfm/sdl2

libs := $(root)/libs

exe := io_test

program_exe := $(build)/$(exe)

bin_data := $(res)/io_test_data.bin



#*** main cpp ***

main_c := $(pltfm)/io_test_sdl2.cpp
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
	cp $(bin_data) $(build)

run: build
	$(program_exe)
	@echo "\n"


clean:
	rm -fv $(build)/*


clean_main:
	rm -fv $(build)/main.o

setup:
	mkdir -p $(build)