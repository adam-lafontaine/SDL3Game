/* devices */

namespace sdl
{
    template <class T, u32 CAP>
    class DeviceArray
    {
    public:
        static constexpr u32 capacity = CAP;

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


    using GamepadDeviceArray = DeviceArray<GamepadDevice, input::MAX_CONTROLLERS>;
    using JoystickDeviceArray = DeviceArray<JoystickDevice, input::MAX_JOYSTICKS>;


    class InputDeviceList
    {
    public:
        GamepadDeviceArray gamepads;

        JoystickDeviceArray joysticks;
    };

}


/* close devices */

namespace sdl
{
    static void close_device(GamepadDevice& device)
    {
        if (device.gamepad)
        {
            SDL_CloseGamepad(device.gamepad);
            device.gamepad = 0;
        }

        device.id = 0;
        device.type = SDL_JOYSTICK_TYPE_UNKNOWN;
    }


    static void close_device(JoystickDevice& device)
    {
        if (device.joystick)
        {
            SDL_CloseJoystick(device.joystick);
            device.joystick = 0;
        }

        device.id = 0;
        device.type = SDL_JOYSTICK_TYPE_UNKNOWN;
    }


    static void close_devices(InputDeviceList& devices)
    {
        for (u32 i = 0; i < devices.gamepads.capacity; i++)
        {
            close_device(devices.gamepads.data[i]);
        }
        
        for (u32 i = 0; i < devices.joysticks.capacity; i++)
        {
            close_device(devices.joysticks.data[i]);
        }
    }
}


/* add devices*/

namespace sdl
{
    static void add_gamepad_device(GamepadDeviceArray& devices, SDL_JoystickID id, SDL_JoystickType type)
    {
    #ifndef NO_CONTROLLER

        if (!SDL_IsGamepad(id))
        {
            sdl::print_message("not a gamepad");
            return;
        }

        auto gp = SDL_OpenGamepad(id);
        if (!gp)
        {
            sdl::print_message(SDL_GetError());
            return;
        }

        for (u32 i = 0; i < devices.capacity; i++)
        {
            auto& device = devices.data[i];
            if (device.gamepad)
            {
                continue;
            }

            device.id = id;
            device.type = type;
            device.gamepad = gp;
        }

    #endif
    }


    static void add_joystick_device(JoystickDeviceArray& devices, SDL_JoystickID id, SDL_JoystickType type)
    {
    #ifndef NO_JOYSTICK

        auto js = SDL_OpenJoystick(id);
        if (!js)
        {
            sdl::print_message(SDL_GetError());
            return;
        }

        for (u32 i = 0; i < devices.capacity; i++)
        {
            auto& device = devices.data[i];
            if (device.joystick)
            {
                continue;
            }

            device.id = id;
            device.type = type;
            device.joystick = js;
        }

    #endif
    }    


    static void add_input_device(InputDeviceList& devices, SDL_JoystickID id)
    {
        SDL_JoystickType type = SDL_GetJoystickTypeForID(id);

        switch (type)
        {
        case SDL_JOYSTICK_TYPE_GAMEPAD:
            add_gamepad_device(devices.gamepads, id, type);
            break;

        default:
            add_joystick_device(devices.joysticks, id, type);
            break;
        }
    }


    
}


/* static data */

namespace sdl
{
    InputDeviceList input_devices;


    static void open_input_devices()
    {        
        int n_connected = 0;
        auto ids = SDL_GetJoysticks(&n_connected);

        SDL_JoystickID id;
        SDL_JoystickType type;

        for (int i = 0; i < n_connected; i++)
        {
            add_input_device(input_devices, ids[i]);
        }

        SDL_free(ids);
    }


    static void close_input_devices()
    {
        close_devices(input_devices);
    }



}


/* joystick */

namespace sdl
{
    using JoystickInput = input::JoystickInput;
    using Input = input::Input;


#ifndef NO_JOYSTICK

    void record_joystic_button_input(Uint8 btn_id, JoystickInput const& old_jsk, JoystickInput& new_jsk, bool is_down)
    {
        switch (btn_id)
        {
        #if JOYSTICK_BTN_0
        case 0:
            input::record_button_input(old_jsk.btn_0, new_jsk.btn_0, is_down);
            break;
        #endif
        #if JOYSTICK_BTN_1
        case 1:
            input::record_button_input(old_jsk.btn_1, new_jsk.btn_1, is_down);
            break;
        #endif
        #if JOYSTICK_BTN_2
        case 2:
            input::record_button_input(old_jsk.btn_2, new_jsk.btn_2, is_down);
            break;
        #endif
        #if JOYSTICK_BTN_3
        case 3:
            input::record_button_input(old_jsk.btn_3, new_jsk.btn_3, is_down);
            break;
        #endif
        #if JOYSTICK_BTN_4
        case 4:
            input::record_button_input(old_jsk.btn_4, new_jsk.btn_4, is_down);
            break;
        #endif
        #if JOYSTICK_BTN_5
        case 5:
            input::record_button_input(old_jsk.btn_5, new_jsk.btn_5, is_down);
            break;
        #endif
        #if JOYSTICK_BTN_6
        case 6:
            input::record_button_input(old_jsk.btn_6, new_jsk.btn_6, is_down);
            break;
        #endif
        #if JOYSTICK_BTN_7
        case 7:
            input::record_button_input(old_jsk.btn_7, new_jsk.btn_7, is_down);
            break;
        #endif
        #if JOYSTICK_BTN_8
        case 8:
            input::record_button_input(old_jsk.btn_8, new_jsk.btn_8, is_down);
            break;
        #endif
        #if JOYSTICK_BTN_9
        case 9:
            input::record_button_input(old_jsk.btn_9, new_jsk.btn_9, is_down);
            break;
        #endif
        }
    }


    static void record_joystick_axis_input(Uint8 axis_id, JoystickInput const& old_jsk, JoystickInput& new_jsk, Sint16 value)
    {
        Sint16 x = 0;
        Sint16 y = 0;

        value /= 3200;

        switch (axis_id)
        {
        case 0:
            x = value;
            break;

        case 1:
            y = value;
            break;

        default:
            break;
        }

        set_unit_vector_state(new_jsk.vec_axis, x, y);

    #if JOYSTICK_BTN_AXIS

        input::record_button_input(old_jsk.btn_axis_left,  new_jsk.btn_axis_left,  x < 0);
        input::record_button_input(old_jsk.btn_axis_right, new_jsk.btn_axis_right, x > 0);
        input::record_button_input(old_jsk.btn_axis_up,    new_jsk.btn_axis_up,    y < 0);
        input::record_button_input(old_jsk.btn_axis_down,  new_jsk.btn_axis_down,  y > 0);

    #endif
    }

#endif


    static void record_joystick_input(SDL_Event const& event, Input const& prev, Input& curr)
    {
    #ifndef NO_JOYSTICK

        int id = -1;

        Uint8 btn = 255;
        bool is_down = false;
        
        Uint8 axis = 255;
        Sint16 value = 0;

        switch (event.type)
        {
        case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
        case SDL_EVENT_JOYSTICK_BUTTON_UP:
        {
            btn = event.jbutton.button;
            is_down = event.jbutton.down;
        } break;

        case SDL_EVENT_JOYSTICK_AXIS_MOTION:
        {
            axis = event.jaxis.axis;
            value = event.jaxis.value;
        } break;

        }

    #endif
    }

}


/* gamepad */

namespace sdl
{
    using ControllerInput = input::ControllerInput;


#ifndef NO_CONTROLLER

    static void record_controller_button_input(SDL_GamepadButton btn, ControllerInput const& old_controller, ControllerInput& new_controller, bool is_down)
    {
        switch (btn)
        {
        #if CONTROLLER_BTN_DPAD_UP
        case SDL_GAMEPAD_BUTTON_DPAD_UP:
            input::record_button_input(old_controller.btn_dpad_up, new_controller.btn_dpad_up, is_down);
            break;
        #endif
        }
    }

#endif


    static void record_gamepad_input(SDL_Event const& event, Input const& prev, Input& curr)
    {
    #ifndef NO_CONTROLLER

    int id = -1;

    Uint8 btn = 255; // SDL_GamepadButton
    bool is_down = false;
    
    Uint8 axis = 255;
    Sint16 value = 0;

    switch (event.type)
    {
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
    {
        btn = event.gbutton.button;
        is_down = event.gbutton.down;

    } break;

    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
    {
        axis = event.gaxis.axis;
        value = event.gaxis.value;

    } break;
    }

    #endif
    }
}