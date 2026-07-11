#+private
package input

import rl "vendor:raylib"


/* keyboard */


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
    
    record_keyboard_input(prev.Keyboard.keys, &curr.Keyboard.keys)

    if (rl.WindowShouldClose()) // ESC key
    {
        curr.cmd_end_program = true
    }
}
