package window

import "../../util"


Vec2Du32 :: util.Vec2Du32


WindowStatus :: enum
{
    Void = 0,
    Ready,
    Running,
    Error
}


Window :: struct 
{
    pixel_buffer: ^u32,
    width_px: u32,
    height_px: u32,

    handle: i32,

    status: WindowStatus
}

@(private)
reset_window :: proc(window: ^Window)
{
    window.pixel_buffer = nil
    window.width_px = 0
    window.height_px = 0
    window.handle = -1
    window.status = .Void
}


Rotate :: enum
{
    None = 0,
    Clockwise_90,
    CounterClockwise_90
}


init :: proc(window: ^Window) -> bool
{
    if (window.status != .Void)
    {
        return false
    }

    reset_window(window)
    window.status = .Ready

    return true;
}


close :: proc(window: ^Window)
{
    if (window.status == .Running)
    {
        api_destroy(window)
        reset_window(window)
    }
}


create :: proc(window: ^Window, title: cstring, window_size: Vec2Du32, pixel_size: Vec2Du32) -> bool
{
    if (window.status != .Ready)
    {
        return false
    }

    ok := api_create(window, title, window_size, pixel_size)

    window.status = ok ? .Running : .Error

    return ok
}


create_rotated :: proc(window: ^Window, title: cstring, window_size: Vec2Du32, pixel_size: Vec2Du32, rotate: Rotate = Rotate.None) -> bool
{
    if (window.status != .Ready)
    {
        return false
    }

    ok := api_create_rotated(window, title, window_size, pixel_size, rotate)

    window.status = ok ? .Running : .Error

    return ok
}


create_fullscreen :: proc(window: ^Window, title: cstring, pixel_size: Vec2Du32) -> bool
{
    if (window.status != .Ready)
    {
        return false
    }

    ok := api_create_fullscreen(window, title, pixel_size)

    window.status = ok ? .Running : .Error

    return ok
}


create_fullscreen_rotated :: proc(window: ^Window, title: cstring, pixel_size: Vec2Du32, rotate: Rotate = Rotate.None) -> bool
{
    if (window.status != .Ready)
    {
        return false
    }

    ok := api_create_fullscreen_rotated(window, title, pixel_size, rotate)

    window.status = ok ? .Running : .Error

    return ok
}


set_window_icon :: proc(window: ^Window)
{
    if (window.status != .Running)
    {
        return
    }

    api_set_window_icon(window)
}


resize_pixel_buffer :: proc(window: ^Window, width: u32, height: u32) -> bool
{
    return api_resize_pixel_buffer(window, width, height)
}


render :: proc(window: ^Window, size_changed: b32 = false)
{
    api_render(window, size_changed)
}


render_rotated :: proc(window: ^Window, rotate: Rotate, size_changed: b32 = false)
{
    api_render_rotated(window, rotate, size_changed)
}


hide_mouse_cursor :: proc()
{
    api_hide_mouse_cursor()
}


show_mouse_cursor :: proc() 
{
    api_show_mouse_cursor()
}


@(private)
is_white := false

dbg_toggle_color :: proc(window: ^Window)
{
    is_white = !is_white

    if (is_white)
    {
        api_set_white(window)
    }
    else
    {
        api_set_black(window)
    }
}
