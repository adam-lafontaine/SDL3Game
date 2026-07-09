#include "app_assert_sdl2.hpp"

#ifdef NDEBUG

#include "./io_test_sdl2_o.cpp"
#include "../io_test_main.cpp"

#else

#include "../io_test_main.cpp"
#include "./io_test_sdl2_o.cpp"

#endif