package game_io_test

import inp "../lib_io/input"
import img "../image_view"
import "../util"

Input :: inp.Input
ImageView :: img.ImageView
Vec2Du32 :: util.Vec2Du32


StateDataRef :: distinct rawptr


AppState :: struct
{
    screen: ImageView,

    data: StateDataRef,
}


AppResult :: struct
{
    success: bool,

    screen_dimensions: Vec2Du32,
}


init :: proc(state: ^AppState) -> AppResult
{
    // check data ptr
    return app_init(state)
}


set_screen_memory :: proc(state: ^AppState, screen: ImageView) -> bool
{
    // game memory init...success
    // state.screen = screen
    return app_set_screen_memory(state, screen)
}


update :: proc(state: ^AppState, input: Input)
{
    // map input here?
    app_update(state, input)
}


reset :: proc(state: ^AppState)
{
    app_reset(state)
}


close :: proc(state: ^AppState)
{
    app_close(state)
}

