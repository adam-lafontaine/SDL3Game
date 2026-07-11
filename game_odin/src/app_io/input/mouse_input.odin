package input

import "../../util"

Pos2Di32 :: util.Vec2Di32

N_MOUSE_BUTTONS :: 3


MouseButtonArray :: [N_MOUSE_BUTTONS]ButtonState


MouseButtons :: struct
{
    btn_left: ButtonState,
    btn_right: ButtonState,
    btn_middle: ButtonState
}


MouseButtonInput :: struct #raw_union
{
    list: MouseButtonArray,
    buttons: MouseButtons
}


MouseInput :: struct
{
    buttons: MouseButtonInput,

    window_pos: Pos2Di32,
    wheel: int
}


@(private)
reset_mouse_wheel :: proc(mouse: ^MouseInput)
{
    mouse.wheel = 0 // !!!
}


@(private)
reset_mouse_position :: proc(mouse: ^MouseInput)
{
    mouse.window_pos.x = 0
    mouse.window_pos.y = 0
}


@(private)
copy_mouse_position :: proc(src: MouseInput, dst: ^MouseInput)
{
    dst.window_pos = src.window_pos
}


@(private)
reset_mouse_state :: proc(mouse: ^MouseInput)
{
    for &btn in mouse.buttons.list
    {
        reset_button_state(&btn)
    }

    reset_mouse_position(mouse)
    reset_mouse_wheel(mouse)
}


@(private)
copy_mouse_state :: proc(src: MouseInput, dst: ^MouseInput)
{
    s := src.buttons.list
    d := dst.buttons.list

    for i in 0..<N_MOUSE_BUTTONS
    {
        copy_button_state(s[i], &d[i])
    }

    copy_mouse_position(src, dst)
    reset_mouse_wheel(dst)
}