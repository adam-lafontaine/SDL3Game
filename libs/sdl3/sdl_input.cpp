#pragma once

#include "../input/input_state.hpp"
#include "../util/numeric.hpp"
#include "sdl_include.hpp"

namespace num = numeric;


void end_program();

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


    static void set_unit_vector_state(input::VectorState<i8>& vs, Sint16 x, Sint16 y)
    {
        auto& vec = vs.vec;
        auto& unit = vs.unit_direction;

        vec.x = num::sign_i8(x);
        vec.y = num::sign_i8(y);

        unit.x = (f32)vec.x;
        unit.y = (f32)vec.y;

        constexpr f32 hypot = 1.4142135f;
        constexpr f32 i_hypot = 1.0f / hypot;
        
        auto mag = (x || y) ? 1.0f : 0.0f;
        auto i_mag = (x && y) ? i_hypot : (x || y) ? 1.0f : 0.0f;

        vs.magnitude = mag;
        unit.x *= i_mag;
        unit.y *= i_mag;
    }
}


#include "sdl_joystick.cpp"
#include "sdl_keyboard.cpp"
#include "sdl_mouse.cpp"
