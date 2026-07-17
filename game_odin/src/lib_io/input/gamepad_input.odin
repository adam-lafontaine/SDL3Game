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
    buttons: GamepadButtonInput,
}