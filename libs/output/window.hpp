#pragma once

#include "../util/types.hpp"


namespace window
{
    class Icon64
    {
    public:
        static constexpr u32 width = 64;
        static constexpr u32 height = 64;
        static constexpr u32 bytes_per_pixel = 4;

        static constexpr u32 min_data_size = width * height * bytes_per_pixel;

        u8* pixel_data = 0;
    };


    class Window
    {
    public:
        u32 width = 0;
        u32 height = 0;

        u32* pixel_buffer = 0;

        u64 handle = 0;
    };
}


namespace window
{
    bool init();

    void close();

    bool create(Window& window, cstr title, u32 width, u32 height, Icon64 const& icon);

    bool create(Window& window, cstr title, u32 width, u32 height);

    void destroy(Window& window);

    bool resize_pixel_buffer(Window& window, u32 width, u32 height);

    void render(Window const& window);
}