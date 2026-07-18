#+private
package game_io_test

import "core:math"

import img "../image_view"
import mb "../util/memory_buffer"
import "../res"
import "../util"


Buffer32 :: img.Buffer32
SubView :: img.SubView
GrayView :: img.GrayView
GraySubView :: img.GraySubView
Vec2Df32 :: util.Vec2Df32
Vec2Di32 :: util.Vec2Di32

COLOR_BLACK       :: img.BLACK
COLOR_TRANSPARENT :: p32{ 0, 0, 0, 0 }
COLOR_BACKGROUND  :: p32{ 200, 200, 200, 255 }
COLOR_ON          :: p32{ 50, 255, 50, 255 }
COLOR_OFF         :: p32{ 127, 127, 127, 255 }
COLOR_UNEXPECTED  :: p32{ 255, 0, 255, 255 }
COLOR_ERROR       :: p32{ 255, 50, 50, 255 }



/* screen dimensions */

screen_dimensions_res :: proc() -> Vec2Du32
{
    /*
    | gpd1 gpd2 |
    | kbd   mse |
    */

    // Need screen dimensions before loading assets

    g := res.masks[.gamepad]
    k := res.masks[.keyboard]
    m := res.masks[.mouse]

    w := math.max(g.width * 2, k.width + m.width)
    h := math.max(g.height + k.height, g.height + m.height)

    dims := Vec2Du32 { w, h }

    return dims
}


screen_dimensions_masks :: proc(masks: MaskViewData) -> Vec2Du32
{
    g := masks.gamepad_view
    k := masks.keyboard_view    
    m := masks.mouse_view

    w := math.max(g.width * 2, k.width + m.width)
    h := math.max(g.height + k.height, g.height + m.height)

    dims := Vec2Du32 { w, h }

    return dims
}


screen_dimensions :: proc {
    screen_dimensions_res,
    screen_dimensions_masks
}


/* state */

StateData :: struct
{
    masks: MaskViewData,
    // music
    // sounds
    asset_memory: AssetMemory,

    mask_views: MaskViewMapList,
    inputs: InputList,
    
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


process_asset_memory :: proc(data: ^StateData) -> AssetStatus
{
    am := &data.asset_memory
    buffer := &data.buffer8

    if am.status != .Process
    {
        return am.status
    }

    n_pixels := mask_view_pixel_count(am^)
    res := mb.create_buffer(buffer, n_pixels)
    if res != .OK
    {
        assert(false, "*** BUFFER ***")
        am.status = .Fail
        return am.status
    }

    data.masks = create_mask_view_data(am^, buffer)

    dim := screen_dimensions(data.masks)
    out := data.out_view
    if dim.x != out.width || dim.y != out.height
    {
        assert(false, "*** MASK DIMENSIONS ***")
        am.status = .Fail
        return am.status
    }

    set_mask_list_views(data.masks, out, &data.mask_views)
    
    // sounds
    // music

    am.status = .Ready
    return am.status
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

    data := get_data(state)

    // check asset_memory.status later
    load_asset_memory_async(&data.asset_memory)    

    res.screen_dimensions = screen_dimensions()
    res.success = true

    return res
}


app_set_screen_memory :: proc(state: ^AppState, screen: ImageView) -> bool
{
    state.screen = screen

    data := get_data(state)

    dim := screen_dimensions()
    if dim.x != screen.width || dim.y != screen.height
    {
        return false
    }

    data.out_view = screen

    // process assets if already loaded
    status := process_asset_memory(data)

    ok := status == .Load || status == .Process || status == .Ready

    return ok
}


app_update :: proc(state: ^AppState, input: Input)
{
    data := get_data(state)

    switch (data.asset_memory.status)
    {
    case .None:
        img.fill(data.out_view, COLOR_UNEXPECTED)
        return

    case .Load:
    case .Process:
        process_asset_memory(data)
        img.fill(data.out_view, COLOR_BACKGROUND)
        return

    case .Fail:
        img.fill(data.out_view, COLOR_ERROR)
        return

    case .Ready:
    }

    map_input_list(input, &data.inputs)    
    
    img.fill(data.out_view, COLOR_BACKGROUND)

    draw_map_list(&data.mask_views, data.inputs)
}


app_reset :: proc(state: ^AppState)
{
    img.fill(state.screen, img.BLACK)
}


app_close :: proc(state: ^AppState)
{
    destroy_state_data(state)
}