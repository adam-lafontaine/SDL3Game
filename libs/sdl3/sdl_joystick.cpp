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


    using GamepadDeviceArray = DeviceArray<GamepadDevice, input::MAX_GAMEPADS>;
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
    #ifndef NO_GAMEPAD

        if (!SDL_IsGamepad(id))
        {
            sdl::print_message("not a gamepad");
            return;
        }

        auto gp = SDL_OpenGamepad(id);
        if (!gp)
        {
            sdl::print_error("SDL_OpenGamepad()");
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
            sdl::print_error("SDL_OpenJoystick()");
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


    static i32 find_gamepad(SDL_JoystickID id)
    {
        auto& devices = input_devices.gamepads;

        for (i32 i = 0; i < devices.capacity; i++)
        {
            if (id == devices.data[i].id)
            {
                return i;
            }
        }

        return -1;
    }


    static i32 find_joystick(SDL_JoystickID id)
    {
        auto& devices = input_devices.joysticks;

        for (i32 i = 0; i < devices.capacity; i++)
        {
            if (id == devices.data[i].id)
            {
                return i;
            }
        }

        return -1;
    }
}


/* joystick */

namespace sdl
{
    using JoystickInput = input::JoystickInput;
    using Input = input::Input;


#ifndef NO_JOYSTICK

    static void record_joystic_button_input(JoystickInput const& old_jsk, JoystickInput& new_jsk, Uint8 btn_id, bool is_down)
    {
        input::ButtonState btn_old = {};
        input::ButtonState* btn_new = 0;

        switch (btn_id)
        {
        #if JOYSTICK_BTN_0
        case 0:
            btn_old = old_jsk.btn_0;
            btn_new = &new_jsk.btn_0;
            break;
        #endif
        #if JOYSTICK_BTN_1
        case 1:
            btn_old = old_jsk.btn_1;
            btn_new = &new_jsk.btn_1;
            break;
        #endif
        #if JOYSTICK_BTN_2
        case 2:
            btn_old = old_jsk.btn_2;
            btn_new = &new_jsk.btn_2;
            break;
        #endif
        #if JOYSTICK_BTN_3
        case 3:
            btn_old = old_jsk.btn_3;
            btn_new = &new_jsk.btn_3;
            break;
        #endif
        #if JOYSTICK_BTN_4
        case 4:
            btn_old = old_jsk.btn_4;
            btn_new = &new_jsk.btn_4;
            break;
        #endif
        #if JOYSTICK_BTN_5
        case 5:
            btn_old = old_jsk.btn_5;
            btn_new = &new_jsk.btn_5;
            break;
        #endif
        #if JOYSTICK_BTN_6
        case 6:
            btn_old = old_jsk.btn_6;
            btn_new = &new_jsk.btn_6;
            break;
        #endif
        #if JOYSTICK_BTN_7
        case 7:
            btn_old = old_jsk.btn_7;
            btn_new = &new_jsk.btn_7;
            break;
        #endif
        #if JOYSTICK_BTN_8
        case 8:
            btn_old = old_jsk.btn_8;
            btn_new = &new_jsk.btn_8;
            break;
        #endif
        #if JOYSTICK_BTN_9
        case 9:
            btn_old = old_jsk.btn_9;
            btn_new = &new_jsk.btn_9;
            break;
        #endif

        default: return;
        }

        input::record_button_input(btn_old, *btn_new, is_down);
    }


    static void record_joystick_axis_input(JoystickInput& jsk, Uint8 axis_id, Sint16 axis_value)
    {
        auto val32 = normalize_axis_value(axis_value);

        switch (axis_id)
        {        
        #if JOYSTICK_AXIS_0
        case 0:
            jsk.axis_0 = val32;
            break;
        #endif
        #if JOYSTICK_AXIS_1
        case 1:
            jsk.axis_1 = val32;
            break;
        #endif
        #if JOYSTICK_AXIS_2
        case 2:
            jsk.axis_2 = val32;
            break;
        #endif
        #if JOYSTICK_AXIS_3
        case 3:
            jsk.axis_3 = val32;
            break;
        #endif
        #if JOYSTICK_AXIS_4
        case 4:
            jsk.axis_4 = val32;
            break;
        #endif
        #if JOYSTICK_AXIS_5
        case 5:
            jsk.axis_5 = val32;
            break;
        #endif

        default:
            break;
        }
    }

#endif


    static void record_joystick_input(SDL_Event const& event, Input const& prev, Input& curr)
    {
    #ifndef NO_JOYSTICK

        i32 id = -1;

        Uint8 btn = 255;
        bool is_down = false;
        
        Uint8 axis = 255;
        Sint16 value = 0;

        switch (event.type)
        {
        case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
        case SDL_EVENT_JOYSTICK_BUTTON_UP:
        {
            id = find_joystick(event.jdevice.which);
            if (id < 0)
            {
                return;
            }

            auto& p = prev.joysticks[id];
            auto& c = curr.joysticks[id];

            btn = event.jbutton.button;
            is_down = event.jbutton.down;

            record_joystic_button_input(p, c, btn, is_down);

        } break;

        case SDL_EVENT_JOYSTICK_AXIS_MOTION:
        {
            id = find_joystick(event.jdevice.which);
            if (id < 0)
            {
                return;
            }

            auto& c = curr.joysticks[id];

            axis = event.jaxis.axis;
            value = event.jaxis.value;
            
            record_joystick_axis_input(c, axis, value);
            
        } break;

        }

    #endif
    }

}


/* gamepad */

namespace sdl
{
    using GamepadInput = input::GamepadInput;


#ifndef NO_GAMEPAD

    static void record_gamepad_button_input(GamepadInput const& old_gamepad, GamepadInput& new_gamepad, Uint8 btn_id, bool is_down)
    {
        input::ButtonState btn_old = {};
        input::ButtonState* btn_new = 0;

        switch (btn_id)
        {
        #if GAMEPAD_BTN_DPAD_UP
        case SDL_GAMEPAD_BUTTON_DPAD_UP:
            btn_old = old_gamepad.btn_dpad_up;
            btn_new = &new_gamepad.btn_dpad_up;
            break;
        #endif
        #if GAMEPAD_BTN_DPAD_DOWN
        case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
            btn_old = old_gamepad.btn_dpad_down;
            btn_new = &new_gamepad.btn_dpad_down;
            break;
        #endif
        #if GAMEPAD_BTN_DPAD_LEFT
        case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
            btn_old = old_gamepad.btn_dpad_left;
            btn_new = &new_gamepad.btn_dpad_left;
            break;
        #endif
        #if GAMEPAD_BTN_DPAD_RIGHT
        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
            btn_old = old_gamepad.btn_dpad_right;
            btn_new = &new_gamepad.btn_dpad_right;
            break;
        #endif
        #if GAMEPAD_BTN_SOUTH
        case SDL_GAMEPAD_BUTTON_SOUTH:
            btn_old = old_gamepad.btn_south;
            btn_new = &new_gamepad.btn_south;
            break;
        #endif
        #if GAMEPAD_BTN_EAST
        case SDL_GAMEPAD_BUTTON_EAST:
            btn_old = old_gamepad.btn_east;
            btn_new = &new_gamepad.btn_east;
            break;
        #endif
        #if GAMEPAD_BTN_WEST
        case SDL_GAMEPAD_BUTTON_WEST:
            btn_old = old_gamepad.btn_west;
            btn_new = &new_gamepad.btn_west;
            break;
        #endif
        #if GAMEPAD_BTN_NORTH
        case SDL_GAMEPAD_BUTTON_NORTH:
            btn_old = old_gamepad.btn_north;
            btn_new = &new_gamepad.btn_north;
            break;
        #endif
        #if GAMEPAD_BTN_BACK
        case SDL_GAMEPAD_BUTTON_BACK:
            btn_old = old_gamepad.btn_back;
            btn_new = &new_gamepad.btn_back;
            break;
        #endif
        #if GAMEPAD_BTN_GUIDE
        case SDL_GAMEPAD_BUTTON_GUIDE:
            btn_old = old_gamepad.btn_;
            btn_new = &new_gamepad.btn_;
            break;
        #endif
        #if GAMEPAD_BTN_START
        case SDL_GAMEPAD_BUTTON_START:
            btn_old = old_gamepad.btn_start;
            btn_new = &new_gamepad.btn_start;
            break;
        #endif
        #if GAMEPAD_BTN_STICK_LEFT
        case SDL_GAMEPAD_BUTTON_LEFT_STICK:
            btn_old = old_gamepad.btn_stick_left;
            btn_new = &new_gamepad.btn_stick_left;
            break;
        #endif
        #if GAMEPAD_BTN_STICK_RIGHT
        case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
            btn_old = old_gamepad.btn_stick_right;
            btn_new = &new_gamepad.btn_stick_right;
            break;
        #endif
        #if GAMEPAD_BTN_SHOULDER_LEFT
        case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
            btn_old = old_gamepad.btn_shoulder_left;
            btn_new = &new_gamepad.btn_shoulder_left;
            break;
        #endif
        #if GAMEPAD_BTN_SHOULDER_RIGHT
        case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
            btn_old = old_gamepad.btn_shoulder_right;
            btn_new = &new_gamepad.btn_shoulder_right;
            break;
        #endif
        #if GAMEPAD_BTN_MISC1
        case SDL_GAMEPAD_BUTTON_MISC1:
            btn_old = old_gamepad.btn_;
            btn_new = &new_gamepad.btn_;
            break;
        #endif
        #if GAMEPAD_BTN_RIGHT_PADDLE1
        case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1:
            btn_old = old_gamepad.btn_;
            btn_new = &new_gamepad.btn_;
            break;
        #endif
        #if GAMEPAD_BTN_LEFT_PADDLE1
        case SDL_GAMEPAD_BUTTON_LEFT_PADDLE1:
            btn_old = old_gamepad.btn_;
            btn_new = &new_gamepad.btn_;
            break;
        #endif
        #if GAMEPAD_BTN_RIGHT_PADDLE2
        case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2:
            btn_old = old_gamepad.btn_;
            btn_new = &new_gamepad.btn_;
            break;
        #endif
        #if GAMEPAD_BTN_LEFT_PADDLE2
        case SDL_GAMEPAD_BUTTON_LEFT_PADDLE2:
            btn_old = old_gamepad.btn_;
            btn_new = &new_gamepad.btn_;
            break;
        #endif
        #if GAMEPAD_BTN_TOUCHPAD
        case SDL_GAMEPAD_BUTTON_TOUCHPAD:
            btn_old = old_gamepad.btn_;
            btn_new = &new_gamepad.btn_;
            break;
    #endif

        default: return;
        }

        input::record_button_input(btn_old, *btn_new, is_down);
    }


    static void record_gamepad_axis_input(GamepadInput& gamepad, Uint8 axis_id, Sint16 axis_value)
    {
        auto val32 = normalize_axis_value(axis_value);

        switch (axis_id)
        {
        #if GAMEPAD_AXIS_STICK_LEFT
        case SDL_GAMEPAD_AXIS_LEFTX:
            gamepad.stick_left.vec.x = val32;
            break;

        case SDL_GAMEPAD_AXIS_LEFTY:
            gamepad.stick_left.vec.y = val32;
            break;
        #endif

        #if GAMEPAD_AXIS_STICK_RIGHT
        case SDL_GAMEPAD_AXIS_RIGHTX:
            gamepad.stick_right.vec.x = val32;
            break;

        case SDL_GAMEPAD_AXIS_RIGHTY:
            gamepad.stick_right.vec.y = val32;
            break;
        #endif

        #if GAMEPAD_TRIGGER_LEFT
        case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
            gamepad.trigger_left = val32;
            break;
        #endif

        #if GAMEPAD_TRIGGER_RIGHT
        case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
            gamepad.trigger_right = val32;
            break;
        #endif

        default: 
            break;
        }
    }

#endif


    static void record_gamepad_input(SDL_Event const& event, Input const& prev, Input& curr)
    {
    #ifndef NO_GAMEPAD

    i32 id = -1;

    Uint8 btn = 255; // SDL_GamepadButton
    bool is_down = false;
    
    Uint8 axis = 255; // SDL_GamepadAxis
    Sint16 value = 0;

    switch (event.type)
    {
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
    {
        id = find_gamepad(event.gdevice.which);
        if (id < 0)
        {
            return;
        }

        auto& p = prev.gamepads[id];
        auto& c = curr.gamepads[id];

        btn = event.gbutton.button;
        is_down = event.gbutton.down;

        record_gamepad_button_input(p, c, btn, is_down);

    } break;

    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
    {
        id = find_gamepad(event.gdevice.which);
        if (id < 0)
        {
            return;
        }

        auto& c = curr.gamepads[id];

        axis = event.gaxis.axis;
        value = event.gaxis.value;

        record_gamepad_axis_input(c, axis, value);

    } break;
    }

    #endif
    }
}