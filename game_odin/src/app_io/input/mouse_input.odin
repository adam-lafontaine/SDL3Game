package input

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

    window_pos: int,
    wheel: int
}


@(private)
reset_mouse_wheel :: proc(mouse: ^MouseInput)
{

}


@(private)
reset_mouse_position :: proc(mouse: ^MouseInput)
{

}


@(private)
copy_mouse_position :: proc(src: MouseInput, dst: ^MouseInput)
{

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