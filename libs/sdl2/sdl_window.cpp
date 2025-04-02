#pragma once

#include "../output/window.hpp"
#include "../alloc_type/alloc_type.hpp"
#include "sdl_include.hpp"


/* screen memory */

namespace sdl
{
    class ScreenMemory
    {
    public:
        SDL_Window* window = 0;
        SDL_Renderer* renderer = 0;
        SDL_Texture* texture = 0;
    };


    static void destroy_screen_memory(ScreenMemory& screen)
    {
        if (screen.texture)
        {
            SDL_DestroyTexture(screen.texture);
        }

        if (screen.renderer)
        {
            SDL_DestroyRenderer(screen.renderer);
        }

        if(screen.window)
        {
            SDL_DestroyWindow(screen.window);
        }

        SDL_zero(screen);
    }


    static bool create_window(ScreenMemory& screen, cstr title, u32 width, u32 height)
    {
        screen.window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            (int)width,
            (int)height,
            SDL_WINDOW_RESIZABLE);

        if(!screen.window)
        {
            display_error("SDL_CreateWindow failed");
            return false;
        }

        return true;
    }


    static bool create_renderer(ScreenMemory& screen)
    {
        screen.renderer = SDL_CreateRenderer(screen.window, -1, 0);

        if(!screen.renderer)
        {
            display_error("SDL_CreateRenderer failed");
            return false;
        }

        return true;
    }


    static bool create_texture(ScreenMemory& screen, u32 width, u32 height)
    {
        static_assert(window::PIXEL_SIZE == 4); // SDL_PIXELFORMAT_ABGR8888

        screen.texture =  SDL_CreateTexture(
            screen.renderer,
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_STREAMING,
            width,
            height);
        
        if(!screen.texture)
        {
            display_error("SDL_CreateTexture failed");
            return false;
        }

        return true;
    }


    static bool create_screen_memory(ScreenMemory& screen, cstr title, u32 width, u32 height)
    {
        destroy_screen_memory(screen);

        if (!create_window(screen, title, width, height))
        {
            destroy_screen_memory(screen);
            return false;
        }
        
        if (!create_renderer(screen))
        {
            destroy_screen_memory(screen);
            return false;
        }

        if (!create_texture(screen, width, height))
        {
            destroy_screen_memory(screen);
            return false;
        }

        return true;
    }
}


/* static data */

namespace sdl
{
    constexpr u32 N_SCREEN_MEMORY = 2;

    static ScreenMemory screen_data[N_SCREEN_MEMORY] = { 0 };
    static u32 screen_data_size = 0;
}


/* sdl helpers */

namespace sdl
{    
    static ScreenMemory* allocate_screen_memory()
    {
        if (screen_data_size >= N_SCREEN_MEMORY)
        {
            return 0;
        }

        auto data = screen_data + screen_data_size;
        ++screen_data_size;

        return data;
    }


    static void set_window_icon(SDL_Window* window, window::Icon64 const& icon_64)
    {
        static_assert(window::PIXEL_SIZE == icon_64.bytes_per_pixel);

        // these masks are needed to tell SDL_CreateRGBSurface(From)
        // to assume the data it gets is byte-wise RGB(A) data
        Uint32 rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        int shift = (icon_64.bytes_per_pixel == 3) ? 8 : 0;
        rmask = 0xff000000 >> shift;
        gmask = 0x00ff0000 >> shift;
        bmask = 0x0000ff00 >> shift;
        amask = 0x000000ff >> shift;
    #else // little endian, like x86
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = (icon_64.bytes_per_pixel == 3) ? 0 : 0xff000000;
    #endif

        SDL_Surface* icon = SDL_CreateRGBSurfaceFrom(
            (void*)icon_64.pixel_data,
            icon_64.width,
            icon_64.height,
            icon_64.bytes_per_pixel * 8,
            icon_64.bytes_per_pixel * icon_64.width,
            rmask, gmask, bmask, amask);

        SDL_SetWindowIcon(window, icon);

        SDL_FreeSurface(icon);
    }

}


/* window helpers */

namespace window
{    
    bool create_screen(Window& window, cstr title, u32 width, u32 height)
    {
        auto data = sdl::allocate_screen_memory();
        if (!data)
        {
            return false;
        }

        auto& screen = *data;

        if (!sdl::create_screen_memory(screen, title, width, height))
        {
            return false;
        }

        window.handle = (u64)data;
        window.width = width;
        window.height = height;

        return true;
    }


    sdl::ScreenMemory& get_screen(Window const& window)
    {
        return *(sdl::ScreenMemory*)window.handle;
    }
}


/* api */

namespace window
{
    constexpr auto subsystem_flags = SDL_INIT_VIDEO;


    bool init()
    {
        auto error = SDL_InitSubSystem(subsystem_flags);
        if (error)
        {
            sdl::print_error("Init Video failed");
            return false;
        }

        return true;
    }


    void close()
    {
        SDL_QuitSubSystem(subsystem_flags);
    }


    bool create(Window& window, cstr title, u32 width, u32 height, Icon64 const& icon)
    {
        SDL_zero(window);
        
        if (!create_screen(window, title, width, height))
        {
            return false;
        }

        auto& screen = get_screen(window);
        sdl::set_window_icon(screen.window, icon);

        auto buffer = mem::alloc<u32>(width * height, "window.pixel_buffer");
        if (!buffer)
        {
            sdl::destroy_screen_memory(screen);
            SDL_zero(window);
            return false;
        }

        window.pixel_buffer = buffer;

        return true;
    }


    bool create(Window& window, cstr title, u32 width, u32 height)
    {
        SDL_zero(window);
        
        if (!create_screen(window, title, width, height))
        {
            return false;
        }

        auto& screen = get_screen(window);

        auto buffer = mem::alloc<u32>(width * height, "window.pixel_buffer");
        if (!buffer)
        {
            sdl::destroy_screen_memory(screen);
            SDL_zero(window);
            return false;
        }

        window.pixel_buffer = buffer;

        return true;
    }


    void destroy(Window& window)
    {
        auto& screen = get_screen(window);

        sdl::destroy_screen_memory(screen);
        mem::free(window.pixel_buffer);

        SDL_zero(window);
    }


    bool resize_pixel_buffer(Window& window, u32 width, u32 height)
    {
        if (width == window.width && height == window.height)
        {
            return true;
        }

        auto& screen = get_screen(window);
        if (screen.texture)
        {
            SDL_DestroyTexture(screen.texture);
            screen.texture = 0;
        }

        if (!sdl::create_texture(screen, width, height))
        {
            return false;
        }
        
        auto n_pixels = width * height;
        if (window.width * window.height == n_pixels)
        {
            window.width = width;
            window.height = height;
            return true;
        }

        if (window.pixel_buffer)
        {
            mem::free(window.pixel_buffer);
            window.pixel_buffer = 0;
        }        

        auto buffer = mem::alloc<u32>(n_pixels, "window.pixel_buffer");
        if (!buffer)
        {
            return false;
        }

        window.pixel_buffer = buffer;
        window.width = width;
        window.height = height;

        return true;
    }


    void render(Window const& window)
    {
        auto pitch = window.width * sizeof(window.pixel_buffer[0]);

        auto& screen = get_screen(window);

        #ifdef PRINT_MESSAGES

        auto error = SDL_UpdateTexture(screen.texture, 0, (void*)window.pixel_buffer, pitch);
        if(error)
        {
            sdl::print_error("SDL_UpdateTexture failed");
        }

        error = SDL_RenderCopy(screen.renderer, screen.texture, 0, 0);
        if(error)
        {
            sdl::print_error("SDL_RenderCopy failed");
        }

        #else

        SDL_UpdateTexture(screen.texture, 0, (void*)window.pixel_buffer, pitch);
        SDL_RenderCopy(screen.renderer, screen.texture, 0, 0);

        #endif
        
        SDL_RenderPresent(screen.renderer);
    }
}