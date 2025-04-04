#include "app.hpp"
#include "../../../libs/util/numeric.hpp"

#include "assets.cpp"


namespace game_io_test
{
    namespace num = numeric;


    class MaskView
    {
    public:
        img::GraySubView mask;
        img::SubView out;
    };


    class ScreenView
    {
    public:

        assets::controller::ControllerDef<MaskView> controller1;
        assets::controller::ControllerDef<MaskView> controller2;
        assets::keyboard::KeyboardDef<MaskView> keyboard;
        assets::mouse::MouseDef<MaskView> mouse;
    };
    


    static Vec2Du32 screen_dimensions(assets::DrawMaskData const& masks)
    {
        /*
        | ctlr ctlr |
        | kbd  mse  |
        */

        auto& c = masks.controller.all;
        auto& k = masks.keyboard.all;
        auto& m = masks.mouse.all;

        auto w = num::max(c.width * 2, k.width + m.width);
        auto h = num::max(c.height + k.height, c.height + m.height);

        return { w, h };
    }


    static void set_screen_view(assets::DrawMaskData const& masks, img::ImageView const& screen, ScreenView& view)
    {
        auto& c = masks.controller.all;
        auto& k = masks.keyboard.all;
        auto& m = masks.mouse.all;

        auto sw = screen.width;
        auto sh = screen.height;
        auto cw = c.width;
        auto ch = c.height;
        auto kw = k.width;
        auto kh = k.height;
        auto mw = m.width;
        auto mh = m.height;

        auto cv1 = img::sub_view(screen, img::make_rect(0, 0, cw, ch));
        auto cv2 = img::sub_view(screen, img::make_rect(sw - cw, 0, cw, ch));
        auto kv = img::sub_view(screen, img::make_rect(0, sh - kh, kw, kh));
        auto mv = img::sub_view(screen, img::make_rect(sw - mw, sh - mh, mw, mh));

        auto cr = assets::controller::get_region_rects();
        auto kr = assets::keyboard::get_region_rects();
        auto mr = assets::mouse::get_region_rects();

        auto const set_mask_regions = [](auto const& m, auto& mv)
        {
            static_assert(m.count == mv.count);
            for (u32 i = 0; i < mv.count; i++)
            {
                mv.list[i].mask = m.list[i];
            }
        };

        auto const set_out_regions = [](auto const& view, auto const& reg, auto& mv)
        {
            static_assert(reg.count == mv.count);
            for (u32 i = 0; i < mv.count; i++)
            {
                mv.list[i].out = img::sub_view(view, reg.list[i]);
            }
        };

        set_mask_regions(masks.controller, view.controller1);
        set_mask_regions(masks.controller, view.controller2);
        set_mask_regions(masks.keyboard, view.keyboard);
        set_mask_regions(masks.mouse, view.mouse);

        set_out_regions(cv1, cr, view.controller1);
        set_out_regions(cv2, cr, view.controller2);
        set_out_regions(kv, kr, view.keyboard);
        set_out_regions(mv, mr, view.mouse);
    }
}


/* draw */

namespace game_io_test
{
    using p32 = img::Pixel;

    constexpr auto COLOR_BLACK = img::to_pixel(0);
    constexpr auto COLOR_TRANSPARENT = img::to_pixel(0, 0, 0, 0);
    constexpr auto COLOR_BACKGROUND = img::to_pixel(200);
    constexpr auto COLOR_ON = img::to_pixel(50, 255, 50);
    constexpr auto COLOR_OFF = img::to_pixel(127);


    static p32 mask_set_on(u8 m, p32 d)
    {
        switch (m)
        {
        case 1: return COLOR_BLACK;
        case 2: return COLOR_ON;
        default: return d;
        }
    }


    static p32 mask_set_off(u8 m, p32 d)
    {
        switch (m)
        {
        case 1: return COLOR_BLACK;
        case 2: return COLOR_OFF;
        default: return d;
        }
    }


    static void draw(MaskView const& view, b8 is_on)
    {
        constexpr p32 on = img::to_pixel(50, 255, 50);
        constexpr p32 off = img::to_pixel(127);

        auto f = is_on ? mask_set_on : mask_set_off;

        img::transform(view.mask, view.out, f);
    }


    static void draw(ScreenView const& screen)
    {
        auto const draw_masks = [](auto const& m)
        {
            for (u32 i = 0; i < m.count; i++)
            {
                draw(m.list[i], 0); // TODO
            }
        };

        draw_masks(screen.controller1);
        draw_masks(screen.controller2);
        draw_masks(screen.keyboard);
        draw_masks(screen.mouse);
    }
}


namespace game_io_test
{
    class StateData
    {
    public:
        assets::DrawMaskData masks;
        ScreenView screen_view;
    };


    static StateData& get_data(AppState& state)
    {
        return *state.data;
    }


    static void destroy_state_data(AppState& state)
    {
        auto& data = get_data(state);

        assets::destroy(data.masks);
        mem::free(state.data);
    }


    static bool create_state_data(AppState& state)
    {
        auto data = mem::alloc<StateData>("StateData");
        if (!data)
        {
            return false;
        }

        state.data = data;

        data->masks = assets::create_draw_mask_data();

        return true;
    }


    static void reset_data(StateData& data)
    {
        
    }
}


/* api */

namespace game_io_test
{
    AppResult init(AppState& state)
    {
        AppResult res{};
        res.success = create_state_data(state);
        
        if (!res.success)
        {
            return res;
        }

        auto& data = get_data(state);

        res.screen_dimensions = screen_dimensions(data.masks);

        return res;
    }


    bool set_screen_memory(AppState& state, image::ImageView screen)
    {
        auto& data = get_data(state);

        auto dim = screen_dimensions(data.masks);

        auto scale_w = dim.x / screen.width;
        auto scale_h = dim.y / screen.height;

        if ( (scale_w != scale_h) || (scale_w != 1))
        {
            return false;
        }

        state.screen = screen;

        set_screen_view(data.masks, screen, data.screen_view);

        return true;
    }


    void update(AppState& state, input::Input const& input)
    {
        auto& kbd = input.keyboard;

        auto& data = get_data(state);

        img::fill(state.screen, COLOR_BACKGROUND);
        draw(data.screen_view);
    }


    void reset(AppState& state)
    {
        auto& data = get_data(state);
        reset_data(data);

        img::fill(state.screen, img::to_pixel(0));
    }


    void close(AppState& state)
    {
        destroy_state_data(state);
    }
}