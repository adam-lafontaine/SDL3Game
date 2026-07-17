package input


GamepadButtons :: enum
{    
    btn_dpad_up,
    btn_dpad_down,
    btn_dpad_left,
    btn_dpad_right,
    btn_start,
    btn_back,
    btn_south,
    btn_east,
    btn_west,
    btn_north,
    btn_shoulder_left,
    btn_shoulder_right,
    btn_stick_left,
    btn_stick_right,
}


GamepadButtonInput :: [GamepadButtons]ButtonState


GamepadInput :: struct
{
    handle: i32,

    buttons: GamepadButtonInput,

    trigger_left: f32,
    trigger_right: f32,

    vec_dpad: VectorState,
    vec_stick_left: VectorState,
    vec_stick_right: VectorState
}


@(private)
reset_gamepad_state :: proc(gamepad: ^GamepadInput)
{
    for &btn in gamepad.buttons
    {
        reset_button_state(&btn)
    }

    gamepad.trigger_left = 0
    gamepad.trigger_right = 0

    reset_vector_state(&gamepad.vec_dpad)
    reset_vector_state(&gamepad.vec_stick_left)
    reset_vector_state(&gamepad.vec_stick_right)
}


@(private)
copy_gamepad_state :: proc(src: GamepadInput, dst: ^GamepadInput)
{
    for btn, id in src.buttons
    {
        copy_button_state(btn, &dst.buttons[id])
    }

    dst.trigger_left = 0
    dst.trigger_right = 0

    reset_vector_state(&dst.vec_dpad)
    reset_vector_state(&dst.vec_stick_left)
    reset_vector_state(&dst.vec_stick_right)
}