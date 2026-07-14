package image_view

import "core:slice"
import "../util"
import mb "../util/memory_buffer"

Pixel32 :: struct 
{
    red: u8,
    green: u8,
    blue: u8,
    alpha: u8
}

Pixel8 :: u8

Buffer32 :: mb.MemoryBuffer(Pixel32)
Buffer8 :: mb.MemoryBuffer(Pixel8)
Rect2Du32 :: util.Rect2Du32


rgba_to_pixel :: proc(r: u8, g: u8, b: u8, a: u8) -> Pixel32
{
    return Pixel32 {
        red = r,
        green = g,
        blue = b,
        alpha = a
    }
}


rgb_to_pixel :: proc(r: u8, g: u8, b: u8) -> Pixel32
{
    return Pixel32 {
        red = r,
        green = g,
        blue = b,
        alpha = 255
    }
}


to_pixel :: proc {
    rgba_to_pixel,
    rgb_to_pixel
}


BLACK := Pixel32 { 0, 0, 0, 255 }
WHITE := Pixel32 { 255, 255, 255, 255 }


ImageView :: struct
{
    width: u32,
    height: u32,

    data: []Pixel32  // !!! Pixel
}


GrayView :: struct
{
    width: u32,
    height: u32,

    data: []Pixel8
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
    res := mb.create_buffer(buffer, n_pixels)

    return res == .OK
}


destroy_buffer32 :: proc(buffer: ^Buffer32)
{
    mb.destroy_buffer(buffer)
}


create_buffer8 :: proc(buffer: ^Buffer8, n_pixels: u32) -> bool
{
    res := mb.create_buffer(buffer, n_pixels)

    return res == .OK
}


destroy_buffer8 :: proc(buffer: ^Buffer8)
{
    mb.destroy_buffer(buffer)
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


push_view_32 :: proc(buffer: ^Buffer32, view: ^ImageView) -> bool
{
    w := view.width
    h := view.height

    if w == 0 || h == 0
    {
        return false
    }

    data, res := mb.push_elements(buffer, w * h)
    if res == .OK
    {
        return false
    }

    view.data = data

    return true
}


push_view_8 :: proc(buffer: ^Buffer8, view: ^GrayView) -> bool
{
    w := view.width
    h := view.height

    if w == 0 || h == 0
    {
        return false
    }

    data, res := mb.push_elements(buffer, w * h)
    if res == .OK
    {
        return false
    }

    view.data = data

    return true
}


push_view :: proc { push_view_32, push_view_8 }


make_view_32 :: proc(buffer: ^Buffer32, width: u32, height: u32) -> ImageView
{
   view: ImageView

    view.width = width
    view.height = height

    if !push_view(buffer, &view)
    {
        view.width = 0
        view.height = 0
    }

    return view
}


make_view_8 :: proc(buffer: ^Buffer8, width: u32, height: u32) -> GrayView
{
   view: GrayView

    view.width = width
    view.height = height

    if !push_view(buffer, &view)
    {
        view.width = 0
        view.height = 0
    }

    return view
}


make_view :: proc { make_view_32, make_view_8 }


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
    slice.fill(view.data, color)
}