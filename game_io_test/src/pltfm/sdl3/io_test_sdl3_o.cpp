
#ifndef NDEBUG
#include "../../../src/app/app.cpp"
#endif

#include "../../../../libs/image/image.cpp"
#include "../../../../libs/ascii_image/ascii_image.cpp"

#include "../../../../libs/sdl3/sdl_span.cpp"
#include "../../../../libs/sdl3/sdl_alloc.cpp"
#include "../../../../libs/sdl3/sdl_input.cpp"
#include "../../../../libs/sdl3/sdl_window.cpp"

#include "../../../../libs/sdl3/sdl_stb_libs.cpp"
#include "../../../../libs/sdl3/sdl_math.cpp"
#include "../../../../libs/sdl3/sdl_datetime.cpp"

#ifndef NO_AUDIO
#include "../../../../libs/sdl3/sdl_audio.cpp"
#endif

#ifndef NO_FILESYSTEM
#include "../../../../libs/sdl3/sdl_filesystem.cpp"
#endif

#ifdef NDEBUG
#include "../../../src/app/app.cpp"
#endif