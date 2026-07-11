package input

import "../../util"


Input :: struct
{
    keyboard: KeyboardInput,
    mouse: MouseInput,    
    // gamepad
    // joystick
    // touch

    // flags
    window_size_changed: b8,
    cmd_end_program: b8
}


@(private)
reset_input_flags :: proc(input: ^Input)
{
    input.window_size_changed = false
    input.cmd_end_program = false
}


@(private)
reset_input_state :: proc(input: ^Input)
{
    reset_keyboard_state(&input.keyboard)    
    reset_mouse_state(&input.mouse)
    // gamepad
    // joystick
    // touch

    reset_input_flags(input)    
}


@(private)
copy_input_state :: proc(src: Input, dst: ^Input)
{
    copy_keyboard_state(src.keyboard, &dst.keyboard)
    copy_mouse_state(src.mouse, &dst.mouse)
    // gamepad
    // joystick
    // touch

    reset_input_flags(dst)
}
