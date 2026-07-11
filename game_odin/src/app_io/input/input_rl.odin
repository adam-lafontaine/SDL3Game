#+private
package input

import rl "vendor:raylib"


/* keyboard */





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
    //assert(false, "*** NOT IMPLEMENTED ***")

    prev := prev(inputs)
    curr := curr(inputs)

    is_down: b8 = false

    is_down = cast(b8)rl.IsKeyDown(.SPACE)
    record_button_input(prev.button, &curr.button, is_down)

    if (rl.WindowShouldClose()) // !!!
    {
        curr.cmd_end_program = true
    }
}
