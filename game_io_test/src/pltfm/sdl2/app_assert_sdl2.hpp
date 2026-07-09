#ifdef APP_ASSERT_LOG

#ifndef NDEBUG
#include <SDL2/SDL.h>
#define app_assert(condition) SDL_assert(condition)
#define app_log(...) SDL_Log(__VA_ARGS__)
#define app_crash(message) SDL_assert(false && message)
#endif

#else

#define app_assert(...)
#define app_log(...)
#define app_crash(...)

#endif