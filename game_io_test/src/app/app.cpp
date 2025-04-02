#include "app.hpp"


namespace game_io_test
{
    namespace img = image;

    constexpr u32 SCREEN_WIDTH = 500;
    constexpr u32 SCREEN_HEIGHT = 500;
}


namespace game_io_test
{
    class StateData
    {
    public:
        u32 placeholder = 0;
    };


    static void destroy_state_data(AppState& state)
    {
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

        return true;
    }


    StateData& get_data(AppState& state)
    {
        return *state.data;
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
        // TODO
    }


    void close(AppState& state)
    {
        destroy_state_data(state);
    }
}