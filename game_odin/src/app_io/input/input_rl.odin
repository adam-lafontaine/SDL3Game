#+private
package input

import rl "vendor:raylib"



/* api for the api */

api_init :: proc(inputs: ^InputArray) -> bool
{
    //assert(false, "*** NOT IMPLEMENTED ***")
    rl.SetTargetFPS(60) // !!!
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

    if (rl.WindowShouldClose()) // !!!
    {
        curr.cmd_end_program = true
    }
}
