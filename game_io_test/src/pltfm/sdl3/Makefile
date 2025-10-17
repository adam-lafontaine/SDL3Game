GPP := g++-11

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


#*** libs/util ***

util := $(libs)/util

types_h := $(util)/types.hpp

numeric_h := $(util)/numeric.hpp
numeric_h += $(types_h)

#************


#*** alloc_type ***

alloc_type := $(libs)/alloc_type

alloc_type_h := $(alloc_type)/alloc_type.hpp
alloc_type_h += $(types_h)

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


#*** io ***

io := $(libs)/io

input := $(io)/input

input_h := $(input)/input.hpp
input_h += $(types_h)
input_h += $(input)/keyboard_input.hpp
input_h += $(input)/mouse_input.hpp
input_h += $(input)/gamepad_input.hpp
input_h += $(input)/joystick_input.hpp

input_state_h := $(input)/input_state.hpp
input_state_h += $(input_h)

window_h := $(io)/window.hpp
audio_h := $(io)/audio.hpp
filesystem_h := $(io)/filesystem.hpp

#*************


#*** sdl3 ***

sdl3 := $(libs)/sdl3

sdl_include_h := $(sdl3)/sdl_include.hpp

sdl_span_c := $(sdl3)/sdl_span.cpp
sdl_span_c += $(span_h)
sdl_span_c += $(sdl_include_h)

sdl_alloc_c := $(sdl3)/sdl_alloc.cpp
sdl_alloc_c += $(alloc_type_h)
sdl_alloc_c += $(sdl_include_h)
sdl_alloc_c += $(span_h)

sdl_audio_c := $(sdl3)/sdl_audio.cpp
sdl_audio_c += $(audio_h)
sdl_audio_c += $(filesystem_h)
sdl_audio_c += $(numeric_h)
sdl_audio_c += $(alloc_type_h)
sdl_audio_c += $(sdl_include_h)

sdl_filesystem_c := $(sdl3)/sdl_filesystem.cpp
sdl_filesystem_c += $(filesystem_h)
sdl_filesystem_c += $(alloc_type_h)
sdl_filesystem_c += $(sdl_include_h)

sdl_joystick_c := $(sdl3)/sdl_joystick.cpp
sdl_keybord_c  := $(sdl3)/sdl_keyboard.cpp
sdl_mouse_c    := $(sdl3)/sdl_mouse.cpp

sdl_input_c := $(sdl3)/sdl_input.cpp
sdl_input_c += $(input_state_h)
sdl_input_c += $(numeric_h)
sdl_input_c += $(sdl_include_h)
sdl_input_c += $(sdl_joystick_c)
sdl_input_c += $(sdl_keyboard_c)
sdl_input_c += $(sdl_mouse_c)

sdl_stb_libs_c := $(sdl3)/sdl_stb_libs.cpp
sdl_stb_libs_c += $(stb_libs_c)

sdl_window_c := $(sdl3)/sdl_window.cpp
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
app_c += $(audio_h)
app_c += $(filesystem_h)
app_c += $(res)/asset_sizes.cpp

#************


#*** main cpp ***

main_c := $(pltfm)/io_test_sdl3_main.cpp
main_o := $(build)/main.o
obj    := $(main_o)

main_dep := $(window_h)
main_dep += $(input_h)
main_dep += $(app_h)

# main_o.cpp
main_dep += $(pltfm)/main_o.cpp
main_dep += $(app_c)
main_dep += $(image_c)
main_dep += $(ascii_image_c)
main_dep += $(sdl_span_c)
main_dep += $(sdl_alloc_c)
main_dep += $(sdl_input_c)
main_dep += $(sdl_window_c)
main_dep += $(sdl_audio_c)
main_dep += $(sdl_filesystem_c)
main_dep += $(sdl_stb_libs_c)

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