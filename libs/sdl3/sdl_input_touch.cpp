/* touch gesture */

namespace sdl
{
#ifndef NO_TOUCH

    using TouchInput = input::TouchInput;
    using TouchGesture = input::TouchGesture;


    static i32 spawn_gesture(TouchInput& touch, SDL_TouchID d, SDL_FingerID g)
    {
        constexpr auto N = (i32)TouchInput::count;

        for (i32 i = 0; i < N; i++)
        {
            auto& item = touch.gestures[i];
            if (!item.is_active)
            {
                item.device_id = d;
                item.gesture_id = g;
                item.pos = { 0 };
                item.btn_touch.any = 0;
                return i;
            }
        }

        return -1;
    }


    static i32 find_gesture(TouchInput& touch, SDL_TouchID d, SDL_FingerID g)
    {
        constexpr auto N = (i32)TouchInput::count;

        for (i32 i = 0; i < N; i++)
        {
            auto& item = touch.gestures[i];
            if (item.device_id == d && item.gesture_id == g)
            {
                return i;
            }
        }

        return -1;
    }

#endif


    static void record_touch_gesture(SDL_Event const& event, TouchInput const& prev, TouchInput& curr)
    {
    #ifndef NO_TOUCH

        bool is_down = false;
        SDL_TouchID device = 0;
        SDL_FingerID gesture = 0;

        switch (event.type)
        {
        case SDL_EVENT_FINGER_DOWN:
        {
            device = event.tfinger.touchID;
            gesture = event.tfinger.fingerID;
            auto id = spawn_gesture(curr, device, gesture);
            if (id < 0)
            {
                return;
            }

            is_down = true;

            auto& p = prev.gestures[id];
            auto& c = curr.gestures[id];

            input::record_button_input(p.btn_touch, c.btn_touch, is_down);            

            c.pos.x = event.tfinger.x;
            c.pos.y = event.tfinger.y;

        } break;

        case SDL_EVENT_FINGER_UP:
        { 
            device = event.tfinger.touchID;
            gesture = event.tfinger.fingerID;
            auto id = find_gesture(curr, device, gesture);
            if (id < 0)
            {
                return;
            }

            is_down = false;

            auto& p = prev.gestures[id];
            auto& c = curr.gestures[id];

            input::record_button_input(p.btn_touch, c.btn_touch, is_down);

            c.pos.x = event.tfinger.x;
            c.pos.y = event.tfinger.y;

            // despawn next frame
            c.device_id = 0;

        } break;

        case SDL_EVENT_FINGER_MOTION:
        { 
            device = event.tfinger.touchID;
            gesture = event.tfinger.fingerID;
            auto id = find_gesture(curr, device, gesture);
            if (id < 0)
            {
                return;
            }

            auto& c = curr.gestures[id];

            c.pos.x = event.tfinger.x;
            c.pos.y = event.tfinger.y;

        } break;

        default:
            break;
        }

    #endif
    }
    
}


/* touch joystick */

namespace sdl
{
#ifdef TOUCH_JOYSTICK

    using JSI = JoystickInput;

    class TouchJoystickButtonList
    {
    public:
        static constexpr u32 count = (u32)JSI::Btn::Count;

        b8 is_set[count] = { 0 };
        Rect2Df32 regions[count];
        b8 is_down[count] = { 0 };
    };


    static TouchJoystickButtonList touch_joystick_buttons;


    static JSI::Btn find_touch_joystick_btn_region(Point2Df32 pos)
    {
        auto const contains = [](Point2Df32 p, Rect2Df32 r)
        {
            return
                p.x >= r.x_begin &&
                p.x < r.x_end &&
                p.y >= r.y_begin &&
                p.y < r.y_end;
        };

        auto& list = touch_joystick_buttons;
        auto S = list.count;

        for (u32 i = 0; i < S; i++)
        {
            if (list.is_set[i] && contains(pos, list.regions[i]))
            {
                return (JSI::Btn)i;
            }
        }

        return JSI::Btn::None;
    }


#endif


}


/* internal api */

namespace sdl
{
    static void record_touch_input_event(SDL_Event const& event, Input const& prev, Input& curr)
    {
        record_touch_gesture(event, prev.touch, curr.touch);
    }


    static void map_touch_joystick(Input const& prev, Input& curr)
    {
    #ifdef TOUCH_JOYSTICK

        auto& src = curr.touch;
        auto& list = touch_joystick_buttons;

        static int count = 0;

        for (u32 i = 0; i < list.count; i++)
        {
            list.is_down[i] = 0;
        }
        
        for (u32 i = 0; i < src.count; i++)
        {
            auto& item = src.gestures[i];
            if (!item.is_active)
            {
                continue;
            }

            auto btn = find_touch_joystick_btn_region(item.pos);
            if (btn == JSI::Btn::None)
            {
                continue;
            }

            list.is_down[(u32)btn] = item.btn_touch.is_down;            
        }

        for (u32 i = 0; i < list.count; i++)
        {
            auto& p = prev.joystick.buttons[i];
            auto& c = curr.joystick.buttons[i];
            auto is_down = list.is_down[i];

            input::record_button_input(p, c, is_down);
        }

    #endif
    }
}


/* touch joystick api */

namespace input
{
#ifdef TOUCH_JOYSTICK


    bool set_touch_joystick_btn(JoystickInput::Btn btn, Rect2Df32 region)
    {
        constexpr auto count = (u32)JoystickInput::Btn::Count;
        constexpr auto none = (u32)JoystickInput::Btn::None;

        auto& list = sdl::touch_joystick_buttons;

        auto i = (u32)btn;

        if (i >= none || i == count)
        {
            return false;
        }

        if (i > count)
        {
            i -= (count + 1u);
        }

        list.is_set[i] = 1;
        list.regions[i] = region;

        return true;
    }


#endif
}