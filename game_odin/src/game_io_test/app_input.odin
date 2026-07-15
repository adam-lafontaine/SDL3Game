#+private
package game_io_test

import inp "../lib_io/input"

BtnState :: inp.ButtonState


ControllerBtnOnOff :: ControllerDef(b8)
KeyboardOnOff :: KeyboardDef(b8)
MouseBtnOnOff :: MouseDef(b8)
ControllerStickRotation :: ControllerStickDef(Vec2Df32)


InputList :: struct
{
    controller1: ControllerBtnOnOff,
    controller2: ControllerBtnOnOff,
    keyboard: KeyboardOnOff,
    mouse: MouseBtnOnOff,

    mouse_pos: Vec2Di32,

    sticks1: ControllerStickRotation,
    sticks2: ControllerStickRotation
}


clear_input_list :: proc(inputs: ^InputList)
{
    clear :: proc(input: $T)
    {
        N := len(input.list)
        for i in 0..<N
        {
            input.list[i] = false
        }
    }

    clear(&inputs.controller1)
    clear(&inputs.controller2)
    clear(&inputs.keyboard)
    clear(&inputs.mouse)

    inputs.mouse_pos = { 0, 0 }
}


map_button :: proc(btn: BtnState, dst: ^b8)
{    
    dst^ |= btn.is_down
}


map_keyboard_inputs :: proc(src: inp.KeyboardInput, dst: ^KeyboardOnOff)
{
    map_button(src.keys.kbd_1, &dst.items.n_1)
    map_button(src.keys.kbd_2, &dst.items.n_2)
    map_button(src.keys.kbd_3, &dst.items.n_3)
    map_button(src.keys.kbd_4, &dst.items.n_4)

    map_button(src.keys.kbd_W, &dst.items.w)
    map_button(src.keys.kbd_A, &dst.items.a)
    map_button(src.keys.kbd_S, &dst.items.s)
    map_button(src.keys.kbd_D, &dst.items.d)

    map_button(src.keys.kbd_SPACE, &dst.items.space)
}


map_mouse_inputs :: proc(src: inp.MouseInput, dst: ^MouseBtnOnOff)
{

}


map_controller_inputs :: proc(dst: ^ControllerBtnOnOff)
{

}


map_thumbstick_input :: proc(dst: ^ControllerStickRotation)
{

}


map_joystick_input :: proc(dst: ^ControllerBtnOnOff)
{

}


update_visual :: proc(src: Input, dst: ^InputList)
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
