#+private
package game_io_test

import inp "../lib_io/input"

BtnState :: inp.ButtonState


KeyboardOnOff :: [KeyboardId]b8
MouseBtnOnOff :: [MouseId]b8
GamepadBtnOnOff :: [GamepadId]b8
//GamepadStickRotation :: GamepadStickDef(Vec2Df32)


InputList :: struct
{    
    keyboard: KeyboardOnOff,
    mouse: MouseBtnOnOff,

    mouse_pos: Vec2Di32,

    gamepad1: GamepadBtnOnOff,
    gamepad2: GamepadBtnOnOff,

    //sticks1: GamepadStickRotation,
    //sticks2: GamepadStickRotation
}


@(private="file")
clear_input_list :: proc(inputs: ^InputList)
{
    clear :: proc(input: $T)
    {
        for &btn in input
        {
            btn = false
        }
    }
    clear(&inputs.keyboard)
    clear(&inputs.mouse)

    inputs.mouse_pos = { 0, 0 }

    clear(&inputs.gamepad1)
    clear(&inputs.gamepad2)

    // thumbsticks?
}


@(private="file")
map_button :: proc(btn: BtnState, dst: ^b8)
{    
    dst^ |= btn.is_down
}


@(private="file")
map_keyboard_key_inputs :: proc(src: inp.KeyboardKeyInput, dst: ^KeyboardOnOff)
{
    map_button(src[.kbd_1], &dst[.n_1])
    map_button(src[.kbd_2], &dst[.n_2])
    map_button(src[.kbd_3], &dst[.n_3])
    map_button(src[.kbd_4], &dst[.n_4])

    map_button(src[.kbd_W], &dst[.w])
    map_button(src[.kbd_A], &dst[.a])
    map_button(src[.kbd_S], &dst[.s])
    map_button(src[.kbd_D], &dst[.d])

    map_button(src[.kbd_SPACE], &dst[.space])
}


@(private="file")
map_mouse_button_inputs :: proc(src: inp.MouseButtonInput, dst: ^MouseBtnOnOff)
{
    map_button(src[.btn_left], &dst[.left])
    map_button(src[.btn_right], &dst[.right])
    map_button(src[.btn_middle], &dst[.middle])
}


map_gamepad_button_inputs :: proc(src: inp.GamepadButtonInput, dst: ^GamepadBtnOnOff)
{     
    map_button(src[.btn_dpad_up],    &dst[.dpad_up])
    map_button(src[.btn_dpad_right], &dst[.dpad_right])
    map_button(src[.btn_dpad_down],  &dst[.dpad_down])
    map_button(src[.btn_dpad_left],  &dst[.dpad_left])
    
    map_button(src[.btn_start], &dst[.start])
    map_button(src[.btn_back],  &dst[.back])

    map_button(src[.btn_south], &dst[.a])
    map_button(src[.btn_east],  &dst[.b])
    map_button(src[.btn_west],  &dst[.x])
    map_button(src[.btn_north], &dst[.y])

    map_button(src[.btn_shoulder_left],  &dst[.shoulder_left])
    map_button(src[.btn_shoulder_right], &dst[.shoulder_right])
    
    map_button(src[.btn_stick_left],  &dst[.stick_left])
    map_button(src[.btn_stick_right], &dst[.shoulder_right])
}


map_gamepad_axis_inputs :: proc(src: inp.GamepadInput, dst: ^GamepadBtnOnOff)
{
    dst[.trigger_left] = src.trigger_left > 0
    dst[.trigger_right] = src.trigger_right > 0
}


/*map_thumbstick_input :: proc(dst: ^GamepadStickRotation)
{

}


map_joystick_input :: proc(dst: ^GamepadBtnOnOff)
{

}*/


map_input_list :: proc(src: Input, dst: ^InputList)
{
    clear_input_list(dst)

    map_keyboard_key_inputs(src.keyboard.keys, &dst.keyboard)
    map_mouse_button_inputs(src.mouse.buttons, &dst.mouse)

    dst.mouse_pos = src.mouse.window_pos

    map_gamepad_button_inputs(src.gamepads[0].buttons, &dst.gamepad1)
    map_gamepad_button_inputs(src.gamepads[1].buttons, &dst.gamepad2)

    map_gamepad_axis_inputs(src.gamepads[0], &dst.gamepad1)
    map_gamepad_axis_inputs(src.gamepads[1], &dst.gamepad2)

    //map_joystick_input(src.joysticks[0], &dst.gamepad1)
    //map_joystick_input(src.joysticks[1], &dst.gamepad2)

    //map_thumbstick_input(src.gamepads[0], &dst.sticks1)
    //map_thumbstick_input(src.gamepads[1], &dst.sticks2)
}
