#include "../input/input_state.hpp"
#include "../util/numeric.hpp"
#include "sdl_include.hpp"


namespace num = numeric;


/* helpers */

namespace sdl
{
    static f32 normalize_axis_value(Sint16 axis)
    {
        constexpr num::MinMax<Sint16> mm_axis = {
            .min = SDL_JOYSTICK_AXIS_MIN,
            .max = SDL_JOYSTICK_AXIS_MAX
        };

        constexpr num::MinMax<f32> mm_res = {
            .min = -1.0f,
            .max = 1.0f
        };

        f32 norm = num::lerp(axis, mm_axis, mm_res);

        return num::abs(norm) < 0.3f ? 0.0f : norm;
    }
}


#include "sdl_joystick.cpp"
#include "sdl_keyboard.cpp"
#include "sdl_mouse.cpp"


namespace sdl
{
    static void handle_sdl_event(SDL_Event const& event, input::Input& input)
    {
        static bool is_fullscreen = false;

        SDL_Keycode key_code;
        SDL_Scancode scan_code;

        switch (event.type)
        {
        case SDL_EVENT_WINDOW_RESIZED:
            input.window_size_changed = 1;
            break;

        case SDL_EVENT_QUIT:
            print_message("SDL_QUIT");
            end_program();
            break;

        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
        {
            key_code = event.key.key;
            scan_code = event.key.scancode;

            auto alt = event.key.mod & SDL_KMOD_ALT;

            if (alt)
            {
                switch (key_code)
                {
                case SDLK_F4:
                    print_message("ALT F4");
                    end_program();
                    break;

                #ifndef NDEBUG
                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    print_message("ALT ENTER");
                    is_fullscreen = !is_fullscreen;                    
                    SDL_SetWindowFullscreen(SDL_GetWindowFromID(event.window.windowID), is_fullscreen);
                    input.window_size_changed = 1;
                    break;
                #endif
                }
            }
            
            switch (key_code)
            {

            #ifndef NDEBUG
            case SDLK_ESCAPE:
                print_message("ESC");
                end_program();
                break;

            #endif


            default:
                break;
            }
            

        } break;
        }
    }
}



/* api */

namespace input
{
    static constexpr Uint32 subsystem_flags()
    {
        auto gamepad = (N_GAMEPAD_BUTTONS || N_GAMEPAD_AXES) ? SDL_INIT_GAMEPAD : 0u;
        auto joystick = (N_JOYSTICK_BUTTONS || N_JOYSTICK_AXES) ? SDL_INIT_JOYSTICK : 0u;

        return gamepad | joystick;
    }


    bool init(InputArray& inputs)
    {
        auto error = SDL_InitSubSystem(subsystem_flags());
        if (error)
        {
            sdl::print_error("Init Input failed");
            return false;
        }

        reset_input_state(inputs.prev());
        reset_input_state(inputs.curr());

        sdl::open_input_devices();

        return true;
    }


    void close()
    {
        sdl::close_input_devices();
        SDL_QuitSubSystem(subsystem_flags());
    }


    void record_input(InputArray& inputs)
    {
        auto& prev = inputs.prev();
        auto& curr = inputs.curr();

        copy_input_state(prev, curr);
        curr.frame = prev.frame + 1;
        curr.dt_frame = 1.0f / 60.0f; // TODO
        curr.window_size_changed = 0;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            sdl::handle_sdl_event(event, curr);
            sdl::record_keyboard_input(event, prev.keyboard, curr.keyboard);
            sdl::record_mouse_input(event, prev.mouse, curr.mouse);
            sdl::record_gamepad_input(event, prev, curr);
            sdl::record_joystick_input(event, prev, curr);
        }

        set_is_active(curr);
    }
}