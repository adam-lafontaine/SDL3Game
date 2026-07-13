package image_view

import "../util"

Pixel32 :: u32 // !!!
Pixel8 :: u8

Buffer32 :: util.MemoryBuffer(Pixel32)
Rect2Du32 :: util.Rect2Du32


BLACK: Pixel32 = 0
WHITE: Pixel32 = ~BLACK


ImageView :: struct
{
    width: u32,
    height: u32,

    data: []Pixel32  // !!! Pixel
}


SubView :: struct
{
    data: []Pixel32,  // !!! Pixel
    view_width: u32,

    x_begin: u32,
    y_begin: u32,

    width: u32,
    height: u32
}


create_buffer32 :: proc(buffer: ^Buffer32, n_pixels: u32) -> bool
{
    res := util.create_buffer(buffer, n_pixels)

    return res == .OK
}


destroy_buffer32 :: proc(buffer: ^Buffer32)
{
    util.destroy_buffer(buffer)
}


make_rect :: proc(x: u32, y: u32, w: u32, h: u32) -> Rect2Du32
{
    return Rect2Du32 {
        x_begin = x,
        x_end = x + w,
        y_begin = y,
        y_end = y + h
    }
}


make_view :: proc(buffer: ^Buffer32, width: u32, height: u32) -> ImageView
{
    view: ImageView

    data, res := util.push_elements(buffer, width * height)
    if res == .OK
    {
        view.width = width
        view.height = height
        view.data = data
    }

    return view
}


sub_view :: proc(view: ImageView, rect: Rect2Du32) -> SubView
{
    return SubView {
        data = view.data,
        view_width = view.width,
        x_begin = rect.x_begin,
        y_begin = rect.y_begin,
        width = rect.x_end - rect.x_begin,
        height = rect.y_end - rect.y_begin
    }
}


fill :: proc(view: ImageView, color: Pixel32)
{
    len := view.width * view.height

    for i in 0..<len
    {
        view.data[i] = color
    }
}