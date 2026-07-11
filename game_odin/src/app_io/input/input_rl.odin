#+private
package input

import rl "vendor:raylib"


/* keyboard */

@(private="file")
record_keyboard_input :: proc(kbd_old: KeyboardKeys, kbd_new: ^KeyboardKeys)
{
    is_down: b8 = false

    is_down = cast(b8)rl.IsKeyDown(.W)
    record_button_input(kbd_old.kbd_W, &kbd_new.kbd_W, is_down)

    is_down = cast(b8)rl.IsKeyDown(.A)
    record_button_input(kbd_old.kbd_A, &kbd_new.kbd_A, is_down)

    is_down = cast(b8)rl.IsKeyDown(.S)
    record_button_input(kbd_old.kbd_S, &kbd_new.kbd_S, is_down)

    is_down = cast(b8)rl.IsKeyDown(.D)
    record_button_input(kbd_old.kbd_D, &kbd_new.kbd_D, is_down)

    is_down = cast(b8)rl.IsKeyDown(.KP_1)
    record_button_input(kbd_old.kbd_1, &kbd_new.kbd_1, is_down)

    is_down = cast(b8)rl.IsKeyDown(.KP_2)
    record_button_input(kbd_old.kbd_2, &kbd_new.kbd_2, is_down)

    is_down = cast(b8)rl.IsKeyDown(.KP_3)
    record_button_input(kbd_old.kbd_3, &kbd_new.kbd_3, is_down)

    is_down = cast(b8)rl.IsKeyDown(.KP_4)
    record_button_input(kbd_old.kbd_4, &kbd_new.kbd_4, is_down)

    is_down = cast(b8)rl.IsKeyDown(.SPACE)
    record_button_input(kbd_old.kbd_SPACE, &kbd_new.kbd_SPACE, is_down)
}


/* mouse */

record_mouse_button_input :: proc(src: MouseButtons, dst: ^MouseButtons)
{
    is_down: b8 = false

    is_down = cast(b8)rl.IsMouseButtonDown(.LEFT)
    record_button_input(src.btn_left, &dst.btn_left, is_down)

    is_down = cast(b8)rl.IsMouseButtonDown(.RIGHT)
    record_button_input(src.btn_right, &dst.btn_right, is_down)

    is_down = cast(b8)rl.IsMouseButtonDown(.MIDDLE)
    record_button_input(src.btn_middle, &dst.btn_middle, is_down)
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
