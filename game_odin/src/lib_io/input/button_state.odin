package input

import "../../util"

import "core:math"


ButtonState :: struct
{
    pressed: b8,
    is_down: b8,
    raised: b8,
    
}


@(private)
reset_button_state :: proc(btn: ^ButtonState)
{
    btn.is_down = false
    btn.pressed = false
    btn.raised = false
}


@(private)
record_button_input :: proc(btn_old: ButtonState, btn_new: ^ButtonState, is_down: b8)
{
    btn_new.pressed = !btn_old.is_down && is_down
    btn_new.is_down = is_down
    btn_new.raised = btn_old.is_down && !is_down
}


@(private)
copy_button_state :: proc(src: ButtonState, dst: ^ButtonState)
{
    dst.is_down = src.is_down
    dst.pressed = false
    dst.raised = false
}


VectorState :: struct
{
    vec: util.Vec2Df32,
    magnitude: f32,

    // unit
    cosine: f32,
    sine: f32
}


@(private)
reset_vector_state :: proc(vs: ^VectorState)
{
    vs.vec.x = 0
    vs.vec.y = 0
    vs.magnitude = 0
    vs.cosine = 0
    vs.sine = 0
}


@(private)
set_vector_state :: proc(vs: ^VectorState, x: f32, y: f32)
{
    vs.vec = { x, y }

    vs.magnitude = math.hypot(x, y)

    if vs.magnitude > 0
    {
        vs.cosine = x / vs.magnitude
        vs.sine = y / vs.magnitude
    }
}