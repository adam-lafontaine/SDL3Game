package image_view

import "core:slice"
import "../util"
import mb "../util/memory_buffer"
import span "../util/span_view"

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


u8_to_pixel :: proc(gray: u8) -> Pixel32
{
    return rgb_to_pixel(gray, gray, gray)
}


to_pixel :: proc {
    rgba_to_pixel,
    rgb_to_pixel,
    u8_to_pixel
}


BLACK :: Pixel32 { 0, 0, 0, 255 }
WHITE :: Pixel32 { 255, 255, 255, 255 }


View2D :: struct($T: typeid)
{
    width: u32,
    height: u32,

    data: []T
}


SubView2D :: struct($T: typeid)
{
    data: []T,
    view_width: u32,

    x_begin: u32,
    y_begin: u32,

    width: u32,
    height: u32
}


ImageView :: View2D(Pixel32)
GrayView :: View2D(Pixel8)

SubView :: SubView2D(Pixel32)
GraySubView :: SubView2D(Pixel8)


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


make_rect_xywh :: proc(x: u32, y: u32, w: u32, h: u32) -> Rect2Du32
{
    return Rect2Du32 {
        x_begin = x,
        x_end = x + w,
        y_begin = y,
        y_end = y + h
    }
}


make_rect_wh :: proc(w: u32, h: u32) -> Rect2Du32
{
    return make_rect_xywh(0, 0, w, h)
}


make_rect :: proc{ make_rect_xywh, make_rect_wh }


push_view2D :: proc (buffer: ^mb.MemoryBuffer($T), view: ^View2D(T)) -> bool
{
    w := view.width
    h := view.height

    if w == 0 || h == 0
    {
        return false
    }

    data, res := mb.push_elements(buffer, w * h)
    if res != .OK
    {
        return false
    }

    view.data = data

    return true
}


push_view_32 :: proc(buffer: ^Buffer32, view: ^ImageView) -> bool
{
    return push_view2D(buffer, view)
}


push_view_8 :: proc(buffer: ^Buffer8, view: ^GrayView) -> bool
{
    return push_view2D(buffer, view)
}


push_view :: proc { push_view_32, push_view_8 }


make_view2D :: proc(buffer: ^mb.MemoryBuffer($T), width: u32, height: u32) -> View2D(T)
{
    view: View2D(T)

    view.width = width
    view.height = height

    if !push_view(buffer, &view)
    {
        view.width = 0
        view.height = 0
    }

    return view
}


make_view_32 :: proc(buffer: ^Buffer32, width: u32, height: u32) -> ImageView
{
   return make_view2D(buffer, width, height)
}


make_view_8 :: proc(buffer: ^Buffer8, width: u32, height: u32) -> GrayView
{
   return make_view2D(buffer, width, height)
}


make_view :: proc { make_view_32, make_view_8 }


sub_view_v :: proc(view: View2D($T), rect: Rect2Du32) -> SubView2D(T)
{
    return SubView2D(T) {
        data = view.data,
        view_width = view.width,
        x_begin = rect.x_begin,
        y_begin = rect.y_begin,
        width = rect.x_end - rect.x_begin,
        height = rect.y_end - rect.y_begin
    }
}


sub_view_sv :: proc(view: SubView2D($T), rect: Rect2Du32) -> SubView2D(T)
{
    return SubView2D(T) {
        data = view.data,
        view_width = view.view_width,
        x_begin = rect.x_begin + view.x_begin,
        y_begin = rect.y_begin + view.y_begin,
        width = rect.x_end - rect.x_begin,
        height = rect.y_end - rect.y_begin
    }
}


sub_view :: proc{ sub_view_v, sub_view_sv}


row_span_v :: proc(view: View2D($T), y: u32) -> span.SpanView(T)
{
    begin := y * view.width
    end := begin + view.width

    return {
        data = view.data[begin:end]
    }
}


row_span_sv :: proc(view: SubView2D($T), y: u32) -> span.SpanView(T)
{
    begin := (view.y_begin + y) * view.view_width + view.x_begin
    end := begin + view.width
    
    return {
        data = view.data[begin:end]
    }
}


row_span :: proc{ row_span_v, row_span_sv }


pixel_at_v :: proc(view: View2D($T), x: u32, y: u32) -> T
{
    return row_span(view, y).data[x]
}


pixel_at_sv :: proc(view: SubView2D($T), x: u32, y: u32) -> T
{
    return row_span(view, y).data[x]
}


pixel_at :: proc{pixel_at_v, pixel_at_sv}


fill_v :: proc(view: ImageView, color: Pixel32)
{
    slice.fill(view.data, color)
}


fill_sv :: proc(view: SubView, color: Pixel32)
{
    for y in 0..<view.height
    {
        row := row_span(view, y).data
        slice.fill(row, color)
    }
}


fill :: proc{ fill_v, fill_sv }