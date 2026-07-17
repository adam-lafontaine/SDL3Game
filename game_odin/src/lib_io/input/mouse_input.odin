package input

import "../../util"

Pos2Di32 :: util.Vec2Di32
Vec2Di32 :: util.Vec2Di32


MouseButtons :: enum
{
    btn_left,
    btn_right,
    btn_middle
}


MouseButtonInput :: [MouseButtons]ButtonState


MouseInput :: struct
{
    buttons: MouseButtonInput,

    window_pos: Pos2Di32,
    wheel: Vec2Di32
}


@(private)
reset_mouse_wheel :: proc(mouse: ^MouseInput)
{
    mouse.wheel.x = 0
    mouse.wheel.y = 0
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
    for &btn in mouse.buttons
    {
        reset_button_state(&btn)
    }

    reset_mouse_position(mouse)
    reset_mouse_wheel(mouse)
}


@(private)
copy_mouse_state :: proc(src: MouseInput, dst: ^MouseInput)
{ 
    for btn, id in src.buttons
    {
        copy_button_state(btn, &dst.buttons[id])
    }

    copy_mouse_position(src, dst)
    reset_mouse_wheel(dst)
}