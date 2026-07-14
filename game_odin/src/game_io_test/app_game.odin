#+private
package game_io_test

import "core:math"

import img "../image_view"
import "../res"


Buffer32 :: img.Buffer32
SubView :: img.SubView


/* mask view */

app_screen_dimensions :: proc() -> Vec2Du32
{
    /*
    | ctlr ctlr |
    | kbd   mse |
    */

    // Need screen dimensions before loading assets

    c := res.masks[.controller]
    k := res.masks[.keyboard]
    m := res.masks[.mouse]

    w := math.max(c.width * 2, k.width + m.width)
    h := math.max(c.height + k.height, c.height + m.height)

    res := Vec2Du32 { w, h }

    return res
}


/* state */

StateData :: struct
{
    asset_memory: AssetMemory,
    
    out_view: ImageView,

    buffer8: img.Buffer8
}


get_data :: proc(state: ^AppState) -> ^StateData
{
    return cast(^StateData)state.data
}


destroy_state_data ::proc(state: ^AppState)
{
    data := get_data(state)

    img.destroy_buffer8(&data.buffer8)

    free(state.data)
}


create_state_data :: proc(state: ^AppState) -> bool
{
    state_data, err := new(StateData)
    if err != nil
    {
        return false
    }

    state.data = cast(StateDataRef)state_data

    data := get_data(state)

    // no thread
    load_asset_memory(&data.asset_memory)

    return true
}





/* api */

app_init :: proc(state: ^AppState) -> AppResult
{
    res: AppResult
    res.success = false

    if !create_state_data(state)
    {
        return res
    }

    res.screen_dimensions = app_screen_dimensions()
    res.success = true

    return res
}


app_set_screen_memory :: proc(state: ^AppState, screen: ImageView) -> bool
{
    state.screen = screen

    data := get_data(state)

    dim := app_screen_dimensions()
    if dim.x != screen.width || dim.y != screen.height
    {
        return false
    }

    data.out_view = screen

    return true
}


app_update :: proc(state: ^AppState, input: Input)
{
    kbd := input.keyboard.keys
    mouse := input.mouse.buttons.buttons
    pos := input.mouse.window_pos
    vec := input.mouse.wheel

    if kbd.kbd_SPACE.pressed
    {
        img.fill(state.screen, img.WHITE)
    }

    if mouse.btn_right.pressed
    {
        img.fill(state.screen, img.BLACK)
    }
}


app_reset :: proc(state: ^AppState)
{
    img.fill(state.screen, img.BLACK)
}


app_close :: proc(state: ^AppState)
{
    destroy_state_data(state)
}