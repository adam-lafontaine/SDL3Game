#pragma once

#include "../io/window.hpp"
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

        SDL_Rect render_rect;

        u32 width_px = 0;
        u32 height_px = 0;

        u32* data_px = 0;
    };


    static void destroy_screen_texture(ScreenMemory& screen)
    {
        if (screen.data_px)
        {
            mem::free(screen.data_px);
        }

        if (screen.texture)
        {
            SDL_DestroyTexture(screen.texture);
        }
    }


    static void destroy_screen_renderer(ScreenMemory& screen)
    {
        if (screen.renderer)
        {
            SDL_DestroyRenderer(screen.renderer);
        }
    }


    static void destroy_screen_window(ScreenMemory& screen)
    {
        if(screen.window)
        {
            SDL_DestroyWindow(screen.window);
        }
    }


    static void destroy_screen_memory(ScreenMemory& screen)
    {
        destroy_screen_texture(screen);
        destroy_screen_renderer(screen);
        destroy_screen_window(screen);

        SDL_zero(screen);
    }


    static bool create_screen_window(ScreenMemory& screen, cstr title, u32 width, u32 height)
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


    static bool create_screen_window_fullscreen(ScreenMemory& screen, cstr title)
    {
        screen.window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            0,
            0,
            SDL_WINDOW_FULLSCREEN_DESKTOP);

        if(!screen.window)
        {
            display_error("SDL_CreateWindow failed");
            return false;
        }

        return true;
    }


    static bool create_screen_renderer(ScreenMemory& screen)
    {
        screen.renderer = SDL_CreateRenderer(screen.window, -1, 0);

        if(!screen.renderer)
        {
            display_error("SDL_CreateRenderer failed");
            return false;
        }

        return true;
    }


    static bool create_screen_texture(ScreenMemory& screen, u32 width, u32 height)
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

        auto data = mem::alloc<u32>(width * height, "screen.data_px");
        if (!data)
        {
            SDL_DestroyTexture(screen.texture);
            return false;
        }

        screen.data_px = data;
        screen.width_px = width;
        screen.height_px = height;

        return true;
    }


    static bool create_screen_memory(ScreenMemory& screen, cstr title, Vec2Du32 window_size, Vec2Du32 pixel_size)
    {
        destroy_screen_memory(screen);

        if (!create_screen_window(screen, title, window_size.x, window_size.y))
        {
            destroy_screen_memory(screen);
            return false;
        }
        
        if (!create_screen_renderer(screen))
        {
            destroy_screen_memory(screen);
            return false;
        }

        if (!create_screen_texture(screen, pixel_size.x, pixel_size.y))
        {
            destroy_screen_memory(screen);
            return false;
        }

        return true;
    }


    static bool create_screen_memory_fullscreen(ScreenMemory& screen, cstr title, Vec2Du32 pixel_size)
    {
        destroy_screen_memory(screen);

        if (!create_screen_window_fullscreen(screen, title))
        {
            destroy_screen_memory(screen);
            return false;
        }
        
        if (!create_screen_renderer(screen))
        {
            destroy_screen_memory(screen);
            return false;
        }

        int width = 0;
        int height = 0;

        auto err = SDL_GetRendererOutputSize(screen.renderer, &width, &height);
        if (err)
        {
            destroy_screen_memory(screen);
            return false;
        }

        if (!create_screen_texture(screen, pixel_size.x, pixel_size.y))
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


    static bool copy_texture_pixels(ScreenMemory& screen)
    {
        auto src = (void*)screen.data_px;
        auto pitch = screen.width_px * sizeof(screen.data_px[0]);

        auto err = SDL_UpdateTexture(screen.texture, 0, src, pitch);
        if (err < 0)
        {
            sdl::print_error("SDL_UpdateTexture failed");
            return false;
        }

        return true;
    }
    
    
    static void resize_render_rect(ScreenMemory& screen)
    {
        int err = 0;

        err = SDL_SetRenderDrawColor(screen.renderer, 0, 0, 0, 255); // Black background
        err = SDL_RenderClear(screen.renderer);

        int out_width;
        int out_height;

        err = SDL_GetRendererOutputSize(screen.renderer, &out_width, &out_height);

    #ifdef PRINT_MESSAGES
        if (err)
        {
            print_error("SDL_GetCurrentRenderOutputSize()");
            return;
        }
    #endif

        auto in_width = (int)screen.width_px;
        auto in_height = (int)screen.height_px;

        auto scale_w = (f32)out_width / in_width;
        auto scale_h = (f32)out_height / in_height;

        auto scale = scale_w < scale_h ? scale_w : scale_h;

        auto w = (int)(scale * in_width);
        auto h = (int)(scale * in_height);

        auto& r = screen.render_rect;

        r.x = (out_width - w) / 2;
        r.y = (out_height - h) / 2;
        r.w = w;
        r.h = h;
    }


    static void resize_render_rect(ScreenMemory& screen, window::Rotate rotate)
    {
        using R = window::Rotate;

        int err = 0;

        err = SDL_SetRenderDrawColor(screen.renderer, 0, 0, 0, 255); // Black background
        err = SDL_RenderClear(screen.renderer);

        int out_width;
        int out_height;

        err = SDL_GetRendererOutputSize(screen.renderer, &out_width, &out_height);

    #ifdef PRINT_MESSAGES
        if (err)
        {
            print_error("SDL_GetCurrentRenderOutputSize()");
            return;
        }
    #endif

        auto in_width = (int)screen.width_px;
        auto in_height = (int)screen.height_px;

        auto scale_w = (f32)out_width / in_width;
        auto scale_h = (f32)out_height / in_height;

        switch (rotate)
        {
        case R::Clockwise_90:
        case R::CounterClockwise_90:
            scale_w = (f32)out_width / in_height;
            scale_h = (f32)out_height / in_width;
            break;
        default:
            break;
        }

        auto scale = scale_w < scale_h ? scale_w : scale_h;

        auto w = (int)(scale * in_width);
        auto h = (int)(scale * in_height);

        auto& r = screen.render_rect;

        r.x = (out_width - w) / 2;
        r.y = (out_height - h) / 2;
        r.w = w;
        r.h = h;
    }

}


/* window helpers */

namespace window
{    
    static bool create_window_memory(Window& window, cstr title, Vec2Du32 window_size, Vec2Du32 pixel_size)
    {
        auto data = sdl::allocate_screen_memory();
        if (!data)
        {
            return false;
        }

        auto& screen = *data;

        if (!sdl::create_screen_memory(screen, title, window_size, pixel_size))
        {
            return false;
        }

        window.handle = (u64)data;
        window.pixel_buffer = screen.data_px;
        window.width_px = screen.width_px;
        window.height_px = screen.height_px;

        return true;
    }


    static bool create_window_memory_fullscreen(Window& window, cstr title, Vec2Du32 pixel_size)
    {
        auto data = sdl::allocate_screen_memory();
        if (!data)
        {
            return false;
        }

        auto& screen = *data;

        if (!sdl::create_screen_memory_fullscreen(screen, title, pixel_size))
        {
            return false;
        }

        window.handle = (u64)data;
        window.pixel_buffer = screen.data_px;
        window.width_px = screen.width_px;
        window.height_px = screen.height_px;

        return true;
    }


    static sdl::ScreenMemory& get_screen(Window const& window)
    {
        return *(sdl::ScreenMemory*)window.handle;
    }
    

    static void set_window_icon_64(sdl::ScreenMemory& screen, window::Icon64 const& icon_64)
    {
        static_assert(window::PIXEL_SIZE == window::Icon64::bytes_per_pixel);

        sdl::set_window_icon(screen.window, icon_64);
    }


    static f32 get_rotate_angle(Rotate r)
    {
        switch (r)
        {
        case Rotate::Clockwise_90: return 90.0f;
        case Rotate::CounterClockwise_90: return -90.0f;
        default: return 0.0f;
        }
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


    bool create(Window& window, cstr title, Vec2Du32 window_size, Vec2Du32 pixel_size)
    {
        SDL_zero(window);
        
        if (!create_window_memory(window, title, window_size, pixel_size))
        {
            return false;
        }

        auto& screen = get_screen(window);

        sdl::resize_render_rect(screen);

        return true;
    }


    bool create(Window& window, cstr title, Vec2Du32 window_size, Vec2Du32 pixel_size, Rotate rotate)
    {
        if (rotate == Rotate::None)
        {
            sdl::print_error("Window rotate must be specified");
            return false;
        }

        SDL_zero(window);
        
        if (!create_window_memory(window, title, window_size, pixel_size))
        {
            return false;
        }

        auto& screen = get_screen(window);

        sdl::resize_render_rect(screen, rotate);

        return true;
    }


    bool create_fullscreen(Window& window, cstr title, Vec2Du32 pixel_size)
    {
        SDL_zero(window);
        
        if (!create_window_memory_fullscreen(window, title, pixel_size))
        {
            return false;
        }

        auto& screen = get_screen(window);

        sdl::resize_render_rect(screen);

        return true;
    }


    bool create_fullscreen(Window& window, cstr title, Vec2Du32 pixel_size, Rotate rotate)
    {
        if (rotate == Rotate::None)
        {
            sdl::print_error("Window rotate must be specified");
            return false;
        }
        
        SDL_zero(window);
        
        if (!create_window_memory_fullscreen(window, title, pixel_size))
        {
            return false;
        }

        auto& screen = get_screen(window);

        sdl::resize_render_rect(screen, rotate);

        return true;
    }
    
    
    void set_window_icon(Window& window, Icon64 const& icon)
    {
        auto& screen = get_screen(window);
        set_window_icon_64(screen, icon);
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
        auto& screen = get_screen(window);

        if (width == screen.width_px && height == screen.height_px)
        {
            return true;
        }
        
        sdl::destroy_screen_texture(screen);
        if (!sdl::create_screen_texture(screen, width, height))
        {
            return false;
        }

        window.pixel_buffer = screen.data_px;
        window.width_px = screen.width_px;
        window.height_px = screen.height_px;

        return true;
    }


    void render(Window const& window, b32 size_changed)
    {
        int err = 0;

        auto& screen = get_screen(window);        

        if (size_changed)
        {
            sdl::resize_render_rect(screen);
        }

        sdl::copy_texture_pixels(screen);

        SDL_SetRenderDrawColor(screen.renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(screen.renderer);

        err = SDL_RenderCopy(screen.renderer, screen.texture, NULL, &screen.render_rect);

        #ifdef PRINT_MESSAGES
        if(err)
        {
            sdl::print_error("SDL_RenderCopy failed");
        }
        #endif
        
        SDL_RenderPresent(screen.renderer);
    }


    void render(Window const& window, Rotate rotate, b32 size_changed)
    {
        int err = 0;

        auto& screen = get_screen(window);        

        if (size_changed)
        {
            sdl::resize_render_rect(screen, rotate);
        }

        sdl::copy_texture_pixels(screen);

        SDL_SetRenderDrawColor(screen.renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(screen.renderer);

        auto angle = get_rotate_angle(rotate);

        err = SDL_RenderCopyEx(screen.renderer, screen.texture,
            NULL,
            &screen.render_rect,
            angle,
            NULL,
            SDL_FLIP_NONE);

        #ifdef PRINT_MESSAGES
        if(err)
        {
            sdl::print_error("SDL_RenderCopyEx failed");
        }
        #endif
        
        SDL_RenderPresent(screen.renderer);
    }


    void hide_mouse_cursor()
    {
        SDL_ShowCursor(SDL_DISABLE);
    }


    void show_mouse_cursor()
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
}