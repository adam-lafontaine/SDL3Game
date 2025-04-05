GPP := g++-11

GPP += -std=c++20
GPP += -mavx -mavx2 -mfma
#GPP += -O3
#GPP += -DNDEBUG

#GPP += -DALLOC_COUNT

NO_FLAGS := 
SDL2   := `sdl2-config --cflags --libs`

ALL_LFLAGS := $(SDL2) -lpthread


root       := ../../../..

app   := $(root)/game_io_test
build := $(app)/build/ubuntu_sdl2
src   := $(app)/src

pltfm := $(src)/pltfm/sdl2

libs := $(root)/libs

exe := io_test

program_exe := $(build)/$(exe)


#*** libs/util ***

util := $(libs)/util

types_h := $(util)/types.hpp

numeric_h := $(util)/numeric.hpp
numeric_h += $(types_h)

stopwatch_h := $(util)/stopwatch.hpp

memory_buffer := $(util)/memory_buffer.hpp

#************


#*** alloc_type ***

alloc_type := $(libs)/alloc_type

alloc_type_h := $(alloc_type)/alloc_type.hpp
alloc_type_h += $(types_h)

alloc_type_c := $(alloc_type)/alloc_type.cpp
alloc_type_c += $(alloc_type_h)

#*************


#*** memory_buffer ***

memory_buffer_h := $(util)/memory_buffer.hpp
memory_buffer_h += $(alloc_type_h)

#***********


#*** stack_buffer ***

stack_buffer_h := $(util)/stack_buffer.hpp
stack_buffer_h += $(types_h)

#***********


#*** stb_libs ***

stb_libs := $(libs)/stb_libs

qsprintf_h := $(stb_libs)/qsprintf.hpp
stb_image_options_h := $(stb_libs)/stb_image_options.hpp

stb_libs_c := $(stb_libs)/stb_libs.cpp
stb_libs_c += $(stb_image_options_h)

#*************


#*** span ***

span := $(libs)/span

span_h := $(span)/span.hpp
span_h += $(memory_buffer_h)
span_h += $(stack_buffer_h)
span_h += $(qsprintf_h)

span_c := $(span)/span.cpp

#************


#*** image ***

image := $(libs)/image

image_h := $(image)/image.hpp
image_h += $(span_h)

image_c := $(image)/image.cpp
image_c += $(image_h)
image_c += $(numeric_h)
image_c += $(stb_image_options_h)

#*************


#*** ascii_image ***

ascii_image := $(libs)/ascii_image

ascii_image_h := $(ascii_image)/ascii_image.hpp
ascii_image_h += $(image_h)
ascii_image_h += $(span_h)
ascii_image_h += $(numeric_h)

ascii_image_c := $(ascii_image)/ascii_image.cpp
ascii_image_c += $(ascii_image_h)
ascii_image_c += $(ascii_image)/ascii_5.cpp
ascii_image_c += $(ascii_image)/ascii_joystick_8.cpp

#************


#*** input ***

input := $(libs)/input

input_h := $(input)/input.hpp
input_h += $(types_h)
input_h += $(input)/keyboard_input.hpp
input_h += $(input)/mouse_input.hpp
input_h += $(input)/controller_input.hpp

input_state_h := $(input)/input_state.hpp
input_state_h += $(input_h)

#*************


#*** output ***

output := $(libs)/output

window_h := $(output)/window.hpp

#*************


#*** sdl2 ***

sdl2 := $(libs)/sdl2

sdl_include_h := $(sdl2)/sdl_include.hpp

sdl_input_c := $(sdl2)/sdl_input.cpp
sdl_input_c += $(input_state_h)
sdl_input_c += $(numeric_h)
sdl_input_c += $(sdl_include_h)

sdl_window_c := $(sdl2)/sdl_window.cpp
sdl_window_c += $(window_h)
sdl_window_c += $(alloc_type_h)
sdl_window_c += $(sdl_include_h)

#************


#*** app ********

app := $(src)/app
res := $(src)/res

app_h := $(app)/app.hpp

app_c := $(app)/app.cpp
app_c += $(app_h)
app_c += $(numeric_h)
app_c += $(ascii_image_h)

# assets.cpp
app_c += $(app)/assets.cpp
app_c += $(res)/controller.cpp
app_c += $(res)/keyboard.cpp
app_c += $(res)/mouse.cpp

#************


#*** main cpp ***

main_c := $(pltfm)/io_test_sdl2_main.cpp
main_o := $(build)/main.o
obj    := $(main_o)

main_dep := $(window_h)
main_dep += $(input_h)
main_dep += $(stopwatch_h)
main_dep += $(app_h)

# main_o.cpp
main_dep += $(app_c)
main_dep += $(alloc_type_c)
main_dep += $(span_c)
main_dep += $(image_c)
main_dep += $(stb_libs_c)
main_dep += $(ascii_image_c)
main_dep += $(sdl_input_c)
main_dep += $(sdl_window_c)

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