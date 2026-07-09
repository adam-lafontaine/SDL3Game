#include "../sdl3/app_assert_sdl3.hpp"


#ifdef NDEBUG

#include "../sdl3/io_test_sdl3_o.cpp"
#include "../io_test_wasm_main.cpp"

#else

#include "../io_test_wasm_main.cpp"
#include "../sdl3/io_test_sdl3_o.cpp"

#endif