#+private
package input

import rl "vendor:raylib"


/* keyboard */

@(private="file")
record_keyboard_input :: proc(kbd_old: KeyboardKeys, kbd_new: ^KeyboardKeys)
{
    bd :: proc(id: rl.KeyboardKey) -> b8 { return cast(b8)rl.IsKeyDown(id) }
    
    record_button_input(kbd_old.kbd_W, &kbd_new.kbd_W, bd(.W))
    record_button_input(kbd_old.kbd_A, &kbd_new.kbd_A, bd(.A))
    record_button_input(kbd_old.kbd_S, &kbd_new.kbd_S, bd(.S))
    record_button_input(kbd_old.kbd_D, &kbd_new.kbd_D, bd(.D))
    record_button_input(kbd_old.kbd_1, &kbd_new.kbd_1, bd(.KP_1) || bd(.ONE))
    record_button_input(kbd_old.kbd_2, &kbd_new.kbd_2, bd(.KP_2) || bd(.TWO))
    record_button_input(kbd_old.kbd_3, &kbd_new.kbd_3, bd(.KP_3) || bd(.THREE))
    record_button_input(kbd_old.kbd_4, &kbd_new.kbd_4, bd(.KP_4) || bd(.FOUR))
    record_button_input(kbd_old.kbd_SPACE, &kbd_new.kbd_SPACE, bd(.SPACE))
}


/* mouse */

record_mouse_button_input :: proc(src: MouseButtons, dst: ^MouseButtons)
{
    bd :: proc(id: rl.MouseButton) -> u8 { return cast(b8)rl.IsMouseButtonDown(id) }
    
    record_button_input(src.btn_left, &dst.btn_left, bd(.LEFT))
    record_button_input(src.btn_right, &dst.btn_right, bd(.RIGHT))
    record_button_input(src.btn_middle, &dst.btn_middle, bd(.MIDDLE))
}


record_mouse_position_input :: proc(mouse: ^MouseInput)
{
    pos := rl.GetMousePosition()

    mouse.window_pos.x = i32(pos.x)
    mouse.window_pos.y = i32(pos.y)
}


record_mouse_wheel_input :: proc(mouse: ^MouseInput)
{
    vec := rl.GetMouseWheelMoveV()

    mouse.wheel.x = i32(vec.x)
    mouse.wheel.y = i32(vec.y) // up < 0
}


/* api for the api */

api_init :: proc(inputs: ^InputArray) -> bool
{
    //assert(false, "*** NOT IMPLEMENTED ***")
    return true
}


api_close :: proc()
{
    //assert(false, "*** NOT IMPLEMENTED ***")
}


api_record_input :: proc(inputs: ^InputArray)
{
    prev := prev(inputs)
    curr := curr(inputs)
    
    record_keyboard_input(prev.keyboard.keys, &curr.keyboard.keys)
    record_mouse_button_input(prev.mouse.buttons.buttons, &curr.mouse.buttons.buttons)
    record_mouse_position_input(&curr.mouse)
    record_mouse_wheel_input(&curr.mouse)

    if (rl.WindowShouldClose()) // ESC key
    {
        curr.cmd_end_program = true
    }
}
