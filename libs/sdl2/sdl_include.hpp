#pragma once

#if defined(_WIN32)
#define SDL_MAIN_HANDLED
#endif

#include <SDL2/SDL.h>

#ifndef NDEBUG
#define PRINT_MESSAGES
#endif

#ifdef __EMSCRIPTEN__
#define NO_CONTROLLER
#define NO_WINDOW
#ifdef WASM_PRINTF
#define PRINT_MESSAGES
#endif
#endif

#ifdef PRINT_MESSAGES
#include <cstdio>
#endif


namespace sdl
{
    inline void print_message(const char* msg)
    {
    #ifdef PRINT_MESSAGES
        printf("%s\n", msg);
    #endif
    }


    inline void print_error(const char* msg)
    {
    #ifdef PRINT_MESSAGES
        printf("%s\n%s\n", msg, SDL_GetError());
    #endif
    }


    inline void display_message(const char* msg)
    {
    #ifndef NO_WINDOW
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Message", msg, 0);
    #endif

        print_error(msg);
    }


    inline void display_error(const char* title)
    {
    #ifndef NO_WINDOW
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, SDL_GetError(), 0);
    #endif

        print_error(title);
    }
}