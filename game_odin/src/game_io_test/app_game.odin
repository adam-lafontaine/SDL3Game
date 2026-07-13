#+private
package game_io_test

import "core:math"

import img "../image_view"


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

    c := Vec2Du32 { 192, 92 } // controller
    k := Vec2Du32 { 272, 92 } // keyboard
    m := Vec2Du32 { 80, 92 }  // mouse

    w := math.max(c.x * 2, k.x + m.x)
    h := math.max(c.y + k.y, c.y + m.y)

    res := Vec2Du32 { w, h }

    return res
}


/* state */

StateData :: struct
{
    out_src: ImageView,
    out_dst: SubView,

    buffer32: Buffer32
}


get_data :: proc(state: ^AppState) -> ^StateData
{
    return cast(^StateData)state.data
}


destroy_state_data ::proc(state: ^AppState)
{
    data := get_data(state)

    img.destroy_buffer32(&data.buffer32)

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
    scale_w := screen.width / dim.x
    scale_h := screen.height / dim.y

    scale := math.min(scale_w, scale_h)

    if scale == 0
    {
        return false // no down scaling
    }

    w := dim.x * scale
    h := dim.y * scale

    x := (screen.width - w) / 2
    y := (screen.height - h) / 2

    r := img.make_rect(x, y, w, h)

    data.out_dst = img.sub_view(screen, r)

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