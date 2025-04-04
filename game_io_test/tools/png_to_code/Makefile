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

src := $(tools)/png_to_code

build := $(tools)/build/img

libs   := $(root)/libs

exe := png_to_code

program_exe := $(build)/$(exe)


#*** png_to_code ***

image_to_code_h := $(libs)/tools/image_to_code.hpp
image_to_code_h += $(libs)/image/image.hpp

#*********


#*** main cpp ***

main_c := $(src)/png_to_code_main.cpp
main_o := $(build)/main.o
obj := $(main_o)

main_dep := $(image_to_code_h)

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