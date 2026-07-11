package window

import "../../util"


Vec2Du32 :: util.Vec2Du32

Window :: struct 
{
    pixel_buffer: ^u32,
    width_px: u32,
    height_px: u32,

    handle: i32
}


Rotate :: enum
{
    None = 0,
    Clockwise_90,
    CounterClockwise_90
}


create :: proc(window: ^Window, title: cstring, window_size: Vec2Du32, pixel_size: Vec2Du32) -> bool
{
    return api_create(window, title, window_size, pixel_size)
}


create_rotated :: proc(window: ^Window, title: cstring, window_size: Vec2Du32, pixel_size: Vec2Du32, rotate: Rotate = Rotate.None) -> bool
{
    return api_create_rotated(window, title, window_size, pixel_size, rotate)
}


create_fullscreen :: proc(window: ^Window, title: cstring, pixel_size: Vec2Du32) -> bool
{
    return api_create_fullscreen(window, title, pixel_size)
}


create_fullscreen_rotated :: proc(window: ^Window, title: cstring, pixel_size: Vec2Du32, rotate: Rotate = Rotate.None) -> bool
{
    return api_create_fullscreen_rotated(window, title, pixel_size, rotate)
}


set_window_icon :: proc(window: ^Window)
{
    api_set_window_icon(window)
}


destroy :: proc(window: ^Window)
{
    api_destroy(window)
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
