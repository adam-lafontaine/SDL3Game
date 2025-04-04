#include "app.hpp"

#include "assets.cpp"


namespace game_io_test
{
    constexpr u32 SCREEN_WIDTH = 500;
    constexpr u32 SCREEN_HEIGHT = 500;



}


namespace game_io_test
{
    class StateData
    {
    public:
        assets::DrawMaskData masks;
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

        res.screen_dimensions.x = SCREEN_WIDTH;
        res.screen_dimensions.y = SCREEN_HEIGHT;

        return res;
    }


    bool set_screen_memory(AppState& state, image::ImageView screen)
    {
        auto scale_w = SCREEN_WIDTH / screen.width;
        auto scale_h = SCREEN_HEIGHT / screen.height;

        if ( (scale_w != scale_h) || (scale_w != 1))
        {
            return false;
        }

        state.screen = screen;

        return true;
    }


    void update(AppState& state, input::Input const& input)
    {
        auto& kbd = input.keyboard;

        if (kbd.kbd_A.pressed)
        {
            img::fill(state.screen, img::to_pixel(255, 0, 0));
        }
        else if (kbd.kbd_S.pressed)
        {
            img::fill(state.screen, img::to_pixel(0, 255, 0));
        }
        else if (kbd.kbd_D.pressed)
        {
            img::fill(state.screen, img::to_pixel(0, 0, 255));
        }
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