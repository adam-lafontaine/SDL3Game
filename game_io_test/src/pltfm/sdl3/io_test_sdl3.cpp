#include "app_assert_sdl3.hpp"

#ifdef NDEBUG

#include "./io_test_sdl3_o.cpp"
#include "../io_test_main.cpp"

#else

#include "../io_test_main.cpp"
#include "./io_test_sdl3_o.cpp"

#endif

