#pragma once

#include "../input/input_state.hpp"
#include "../util/numeric.hpp"
#include "sdl_include.hpp"

namespace num = numeric;


void end_program();


/* joystick device */

namespace sdl
{
    template <class T, u32 CAP>
    class DeviceArray
    {
    public:
        u32 count = 0;

        T data[CAP];        
    };


    class GamepadDevice
    {
    public:
        SDL_JoystickID id = 0;
        SDL_JoystickType type = SDL_JOYSTICK_TYPE_UNKNOWN;

        SDL_Gamepad* gamepad = 0;
    };
    
    
    class JoystickDevice
    {
    public:
        SDL_JoystickID id = 0;
        SDL_JoystickType type = SDL_JOYSTICK_TYPE_UNKNOWN;

        SDL_Joystick* joystick = 0;
    };


    class InputDeviceList
    {
    public:
        DeviceArray<GamepadDevice, input::MAX_CONTROLLERS> gamepads;

        DeviceArray<JoystickDevice, input::MAX_JOYSTICKS> joysticks;
    };


    static void close_devices(InputDeviceList& devices)
    {
        for (u32 i = 0; i < devices.gamepads.count; i++)
        {
            SDL_CloseGamepad(devices.gamepads.data[i].gamepad);
        }
        
        for (u32 i = 0; i < devices.joysticks.count; i++)
        {
            SDL_CloseJoystick(devices.joysticks.data[i].joystick);
        }
    }


    static void open_devices(InputDeviceList& devices)
    {
        int cmax = (int)input::MAX_CONTROLLERS;
        int jmax = (int)input::MAX_JOYSTICKS;

        #ifdef NO_CONTROLLER
        cmax = 0;
        #endif

        #ifdef NO_JOYSTICK
        jmax = 0;
        #endif
        
        int c = 0;
        int j = 0;

        int n_connected = 0;
        auto ids = SDL_GetJoysticks(&n_connected);

        SDL_JoystickID id;
        SDL_JoystickType type;

        for (int i = 0; i < n_connected; i++)
        {
            id = ids[i];
            type = SDL_GetJoystickTypeForID(id);
            switch (type)
            {
            case SDL_JOYSTICK_TYPE_GAMEPAD:
                sdl::print_message("found a controller");
                if (c >= cmax)
                {
                    continue;
                }

                auto gp = SDL_OpenGamepad(id);
                if (!gp)
                {
                    sdl::print_message(SDL_GetError());
                    continue;
                }

                auto& gamepad = devices.gamepads.data[c];
                gamepad.id = id;
                gamepad.type = type;
                gamepad.gamepad = gp;

                ++c;

                break;

            default:
                sdl::print_message("found a joystick");
                if (j >= jmax)
                {
                    continue;
                }

                auto js = SDL_OpenJoystick(id);
                if (!js)
                {
                    sdl::print_message(SDL_GetError());
                    continue;
                }

                auto& joystick = devices.joysticks.data[j];
                joystick.id = id;
                joystick.type = type;
                joystick.joystick = js;

                ++j;

                break;
            }
        }

        devices.gamepads.count = c;
        devices.joysticks.count = j;

        SDL_free(ids);
    }
}


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


/* keyboard */

namespace sdl
{
    // https://wiki.libsdl.org/SDL3/BestKeyboardPractices

#ifndef NO_KEYBOARD

    using KeyboardInput = input::KeyboardInput;


    static void record_keycode_input(SDL_Keycode key_code, KeyboardInput const& old_keyboard, KeyboardInput& new_keyboard, bool is_down)
    {
        switch (key_code)
        {
        #if KEYBOARD_A
        case SDLK_A:
            record_button_input(old_keyboard.kbd_A, new_keyboard.kbd_A, is_down);
            break;
        #endif
        #if KEYBOARD_B
        case SDLK_B:
            record_button_input(old_keyboard.kbd_B, new_keyboard.kbd_B, is_down);
            break;
        #endif
        #if KEYBOARD_C
        case SDLK_C:
            record_button_input(old_keyboard.kbd_C, new_keyboard.kbd_C, is_down);
            break;
        #endif
        #if KEYBOARD_D
        case SDLK_D:
            record_button_input(old_keyboard.kbd_D, new_keyboard.kbd_D, is_down);
            break;
        #endif
        #if KEYBOARD_E
        case SDLK_E:
            record_button_input(old_keyboard.kbd_E, new_keyboard.kbd_E, is_down);
            break;
        #endif
        #if KEYBOARD_F
        case SDLK_F:
            record_button_input(old_keyboard.kbd_F, new_keyboard.kbd_F, is_down);
            break;
        #endif
        #if KEYBOARD_G
        case SDLK_G:
            record_button_input(old_keyboard.kbd_G, new_keyboard.kbd_G, is_down);
            break;
        #endif
        #if KEYBOARD_H
        case SDLK_H:
            record_button_input(old_keyboard.kbd_H, new_keyboard.kbd_H, is_down);
            break;
        #endif
        #if KEYBOARD_I
        case SDLK_I:
            record_button_input(old_keyboard.kbd_I, new_keyboard.kbd_I, is_down);
            break;
        #endif
        #if KEYBOARD_J
        case SDLK_J:
            record_button_input(old_keyboard.kbd_J, new_keyboard.kbd_J, is_down);
            break;
        #endif
        #if KEYBOARD_K
        case SDLK_K:
            record_button_input(old_keyboard.kbd_K, new_keyboard.kbd_K, is_down);
            break;
        #endif
        #if KEYBOARD_L
        case SDLK_L:
            record_button_input(old_keyboard.kbd_L, new_keyboard.kbd_L, is_down);
            break;
        #endif
        #if KEYBOARD_M
        case SDLK_M:
            record_button_input(old_keyboard.kbd_M, new_keyboard.kbd_M, is_down);
            break;
        #endif
        #if KEYBOARD_N
        case SDLK_N:
            record_button_input(old_keyboard.kbd_N, new_keyboard.kbd_N, is_down);
            break;
        #endif
        #if KEYBOARD_O
        case SDLK_O:
            record_button_input(old_keyboard.kbd_O, new_keyboard.kbd_O, is_down);
            break;
        #endif
        #if KEYBOARD_P
        case SDLK_P:
            record_button_input(old_keyboard.kbd_P, new_keyboard.kbd_P, is_down);
            break;
        #endif
        #if KEYBOARD_Q
        case SDLK_Q:
            record_button_input(old_keyboard.kbd_Q, new_keyboard.kbd_Q, is_down);
            break;
        #endif
        #if KEYBOARD_R
        case SDLK_R:
            record_button_input(old_keyboard.kbd_R, new_keyboard.kbd_R, is_down);
            break;
        #endif
        #if KEYBOARD_S
        case SDLK_S:
            record_button_input(old_keyboard.kbd_S, new_keyboard.kbd_S, is_down);
            break;
        #endif
        #if KEYBOARD_T
        case SDLK_T:
            record_button_input(old_keyboard.kbd_T, new_keyboard.kbd_T, is_down);
            break;
        #endif
        #if KEYBOARD_U
        case SDLK_U:
            record_button_input(old_keyboard.kbd_U, new_keyboard.kbd_U, is_down);
            break;
        #endif
        #if KEYBOARD_V
        case SDLK_V:
            record_button_input(old_keyboard.kbd_V, new_keyboard.kbd_V, is_down);
            break;
        #endif
        #if KEYBOARD_W
        case SDLK_W:
            record_button_input(old_keyboard.kbd_W, new_keyboard.kbd_W, is_down);
            break;
        #endif
        #if KEYBOARD_X
        case SDLK_X:
            record_button_input(old_keyboard.kbd_X, new_keyboard.kbd_X, is_down);
            break;
        #endif
        #if KEYBOARD_Y
        case SDLK_Y:
            record_button_input(old_keyboard.kbd_Y, new_keyboard.kbd_Y, is_down);
            break;
        #endif
        #if KEYBOARD_Z
        case SDLK_Z:
            record_button_input(old_keyboard.kbd_Z, new_keyboard.kbd_Z, is_down);
            break;
        #endif
        #if KEYBOARD_0
        case SDLK_0:
            record_button_input(old_keyboard.kbd_0, new_keyboard.kbd_0, is_down);
            break;
        #endif
        #if KEYBOARD_1
        case SDLK_1:
            record_button_input(old_keyboard.kbd_1, new_keyboard.kbd_1, is_down);
            break;
        #endif
        #if KEYBOARD_2
        case SDLK_2:
            record_button_input(old_keyboard.kbd_2, new_keyboard.kbd_2, is_down);
            break;
        #endif
        #if KEYBOARD_3
        case SDLK_3:
            record_button_input(old_keyboard.kbd_3, new_keyboard.kbd_3, is_down);
            break;
        #endif
        #if KEYBOARD_4
        case SDLK_4:
            record_button_input(old_keyboard.kbd_4, new_keyboard.kbd_4, is_down);
            break;
        #endif
        #if KEYBOARD_5
        case SDLK_5:
            record_button_input(old_keyboard.kbd_5, new_keyboard.kbd_5, is_down);
            break;
        #endif
        #if KEYBOARD_6
        case SDLK_6:
            record_button_input(old_keyboard.kbd_6, new_keyboard.kbd_6, is_down);
            break;
        #endif
        #if KEYBOARD_7
        case SDLK_7:
            record_button_input(old_keyboard.kbd_7, new_keyboard.kbd_7, is_down);
            break;
        #endif
        #if KEYBOARD_8
        case SDLK_8:
            record_button_input(old_keyboard.kbd_8, new_keyboard.kbd_8, is_down);
            break;
        #endif
        #if KEYBOARD_9
        case SDLK_9:
            record_button_input(old_keyboard.kbd_9, new_keyboard.kbd_9, is_down);
            break;
        #endif
        #if KEYBOARD_UP
        case SDLK_UP:
            record_button_input(old_keyboard.kbd_up, new_keyboard.kbd_up, is_down);
            break;
        #endif
        #if KEYBOARD_DOWN
        case SDLK_DOWN:
            record_button_input(old_keyboard.kbd_down, new_keyboard.kbd_down, is_down);
            break;
        #endif
        #if KEYBOARD_LEFT
        case SDLK_LEFT:
            record_button_input(old_keyboard.kbd_left, new_keyboard.kbd_left, is_down);
            break;
        #endif
        #if KEYBOARD_RIGHT
        case SDLK_RIGHT:
            record_button_input(old_keyboard.kbd_right, new_keyboard.kbd_right, is_down);
            break;
        #endif
        #if KEYBOARD_RETURN
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            record_button_input(old_keyboard.kbd_return, new_keyboard.kbd_return, is_down);
            break;
        #endif
        #if KEYBOARD_ESCAPE
        case SDLK_ESCAPE:
            record_button_input(old_keyboard.kbd_escape, new_keyboard.kbd_escape, is_down);
            break;
        #endif
        #if KEYBOARD_SPACE
        case SDLK_SPACE:
            record_button_input(old_keyboard.kbd_space, new_keyboard.kbd_space, is_down);
            break;
        #endif
        #if KEYBOARD_LSHIFT
        case SDLK_LSHIFT:
            record_button_input(old_keyboard.kbd_left_shift, new_keyboard.kbd_left_shift, is_down);
            break;
        #endif
        #if KEYBOARD_RSHIFT
        case SDLK_RSHIFT:
            record_button_input(old_keyboard.kbd_right_shift, new_keyboard.kbd_right_shift, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_0
        case SDLK_KP_0:
            record_button_input(old_keyboard.npd_0, new_keyboard.npd_0, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_1
        case SDLK_KP_1:
            record_button_input(old_keyboard.npd_1, new_keyboard.npd_1, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_2
        case SDLK_KP_2:
            record_button_input(old_keyboard.npd_2, new_keyboard.npd_2, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_3
        case SDLK_KP_3:
            record_button_input(old_keyboard.npd_3, new_keyboard.npd_3, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_4
        case SDLK_KP_4:
            record_button_input(old_keyboard.npd_4, new_keyboard.npd_4, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_5
        case SDLK_KP_5:
            record_button_input(old_keyboard.npd_5, new_keyboard.npd_5, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_6
        case SDLK_KP_6:
            record_button_input(old_keyboard.npd_6, new_keyboard.npd_6, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_7
        case SDLK_KP_7:
            record_button_input(old_keyboard.npd_7, new_keyboard.npd_7, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_8
        case SDLK_KP_8:
            record_button_input(old_keyboard.npd_8, new_keyboard.npd_8, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_9
        case SDLK_KP_9:
            record_button_input(old_keyboard.npd_9, new_keyboard.npd_9, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_PLUS
        case SDLK_KP_PLUS:
            record_button_input(old_keyboard.npd_plus, new_keyboard.npd_plus, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_MINUS
        case SDLK_KP_MINUS:
            record_button_input(old_keyboard.npd_minus, new_keyboard.npd_minus, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_MULTIPLY
        case SDLK_KP_MULTIPLY:
            record_button_input(old_keyboard.npd_mult, new_keyboard.npd_mult, is_down);
            break;
        #endif
        #if KEYBOARD_NUMPAD_DIVIDE
        case SDLK_KP_DIVIDE:
            record_button_input(old_keyboard.npd_div, new_keyboard.npd_div, is_down);
            break;
        #endif
        #if KEYBOARD_CTRL
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            record_button_input(old_keyboard.kbd_ctrl, new_keyboard.kbd_ctrl, is_down);
            break;
        #endif
        
        default:
            break;
        }
    }


    static void record_scancode_input(SDL_Scancode scan_code, KeyboardInput const& old_keyboard, KeyboardInput& new_keyboard, bool is_down)
    {
        // TODO
        // https://wiki.libsdl.org/SDL3/SDL_Scancode

        switch (scan_code)
        {
        #if KEYBOARD_A
        case SDL_SCANCODE_A:
            //record_button_input(old_keyboard.kbd_A, new_keyboard.kbd_A, is_down);
            break;
        #endif
        }
    }

#endif


    void record_keyboard_input(SDL_Event const& event, KeyboardInput const& old_keyboard, KeyboardInput& new_keyboard)
    {
    #ifndef NO_KEYBOARD

        SDL_Keycode key_code;
        SDL_Scancode scan_code;

        switch (event.type)
        {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
        {
            if (event.key.repeat)
            {
                return;
            }

            bool is_down = event.type == SDL_EVENT_KEY_DOWN;

            key_code = event.key.key;
            scan_code = event.key.scancode;

            record_keycode_input(key_code, old_keyboard, new_keyboard, is_down);
            record_scancode_input(scan_code, old_keyboard, new_keyboard, is_down);
        } break;
        }

        set_is_active(new_keyboard);

    #endif
    }
}


/* mouse */

namespace sdl
{
#ifndef NO_MOUSE
    using MouseInput = input::MouseInput;


    static void record_mouse_button_input(Uint8 button_code, MouseInput const& old_mouse, MouseInput& new_mouse, bool is_down)
    {
        switch(button_code)
        {		
        #if MOUSE_LEFT
            case SDL_BUTTON_LEFT:
            {
                record_button_input(old_mouse.btn_left, new_mouse.btn_left, is_down);
            } break;
        #endif
        #if MOUSE_RIGHT
            case SDL_BUTTON_RIGHT:
            {
                record_button_input(old_mouse.btn_right, new_mouse.btn_right, is_down);
            } break;
        #endif
        #if MOUSE_MIDDLE
            case SDL_BUTTON_MIDDLE:
            {
                record_button_input(old_mouse.btn_middle, new_mouse.btn_middle, is_down);
            } break;
        #endif
        #if MOUSE_X1
            case SDL_BUTTON_X1:
            {
                record_button_input(old_mouse.btn_x1, new_mouse.btn_x1, is_down);
            } break;
        #endif
        #if MOUSE_X2
            case SDL_BUTTON_X2:
            {
                record_button_input(old_mouse.btn_x1, new_mouse.btn_x1, is_down);
            } break;
        #endif
        }
    }


    static void record_mouse_position_input(MouseInput& mouse, SDL_MouseMotionEvent const& motion)
    {
    #if MOUSE_POSITION
        mouse.window_pos.x = motion.x;
        mouse.window_pos.y = motion.y;
    #endif
    }


    static void record_mouse_wheel_input(MouseInput& mouse, SDL_MouseWheelEvent const& wheel)
    {
    #if MOUSE_WHEEL
        mouse.wheel.x = wheel.x;
        mouse.wheel.y = wheel.y;
    #endif
    }


#endif


    static void record_mouse_input(SDL_Event const& event, MouseInput const& old_mouse, MouseInput& new_mouse)
    {
    #ifndef NO_MOUSE

        auto& mouse = new_mouse;

        switch (event.type)
        {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
        {
            bool is_down = event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
            auto button_code = event.button.button;

            record_mouse_button_input(button_code, old_mouse, mouse, is_down);
        } break;

        #if MOUSE_POSITION
        case SDL_EVENT_MOUSE_MOTION:
        { 
            record_mouse_position_input(mouse, event.motion); 
        } break;
        #endif

        #if MOUSE_WHEEL
        case SDL_EVENT_MOUSE_WHEEL:
        { 
            record_mouse_wheel_input(mouse, event.wheel); 
        } break;
        #endif
            
        }

        set_is_active(mouse);

    #endif
    }
}


/* joystick */

namespace sdl
{
    
}