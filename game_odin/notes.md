# Odin Language Notes

## Install

Latest releases: https://github.com/odin-lang/Odin/releases

```
mkdir -p ~/odin
cd ~/Downloads

wget https://github.com/odin-lang/Odin/releases/download/dev-2026-07/odin-linux-amd64-dev-2026-07.tar.gz
tar -xzf odin-linux-amd64-*.tar.gz -C ~/odin --strip-components=1

echo 'export PATH="$HOME/odin:$PATH"' >> ~/.bashrc
source ~/.bashrc

odin version

sudo apt install clang
```


## C++ Raylib Window

```
#pragma once

#include "../io/window.hpp"
//#include "../alloc_type/alloc_type.hpp"
#include "rlcpp_include.hpp"

#define ASSERT_WINDOW
#define LOG_WINDOW


#ifndef NDEBUG

#ifdef LOG_WINDOW
#include <cstdio>
#define window_log(...) printf(__VA_ARGS__)
#else
#define window_log(...)
#endif

#ifdef ASSERT_WINDOW
#include<cassert>
#define window_assert(condition) assert(condition)
#define window_crash(msg) assert(msg && false)
#else
#define window_assert(...)
#endif

#else

#define window_log(...)
#define window_assert(...)

#endif


/* screen memory */

namespace rlcpp
{
    class ScreenMemory
    {
    public:
        
        cstr window_title = 0;

        rl::RenderTexture2D target;

        rl::Image screen_pixels;
        rl::Rectangle render_rect;
    };


    static void destroy_screen_texture(ScreenMemory& screen)
    {
        rl::UnloadRenderTexture(screen.target);
        rl::UnloadImage(screen.screen_pixels);

        screen.target = {};
        screen.screen_pixels = {};
    }


    static void destroy_screen_memory(ScreenMemory& screen)
    {
        destroy_screen_texture(screen);
        rl::CloseWindow();
    }


    static bool create_screen_window(ScreenMemory& screen, cstr title,  u32 width, u32 height)
    {
        rl::SetConfigFlags(rl::FLAG_WINDOW_RESIZABLE | rl::FLAG_VSYNC_HINT);
        rl::InitWindow((int)width, (int)height, title);
        //rl::SetWindowMinSize(320, 240);

        screen.window_title = title;

        bool ok = rl::IsWindowReady();

        window_assert(ok && "*** create_screen_window() ***");

        return ok;
    }


    static bool create_screen_window_fullscreen(ScreenMemory& screen, cstr title)
    {
        rl::SetConfigFlags(rl::FLAG_FULLSCREEN_MODE | rl::FLAG_VSYNC_HINT);
        rl::InitWindow(0, 0, title);

        screen.window_title = title;

        bool ok = rl::IsWindowReady();

        window_assert(ok && "*** create_screen_window_fullscreen() ***");

        return ok;
    }


    static bool create_screen_texture(ScreenMemory& screen, u32 width, u32 height)
    {
        auto w = (int)width;
        auto h = (int)height;

        screen.target = rl::LoadRenderTexture(w, h);
        rl::SetTextureFilter(screen.target.texture, rl::TEXTURE_FILTER_BILINEAR);

        bool ok = rl::IsRenderTextureValid(screen.target);

        window_assert(ok && "*** create_screen_texture() ***");

        if (!ok)
        {
            return false;
        }

        screen.screen_pixels = rl::GenImageColor(w, h, rl::BLACK);

        auto& tx = screen.target.texture;
        auto& px = screen.screen_pixels;

        ok &= !(!px.data);
        ok &= px.width == tx.width;
        ok &= px.height == tx.height;
        ok &= rl::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

        window_assert(ok && "*** create_screen_texture() ***");

        return ok;
    }


    static bool create_screen_memory(ScreenMemory& screen, cstr title, Vec2Du32 window_size, Vec2Du32 pixel_size)
    {
        bool ok = true;

        ok &= create_screen_window(screen, title, window_size.x, window_size.y);
        ok &= create_screen_texture(screen, pixel_size.x, pixel_size.y);

        if (!ok)
        {
            destroy_screen_memory(screen);
        }

        return ok;
    }


    static bool create_screen_memory_fullscreen(ScreenMemory& screen, cstr title, Vec2Du32 pixel_size)
    {
        bool ok = true;

        ok &= create_screen_window_fullscreen(screen, title);
        ok &= create_screen_texture(screen, pixel_size.x, pixel_size.y);

        if (!ok)
        {
            destroy_screen_memory(screen);
        }

        return ok;
    }
}


/* static data */

namespace window
{
    constexpr u32 N_SCREEN_MEMORY = 2;

    static rlcpp::ScreenMemory screen_data[N_SCREEN_MEMORY] = { 0 };
    static u32 screen_data_size = 0;
}


/* helpers */

namespace window
{
    static rlcpp::ScreenMemory* allocate_screen_memory()
    {
        if (screen_data_size >= N_SCREEN_MEMORY)
        {
            return 0;
        }

        auto data = screen_data + screen_data_size;
        ++screen_data_size;

        return data;
    }
    
    
    static rlcpp::ScreenMemory& get_screen(Window const& window)
    {
        return *(rlcpp::ScreenMemory*)window.handle;
    }


    static void set_window_pixels(rlcpp::ScreenMemory const& screen, Window& window)
    {
        auto& pixels = screen.screen_pixels;

        window.pixel_buffer = (u32*)pixels.data;
        window.width_px = (u32)pixels.width;
        window.height_px = (u32)pixels.height;
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
    
    
    static void resize_render_rect(rlcpp::ScreenMemory& screen, Rotate rotate = Rotate::None)
    {
        using R = window::Rotate;

        auto in_w = (f32)screen.screen_pixels.width;
        auto in_h = (f32)screen.screen_pixels.height;
        auto rot_w = in_w;
        auto rot_h = in_h;

        auto out_w = (f32)rl::GetScreenWidth();
        auto out_h = (f32)rl::GetScreenHeight();

        switch (rotate)
        {
        case R::Clockwise_90:
        case R::CounterClockwise_90:
            rot_w = in_h;
            rot_h = in_w;
            break;
        default:
            break;
        }

        auto scale_w = out_w / rot_w;
        auto scale_h = out_h / rot_h;
        auto scale = scale_w < scale_h ? scale_w : scale_h;

        f32 w = scale * in_w;
        f32 h = scale * in_h;
        f32 xb = (out_w - w) / 2;
        f32 yb = (out_h - h) / 2;

        auto& r = screen.render_rect;
        r.x = xb;
        r.y = yb;
        r.width = w;
        r.height = h;
    }


    static void render_screen_memory(rlcpp::ScreenMemory& screen, Rotate rotate = Rotate::None)
    {
        rl::Rectangle r_px{};
        r_px.x = 0.0f;
        r_px.y = 0.0f;
        r_px.width = (f32)screen.screen_pixels.width;
        r_px.height = (f32)screen.screen_pixels.height;

        auto r_out = screen.render_rect;

        rl::Vector2 origin{};
        origin.x = r_out.width / 2;
        origin.y = r_out.height / 2;

        auto tint = rl::WHITE;

        f32 angle = get_rotate_angle(rotate);
        
        rl::UpdateTexture(screen.target.texture, screen.screen_pixels.data);

        rl::BeginDrawing();

        rl::ClearBackground(rl::BLACK);

        rl::DrawTexturePro(
            screen.target.texture,
            r_px,
            screen.render_rect,
            origin,
            angle,
            tint
        );

        rl::EndDrawing();
    }
}



/* api */

namespace window
{
    bool init() 
    {
        return true;
    }


    void close() 
    {

    }


    bool create(Window& window, cstr title, Vec2Du32 window_size, Vec2Du32 pixel_size)
    {
        auto data = allocate_screen_memory();
        if (!data)
        {
            window_crash("*** SCREEN NOT ALLOCATED ***");
            return false;
        }

        auto& screen = *data;

        if (!rlcpp::create_screen_memory(screen, title, window_size, pixel_size))
        {
            return false;
        }

        window.handle = (u64)data;
        set_window_pixels(screen, window);
        resize_render_rect(screen);

        return true;
    }


    bool create(Window& window, cstr title, Vec2Du32 window_size, Vec2Du32 pixel_size, Rotate rotate)
    {
        if (rotate == Rotate::None)
        {
            window_crash("*** Window rotate must be specified ***");
            return false;
        }
        
        auto data = allocate_screen_memory();
        if (!data)
        {
            window_crash("*** SCREEN NOT ALLOCATED ***");
            return false;
        }

        auto& screen = *data;

        if (!rlcpp::create_screen_memory(screen, title, window_size, pixel_size))
        {
            return false;
        }

        window.handle = (u64)data;
        set_window_pixels(screen, window);
        resize_render_rect(screen, rotate);

        return true;
    }


    bool create_fullscreen(Window& window, cstr title, Vec2Du32 pixel_size)
    {
        auto data = allocate_screen_memory();
        if (!data)
        {
            window_crash("*** SCREEN NOT ALLOCATED ***");
            return false;
        }

        auto& screen = *data;

        if (!rlcpp::create_screen_memory_fullscreen(screen, title, pixel_size))
        {
            return false;
        }

        window.handle = (u64)data;
        set_window_pixels(screen, window);
        resize_render_rect(screen);

        return true;
    }


    bool create_fullscreen(Window& window, cstr title, Vec2Du32 pixel_size, Rotate rotate)
    {
        if (rotate == Rotate::None)
        {
            window_crash("*** Window rotate must be specified ***");
            return false;
        }

        auto data = allocate_screen_memory();
        if (!data)
        {
            window_crash("*** SCREEN NOT ALLOCATED ***");
            return false;
        }

        auto& screen = *data;

        if (!rlcpp::create_screen_memory_fullscreen(screen, title, pixel_size))
        {
            return false;
        }

        window.handle = (u64)data;
        set_window_pixels(screen, window);
        resize_render_rect(screen, rotate);

        return true;
    }


    void set_window_icon(Window& window, Icon64 const& icon)
    {
        rl::Image rimage{};
        rimage.width = icon.width;
        rimage.height = icon.height;
        rimage.data = (void*)icon.pixel_data;
        rimage.mipmaps = 1;

        rl::SetWindowIcon(rimage);
    }


    void destroy(Window& window)
    {
        auto& screen = get_screen(window);
        rlcpp::destroy_screen_memory(screen);       

        window = {0};
    }


    bool resize_pixel_buffer(Window& window, u32 width, u32 height)
    {
        auto& screen = get_screen(window);
        auto& pixels = screen.screen_pixels;

        if ((int)width == pixels.width && (int)height == pixels.height)
        {
            return true;
        }

        rlcpp::destroy_screen_texture(screen);
        if (!rlcpp::create_screen_texture(screen, width, height))
        {
            return false;
        }

        set_window_pixels(screen, window);

        return true;;
    }


    void render(Window const& window, b32 size_changed = 0)
    {
        auto& screen = get_screen(window);

        if (size_changed)
        {
            resize_render_rect(screen);
        }

        render_screen_memory(screen);
    }


    void render(Window const& window, Rotate rotate, b32 size_changed = 0)
    {
        auto& screen = get_screen(window);

        if (size_changed)
        {
            resize_render_rect(screen, rotate);
        }

        render_screen_memory(screen);
    }


    void hide_mouse_cursor()
    {
        rl::HideCursor();
    }


    void show_mouse_cursor()
    {
        rl::ShowCursor();
    }
}
```