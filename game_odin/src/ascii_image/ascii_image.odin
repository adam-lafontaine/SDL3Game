package ascii_image

import img "../image_view"
import "core:math"

p32 :: img.Pixel32
SubView :: img.SubView
MaskView :: img.GrayView
MaskSubView :: img.GraySubView
RectPx :: img.Rect2Du32


Font :: enum 
{
    Ascii5,
    Joystick8
}


@(private="file")
make_char_mask :: proc(ascii: FontDef, c: u8) -> MaskView
{
    begin :: cast(u8)(' ')
    last :: cast(u8)('~')
    err :: cast(u8)('?')

    id := (begin <= c && c <= last) ? c - begin : err - begin

    return MaskView {
        width = ascii.widths[id],
        height = ascii.height,
        data = transmute([]u8)string(ascii.u8_pixel_data[id])
    }
}


@(private="file")
count_text_width :: proc(ascii: FontDef, text: []u8) -> u32
{
    begin :: cast(u8)(' ')
    last :: cast(u8)('~')

    width: u32 = 0

    for c in text
    {
        width +=  (begin <= c && c <= last) ? 1 : 0
    }

    return width
}


@(private="file")
make_font_mask :: proc(font: Font, c: u8) -> MaskView
{
    fd: FontDef

    switch font
    {
        case .Ascii5: fd = font_ascii5
        case .Joystick8: fd = font_joystick8
        case:
    }
    
    return make_char_mask(fd, c)
}



@(private="file")
font_height :: proc(font: Font) -> u32
{
    switch font
    {
        case .Ascii5: return font_ascii5.height
        case .Joystick8: return font_joystick8.height
        case: return 0
    }
}


@(private="file")
transform_mask :: proc(src: MaskSubView, dst: SubView, color: p32)
{
    for y in 0..<src.height
    {
        s := img.row_span(src, y).data
        d := img.row_span(dst, y).data

        for ms, x in s
        {
            if ms > '0'
            {
                d[x] = color
            }
        }
    }
}


render_text ::proc(text: string, dst: SubView, font: Font, color: p32)
{
    sw: u32 = 0
    sh := math.min(font_height(font), dst.height)
    w_remaining := cast(i32)dst.width

    d_range: RectPx
    d_range.x_begin = 0
    d_range.x_end = 0
    d_range.y_begin = 0
    d_range.y_end = sh

    L := cast(u32)len(text)

    for i: u32 = 0; i < L && w_remaining > 0; i += 1
    {
        mask := make_font_mask(font, text[i])

        sw = math.min(mask.width, cast(u32)w_remaining)
        d_range.x_end += sw

        s_view := img.sub_view(mask, img.make_rect(sw, sh))
        d_view := img.sub_view(dst, d_range)

        transform_mask(s_view, d_view, color);

        d_range.x_begin = d_range.x_end;
        w_remaining -= cast(i32)sw;
    }

}