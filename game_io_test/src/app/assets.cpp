#pragma once

#include "app.hpp"
#include "../../../libs/output/audio.hpp"


/* images */

namespace game_io_test
{


/* controller */

namespace assets
{
namespace controller
{
    template <class T>
    class ControllerDef
    {
    public:
        static constexpr u32 count = 16;

        union
        {
            T list[count];

            struct
            {
                //T all;

                T dpad_up;
                T dpad_down;
                T dpad_left;
                T dpad_right;

                T a;
                T b;
                T x;
                T y;

                T start;
                T back;

                T shoulder_left;
                T shoulder_right;

                T stick_left;
                T stick_right;

                T trigger_left;
                T trigger_right;
            };
        };
    };


    using MaskList = ControllerDef<img::GraySubView>;
    using RectList = ControllerDef<Rect2Du32>;


    static RectList get_region_rects()
    {
        RectList r{};

        r.trigger_left  = img::make_rect(17, 4, 19, 15);
        r.trigger_right = img::make_rect(156, 4, 19, 15);

        r.shoulder_left  = img::make_rect(17, 21, 19, 8);
        r.shoulder_right = img::make_rect(156, 21, 19, 8);

        r.back  = img::make_rect(74, 23, 16, 9);
        r.start = img::make_rect(102, 23, 16, 9);

        r.dpad_up    = img::make_rect(21, 32, 10, 17);
        r.dpad_down  = img::make_rect(21, 60, 10, 17);
        r.dpad_left  = img::make_rect(5, 50, 17, 10);
        r.dpad_right = img::make_rect(32, 50, 17, 10);

        r.stick_left  = img::make_rect(59, 44, 25, 25);
        r.stick_right = img::make_rect(108, 44, 25, 25);

        r.a = img::make_rect(158, 62, 15, 15);
        r.b = img::make_rect(173, 47, 15, 15);
        r.x = img::make_rect(143, 47, 15, 15);
        r.y = img::make_rect(158, 32, 15, 15);

        return r;
    }


    static img::GrayView make_mask(img::Buffer8& buffer)
    {
    #include "../res/masks/controller.cpp"

        auto w = controller.width;
        auto h = controller.height;

        auto src = span::make_view((u8*)controller.data, w * h);
        
        auto mask = img::make_view(w, h, buffer);

        span::copy(src, img::to_span(mask));

        return mask;
    }
}
}


/* keyboard */

namespace assets
{
namespace keyboard
{
    template <class T>
    class KeyboardDef
    {
    public:
        static constexpr u32 count = 9;

        union
        {
            T list[count];

            struct
            {
                //T all;

                T n_1;
                T n_2;
                T n_3;
                T n_4;

                T w;
                T a;
                T s;
                T d;

                T space;
            };
        };
    };


    using MaskList = KeyboardDef<img::GraySubView>;
    using RectList = KeyboardDef<Rect2Du32>;


    static RectList get_region_rects()
    {
        RectList r{};

        r.n_1 = img::make_rect(20, 2, 16, 16);
        r.n_2 = img::make_rect(38, 2, 16, 16);
        r.n_3 = img::make_rect(56, 2, 16, 16);
        r.n_4 = img::make_rect(74, 2, 16, 16);

        r.w = img::make_rect(47, 20, 16, 16);
        r.a = img::make_rect(34, 38, 16, 16);
        r.s = img::make_rect(52, 38, 16, 16);
        r.d = img::make_rect(70, 38, 16, 16);

        r.space = img::make_rect(83, 74, 106, 16);

        return r;
    }


    static img::GrayView make_mask(img::Buffer8& buffer)
    {
    #include "../res/masks/keyboard.cpp"

        auto w = keyboard.width;
        auto h = keyboard.height;

        auto src = span::make_view((u8*)keyboard.data, w * h);

        auto mask = img::make_view(w, h, buffer);

        span::copy(src, img::to_span(mask));

        return mask;
    }
    
}
}


/* mouse */

namespace assets
{
namespace mouse
{
    template <class T>
    class MouseDef
    {
    public:
        static constexpr u32 count = 5;

        union
        {
            T list[count];

            struct
            {
                //T all;

                T left;
                T right;
                T middle;

                T pos_x;
                T pos_y;
            };
        };
    };


    using MaskList = MouseDef<img::GraySubView>;
    using RectList = MouseDef<Rect2Du32>;


    static RectList get_region_rects()
    {
        RectList r{};

        r.left   = img::make_rect(1, 1, 30, 30);
        r.right  = img::make_rect(49, 1, 30, 30);
        r.middle = img::make_rect(32, 1, 14, 30);
        r.pos_x  = img::make_rect(6, 44, 68, 8);
        r.pos_y  = img::make_rect(6, 56, 68, 8);

        return r;
    }


    static img::GrayView make_mask(img::Buffer8& buffer)
    {
    #include "../res/masks/mouse.cpp"

        auto w = mouse.width;
        auto h = mouse.height;

        auto src = span::make_view((u8*)mouse.data, w * h);

        auto mask = img::make_view(w, h, buffer);

        span::copy(src, img::to_span(mask));

        return mask;
    }
}
}


namespace assets
{
    class DrawMaskData
    {
    public:

        controller::MaskList controller;
        keyboard::MaskList keyboard;
        mouse::MaskList mouse;

        img::GrayView controller_view;
        img::GrayView keyboard_view;
        img::GrayView mouse_view;
    };


    static u32 draw_mask_size()
    {
    #include "../res/masks/mask_sizes.cpp"

        auto c = mask_sizes.controller;
        auto k = mask_sizes.keyboard;
        auto m = mask_sizes.mouse;        

        auto cn = c.width * c.height;
        auto kn = k.width * k.height;
        auto mn = m.width * m.height;

        return cn + kn + mn;
    }


    static DrawMaskData create_draw_mask_data(img::Buffer8& buffer)
    {
        auto cr = controller::get_region_rects();
        auto kr = keyboard::get_region_rects();
        auto mr = mouse::get_region_rects();

        DrawMaskData data;

        auto cmv = controller::make_mask(buffer);
        auto kmv = keyboard::make_mask(buffer);
        auto mmv = mouse::make_mask(buffer);

        auto const set_mask_regions = [](auto const& view, auto const& reg, auto& mask)
        {
            static_assert(reg.count == mask.count);
            for (u32 i = 0; i < reg.count; i++)
            {
                mask.list[i] = img::sub_view(view, reg.list[i]);
            }
        };

        set_mask_regions(cmv, cr, data.controller);
        set_mask_regions(kmv, kr, data.keyboard);
        set_mask_regions(mmv, mr, data.mouse);

        data.controller_view = cmv;
        data.keyboard_view = kmv;
        data.mouse_view = mmv;

        return data;
    }
}


} // game_io_test


/* sounds */

namespace game_io_test
{
    using Sound = audio::Sound;    


    class SoundList
    {
    public:
        static constexpr u32 count = 4;

        union
        {
            Sound list[count];

            struct
            {
                Sound laser;
                Sound explosion;
                Sound ui_confirm;
                Sound ui_select;
            };
        };
    };


namespace sound
{
    static inline bool load_sound(auto const& src, audio::Sound& sound, cstr mem_tag)
    {
        auto bytes = span::make_view((u8*)src.data, src.length);

        return audio::load_sound_from_bytes(bytes, sound, mem_tag);
    }
}    

} // game_io_test


/* music */

namespace game_io_test
{
    using Music = audio::Music;


    class MusicList
    {
    public:
        static constexpr u32 count = 4;

        union
        {
            Music list[count];

            struct
            {
                Music game_00;
                Music game_01;
                Music game_02;
                Music game_03;
            };
        };
    };
}