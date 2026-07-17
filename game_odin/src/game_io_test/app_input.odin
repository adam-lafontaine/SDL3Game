#+private
package game_io_test

import inp "../lib_io/input"

BtnState :: inp.ButtonState


//ControllerBtnOnOff :: ControllerDef(b8)
KeyboardOnOff :: [KeyboardId]b8
MouseBtnOnOff :: [MouseId]b8
//ControllerStickRotation :: ControllerStickDef(Vec2Df32)


InputList :: struct
{
    //controller1: ControllerBtnOnOff,
    //controller2: ControllerBtnOnOff,
    keyboard: KeyboardOnOff,
    mouse: MouseBtnOnOff,

    mouse_pos: Vec2Di32,

    //sticks1: ControllerStickRotation,
    //sticks2: ControllerStickRotation
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

    //clear(&inputs.controller1)
    //clear(&inputs.controller2)
    clear(&inputs.keyboard)
    clear(&inputs.mouse)

    inputs.mouse_pos = { 0, 0 }
}


@(private="file")
map_button :: proc(btn: BtnState, dst: ^b8)
{    
    dst^ |= btn.is_down
}


@(private="file")
map_keyboard_inputs :: proc(src: inp.KeyboardInput, dst: ^KeyboardOnOff)
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
map_mouse_inputs :: proc(src: inp.MouseInput, dst: ^MouseBtnOnOff)
{
    map_button(src.buttons[.btn_left], &dst[.left])
    map_button(src.buttons[.btn_right], &dst[.right])
    map_button(src.buttons[.btn_middle], &dst[.middle])

    // pos
}


/*map_controller_inputs :: proc(dst: ^ControllerBtnOnOff)
{

}


map_thumbstick_input :: proc(dst: ^ControllerStickRotation)
{

}


map_joystick_input :: proc(dst: ^ControllerBtnOnOff)
{

}*/


map_input_list :: proc(src: Input, dst: ^InputList)
{
    clear_input_list(dst)

    map_keyboard_inputs(src.keyboard, &dst.keyboard)
    map_mouse_inputs(src.mouse, &dst.mouse)

    dst.mouse_pos = src.mouse.window_pos

    //map_gamepad_input(src.gamepads[0], &dst.controller1)
    //map_gamepad_input(src.gamepads[1], &dst.controller2)

    //map_joystick_input(src.joysticks[0], &dst.controller1)
    //map_joystick_input(src.joysticks[1], &dst.controller2)

    //map_thumbstick_input(src.gamepads[0], &dst.sticks1)
    //map_thumbstick_input(src.gamepads[1], &dst.sticks2)
}
