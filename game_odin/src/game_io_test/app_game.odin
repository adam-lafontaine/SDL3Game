#+private
package game_io_test

import "../util"


Buffer32 :: util.MemoryBuffer // !!!


StateData :: struct
{
    buffer32: Buffer32
}


app_init :: proc(state: ^AppState) -> AppResult
{
    assert(false, "*** NOT IMPLEMENTED ***")

    res: AppResult

    return res
}


app_set_screen_memory :: proc(state: ^AppState, screen: ImageView) -> bool
{
    assert(false, "*** NOT IMPLEMENTED ***")

    return false
}


app_update :: proc(state: ^AppState, input: Input)
{
    assert(false, "*** NOT IMPLEMENTED ***")
}


app_reset :: proc(state: ^AppState)
{
    assert(false, "*** NOT IMPLEMENTED ***")
}


app_close :: proc(state: ^AppState)
{
    assert(false, "*** NOT IMPLEMENTED ***")
}