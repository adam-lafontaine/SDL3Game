#pragma once

#include "../io/window.hpp"
#include "../alloc_type/alloc_type.hpp"
#include "rlcpp_include.hpp"


/* screen memory */

namespace rlcpp
{
    class ScreenMemory
    {
    public:
        
        cstr window_title = 0;

        rl::RenderTexture2D target;

        u32 width_px = 0;
        u32 height_px = 0;

        u32* data_px = 0;        
    };


    static void destroy_screen_memory(ScreenMemory& screen)
    {
        rl::UnloadRenderTexture(screen.target);
        rl::CloseWindow();
    }


    static bool create_screen_memory(ScreenMemory& screen, cstr title, Vec2Du32 window_size, Vec2Du32 pixel_size)
    {
        int window_width = window_size.x;
        int window_height = window_size.y;
        int texture_width = pixel_size.x;
        int texture_height = pixel_size.y;

        rl::SetConfigFlags(rl::FLAG_WINDOW_RESIZABLE | rl::FLAG_VSYNC_HINT);
        rl::InitWindow(window_width, window_height, title);
        //rl::SetWindowMinSize(320, 240);

        screen.target = rl::LoadRenderTexture(texture_width, texture_height);
        rl::SetTextureFilter(screen.target.texture, rl::TEXTURE_FILTER_BILINEAR);

        bool ok = true;

        ok &= rl::IsWindowReady();
        ok &= rl::IsRenderTextureValid(screen.target);

        if (!ok)
        {
            return false;
        }

        u32 width = pixel_size.x;
        u32 height = pixel_size.y;

        auto data = mem::alloc<u32>(width * height, "screen.data_px");
        if (!data)
        {
            destroy_screen_memory(screen);
            return false;
        }

        screen.data_px = data;
        screen.width_px = width;
        screen.height_px = height;

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


    static void example()
    {
        
        f32 scale = 2.0f;
        int gameScreenWidth = 640;
        int gameScreenHeight = 480;

        rl::RenderTexture2D target = rl::LoadRenderTexture(gameScreenWidth, gameScreenHeight);
        rl::SetTextureFilter(target.texture, rl::TEXTURE_FILTER_BILINEAR);

        auto sw = (float)gameScreenWidth * scale;
        auto sh = (float)gameScreenHeight * scale;

        // Draw render texture to screen, properly scaled
        rl::DrawTexturePro(
            target.texture, 
            (rl::Rectangle){ 0.0f, 0.0f, 
            (float)target.texture.width,
            (float)-target.texture.height },
            (rl::Rectangle) { 
                (rl::GetScreenWidth() - sw) * 0.5f, 
                (rl::GetScreenHeight() - sh) * 0.5f,
                sw, 
                sh 
            }, 
            (rl::Vector2) { 0, 0 }, 
            0.0f, 
            rl::WHITE);
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
            return false;
        }

        auto& screen = *data;

        if (!rlcpp::create_screen_memory(screen, title, window_size, pixel_size))
        {
            return false;
        }

        window.handle = (u64)data;
        window.pixel_buffer = screen.data_px;
        window.width_px = screen.width_px;
        window.height_px = screen.height_px;

        return true;
    }


    bool create(Window& window, cstr title, Vec2Du32 window_size, Vec2Du32 pixel_size, Rotate rotate);

    bool create_fullscreen(Window& window, cstr title, Vec2Du32 pixel_size);

    bool create_fullscreen(Window& window, cstr title, Vec2Du32 pixel_size, Rotate rotate);


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


    bool resize_pixel_buffer(Window& window, u32 width, u32 height);

    void render(Window const& window, b32 size_changed = 0);

    void render(Window const& window, Rotate rotate, b32 size_changed = 0);


    void hide_mouse_cursor()
    {
        rl::HideCursor();
    }


    void show_mouse_cursor()
    {
        rl::ShowCursor();
    }
}