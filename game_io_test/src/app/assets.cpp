#pragma once

#include "app.hpp"


namespace game_io_test
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
                T dpad_up;
                T dpad_down;
                T dpad_left;
                T dpad_right;

                T start;
                T back;

                T a;
                T b;
                T x;
                T y;

                T shoulder_left;
                T shoulder_right;

                T stick_left;
                T stick_right;

                T trigger_left;
                T trigger_right;
            };
        };
    };


    using RegionList = ControllerDef<img::SubView>;
    using RectList = ControllerDef<Rect2Du32>;


    static RectList get_region_rects()
    {
        RectList r{};

        r.trigger_left  = img::make_rect(17, 4, 19, 15);
        r.trigger_right = img::make_rect(156, 4, 19, 15);

        r.shoulder_left  = img::make_rect(17, 21, 19, 8);
        r.shoulder_right = img::make_rect(156, 21, 19, 8);

        r.start = img::make_rect(74, 23, 16, 9);
        r.back  = img::make_rect(102, 23, 16, 9);

        r.dpad_up    = img::make_rect(21, 32, 10, 17);
        r.dpad_down  = img::make_rect(21, 60, 10, 17);
        r.dpad_left  = img::make_rect(5, 50, 17, 10);
        r.dpad_right = img::make_rect(32, 50, 17, 10);

        r.stick_left  = img::make_rect(59, 44, 25, 25);
        r.stick_right = img::make_rect(108, 44, 25, 25);

        r.a = img::make_rect(158, 62, 15, 15);
        r.b = img::make_rect(143, 47, 15, 15);
        r.x = img::make_rect(173, 47, 15, 15);
        r.y = img::make_rect(158, 32, 15, 15);

        return r;
    }
}


    class ControllerView
    {
    public:
        img::SubView view;

        controller::RegionList regions;
    };


    static void set_regions(ControllerView& v)
    {
        auto r = controller::get_region_rects();
        static_assert(r.count == v.regions.count);

        for (u32 i = 0; i < r.count; i++)
        {
            v.regions.list[i] = img::sub_view(v.view, r.list[i]);
        }
    }
}


namespace game_io_test
{
namespace keyboard
{
    template <class T>
    class KeyboardDef
    {
    public:
        static constexpr u32 count = 5;

        union
        {
            T list[count];

            struct
            {
                T w;
                T a;
                T s;
                T d;
                T space;
            };
        };
    };


    using RegionList = KeyboardDef<img::SubView>;
    using RectList = KeyboardDef<Rect2Du32>;


    static RectList get_region_rects()
    {
        RectList r{};

        r.w = img::make_rect(47, 20, 16, 16);
        r.a = img::make_rect(34, 38, 16, 16);
        r.s = img::make_rect(52, 38, 16, 16);
        r.d = img::make_rect(70, 38, 16, 16);

        r.space = img::make_rect(83, 74, 106, 16);

        return r;
    }
}


    class KeyboardView
    {
    public:
        img::SubView view;

        keyboard::RegionList regions;        
    };


    static void set_regions(KeyboardView& v)
    {
        auto r = keyboard::get_region_rects();
        static_assert(r.count == v.regions.count);

        for (u32 i = 0; i < r.count; i++)
        {
            v.regions.list[i] = img::sub_view(v.view, r.list[i]);
        }
    }
}


namespace game_io_test
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
                T left;
                T right;
                T middle;
                T pos_x;
                T pos_y;
            };
        };
    };


    using RegionList = MouseDef<img::SubView>;
    using RectList = MouseDef<Rect2Du32>;


    static RectList get_region_rects()
    {
        RectList r{};

        r.left   = img::make_rect(1, 1, 30, 30);
        r.right  = img::make_rect(49, 1, 30, 30);
        r.middle = img::make_rect(32, 1, 14, 30);
        r.pos_x  = img::make_rect(4, 37, 72, 16);
        r.pos_y  = img::make_rect(4, 55, 72, 16);

        return r;
    }
}


    class MouseView
    {
    public:
        img::SubView view;

        mouse::RegionList regions;        
    };


    static void set_regions(MouseView& v)
    {
        auto r = mouse::get_region_rects();
        static_assert(r.count == v.regions.count);

        for (u32 i = 0; i < r.count; i++)
        {
            v.regions.list[i] = img::sub_view(v.view, r.list[i]);
        }
    }
}