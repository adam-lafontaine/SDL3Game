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