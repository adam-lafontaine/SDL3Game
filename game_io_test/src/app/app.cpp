#include "app.hpp"
#include "../../../libs/util/numeric.hpp"

#include "assets.cpp"


/* definitions */

namespace game_io_test
{
    namespace num = numeric;

    using Input = input::Input;

    template <typename T>
    using ControllerDef = assets::controller::ControllerDef<T>;

    template <typename T>
    using KeyboardDef = assets::keyboard::KeyboardDef<T>;

    template <typename T>
    using MouseDef = assets::mouse::MouseDef<T>;
}


/* mask view */

namespace game_io_test
{
    class MaskView
    {
    public:
        img::GraySubView mask;
        img::SubView out;
    };


    class MaskViewList
    {
    public:

        ControllerDef<MaskView> controller1;
        ControllerDef<MaskView> controller2;
        KeyboardDef<MaskView> keyboard;
        MouseDef<MaskView> mouse;
    };    


    static Vec2Du32 app_screen_dimensions(assets::DrawMaskData const& masks)
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


    static void set_mask_views(assets::DrawMaskData const& masks, img::ImageView const& out, MaskViewList& mv)
    {
        auto& c = masks.controller.all;
        auto& k = masks.keyboard.all;
        auto& m = masks.mouse.all;

        auto sw = out.width;
        auto sh = out.height;
        auto cw = c.width;
        auto ch = c.height;
        auto kw = k.width;
        auto kh = k.height;
        auto mw = m.width;
        auto mh = m.height;

        auto cview1 = img::sub_view(out, img::make_rect(0, 0, cw, ch));
        auto cview2 = img::sub_view(out, img::make_rect(sw - cw, 0, cw, ch));
        auto kview = img::sub_view(out, img::make_rect(0, sh - kh, kw, kh));
        auto mview = img::sub_view(out, img::make_rect(sw - mw, sh - mh, mw, mh));

        auto creg = assets::controller::get_region_rects();
        auto kreg = assets::keyboard::get_region_rects();
        auto mreg = assets::mouse::get_region_rects();

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

        set_mask_regions(masks.controller, mv.controller1);
        set_mask_regions(masks.controller, mv.controller2);
        set_mask_regions(masks.keyboard, mv.keyboard);
        set_mask_regions(masks.mouse, mv.mouse);

        set_out_regions(cview1, creg, mv.controller1);
        set_out_regions(cview2, creg, mv.controller2);
        set_out_regions(kview, kreg, mv.keyboard);
        set_out_regions(mview, mreg, mv.mouse);
    }
}


/* update */

namespace game_io_test
{
    class InputList
    {
    public:

        ControllerDef<b8> controller1;
        ControllerDef<b8> controller2;
        KeyboardDef<b8> keyboard;
        MouseDef<b8> mouse;
    };


    static void clear_input_list(InputList& inputs)
    {
        auto const& clear = [](auto& in)
        {
            for (u32 i = 0; i < in.count; i++)
            {
                in.list[i] = 0;
            }
        };

        clear(inputs.controller1);
        clear(inputs.controller2);
        clear(inputs.keyboard);
        clear(inputs.mouse);
    }


    static void map_controller(input::ControllerInput const& src, ControllerDef<b8>& dst)
    {
        dst.dpad_up = src.btn_dpad_up.is_down;
        dst.dpad_down = src.btn_dpad_down.is_down;
        dst.dpad_left = src.btn_dpad_left.is_down;
        dst.dpad_right = src.btn_dpad_right.is_down;
    }


    static void update(Input const& src, InputList& dst)
    {
        map_controller(src.controllers[0], dst.controller1);
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


    static void draw(MaskViewList const& mv, InputList const& input)
    {
        auto const draw_masks = [](auto const& m, auto const& in)
        {
            static_assert(m.count == in.count);
            for (u32 i = 0; i < m.count; i++)
            {
                draw(m.list[i], in.list[i]);
            }
        };

        draw_masks(mv.controller1, input.controller1);
        draw_masks(mv.controller2, input.controller2);
        draw_masks(mv.keyboard, input.keyboard);
        draw_masks(mv.mouse, input.mouse);
    }
}


/* state */

namespace game_io_test
{
    class StateData
    {
    public:
        assets::DrawMaskData masks;
        MaskViewList mask_views;
        InputList inputs;

        img::ImageView out_src;
        img::SubView out_dst;

        u32 out_scale = 1;

        img::Buffer32 buffer32;
        img::Buffer8 buffer8;
    };


    static StateData& get_data(AppState& state)
    {
        return *state.data;
    }


    static void destroy_state_data(AppState& state)
    {
        auto& data = get_data(state);

        mb::destroy_buffer(data.buffer32);
        mb::destroy_buffer(data.buffer8);
        mem::free(state.data);
    }


    static bool create_state_data(AppState& state)
    {
        auto state_data = mem::alloc<StateData>("StateData");
        if (!state_data)
        {
            return false;
        }

        state.data = state_data;

        auto& data = get_data(state);

        data.buffer8 = img::create_buffer8(assets::draw_mask_size(), "buffer8");
        if (!data.buffer8.ok)
        {
            return false;
        }

        data.masks = assets::create_draw_mask_data(data.buffer8);

        auto dim = app_screen_dimensions(data.masks);
        data.buffer32 = img::create_buffer32(dim.x * dim.y, "buffer32");
        if (!data.buffer32.ok)
        {
            return false;
        }

        data.out_src = img::make_view(dim.x, dim.y, data.buffer32);
        set_mask_views(data.masks, data.out_src, data.mask_views);

        clear_input_list(data.inputs);

        return true;
    }


    static void reset_data(StateData& data)
    {
        clear_input_list(data.inputs);
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

        res.screen_dimensions = app_screen_dimensions(data.masks);

        return res;
    }


    bool set_screen_memory(AppState& state, image::ImageView screen)
    {
        state.screen = screen;

        auto& data = get_data(state);

        auto dim = app_screen_dimensions(data.masks);

        auto scale_w = screen.width / dim.x;
        auto scale_h = screen.height / dim.y;

        auto scale = num::min(scale_w, scale_h);

        auto w = dim.x * scale;
        auto h = dim.y * scale;

        auto x = (screen.width - w) / 2;
        auto y = (screen.height - h) / 2;

        auto r = img::make_rect(x, y, w, h);

        data.out_dst = img::sub_view(screen, r);
        data.out_scale = scale;

        return true;
    }


    void update(AppState& state, Input const& input)
    {
        auto& kbd = input.keyboard;

        auto& data = get_data(state);

        update(input, data.inputs);

        img::fill(data.out_src, COLOR_BACKGROUND);

        draw(data.mask_views, data.inputs);
        img::scale_up(data.out_src, data.out_dst, data.out_scale);
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