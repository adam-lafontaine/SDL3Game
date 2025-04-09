GPP := g++-11

GPP += -std=c++20
GPP += -mavx -mavx2 -mfma
#GPP += -O3
#GPP += -DNDEBUG

GPP += -DALLOC_NO_COUNT

NO_FLAGS :=
ALL_LFLAGS :=


root := ../../..

game := $(root)/game_io_test

tools := $(game)/tools

src := $(tools)/assets_to_bin

build := $(tools)/build/a2c

libs   := $(root)/libs

exe := assets_to_bin

program_exe := $(build)/$(exe)


#*** assets_to_bin ***

assets_to_bin_h := $(libs)/tools/assets_to_bin.hpp
assets_to_bin_h += $(libs)/image/image.hpp

#*********


#*** main cpp ***

main_c := $(src)/assets_to_bin_main.cpp
main_o := $(build)/main.o
obj := $(main_o)

main_dep := $(assets_to_bin_h)

#************


$(main_o): $(main_c) $(main_dep)
	@echo "\n  main"
	$(GPP) -o $@ -c $< $(ALL_LFLAGS)


$(program_exe): $(obj)
	@echo "\n  program_exe"
	$(GPP) -o $@ $+ $(ALL_LFLAGS)



build: $(program_exe)


run: build
	$(program_exe)
	@echo "\n"


clean:
	rm -rfv $(build)/*

setup:
	mkdir -p $(build)