#+private
package game_io_test

import img "../image_view"
import ascii "../ascii_image"

import "core:fmt"
import "core:math"


@(private="file")
mask_set_on :: proc(p: u8, default: p32) -> p32
{
    m := cast(MaskPixel)p

    switch m
    {
    case .Black: return COLOR_BLACK
    case .Color: return COLOR_ON
    case .Default: return default
    case: return default
    }
}


@(private="file")
mask_set_off :: proc(p: u8, default: p32) -> p32
{
    m := cast(MaskPixel)p

    switch m
    {
    case .Black: return COLOR_BLACK
    case .Color: return COLOR_OFF
    case .Default: return default
    case: return default
    }
}



@(private="file")
draw_map :: proc(mv_map: ^MaskViewMap, is_on: b8)
{
    set_mask := is_on ? mask_set_on : mask_set_off

    src := mv_map.mask
    dst := mv_map.out

    for y in 0..<dst.height
    {
        s := img.row_span(src, y).data
        d := img.row_span(dst, y).data
        
        for mp, x in s
        {
            d[x] = set_mask(mp, d[x])
        }
    }
}


@(private="file")
draw_map_rotated :: proc(mv_map: ^MaskViewMap, sin_cos: Vec2Df32)
{
    mask_set :: proc(m: u8, p: p32) -> p32 
    { 
        return cast(MaskPixel)m == .Color ? COLOR_BLACK : p
    }

    src := mv_map.mask
    dst := mv_map.out

    sw := src.width
    sh := src.height

    dw := dst.width
    dh := dst.height

    spx := cast(f32)sw / 2
    spy := cast(f32)sh / 2

    dpx := cast(f32)dw / 2
    dpy := cast(f32)dh / 2

    cos := sin_cos.x
    sin := sin_cos.y

    dycos := spy - dpy * cos + dpx * sin + sin
    dysin := spx - dpy * sin - dpx * cos - cos

    sx: u32 = 0
    sy: u32 = 0

    sxf: f32 = 0
    syf: f32 = 0

    for y in 0..<dh
    {
        d := img.row_span(dst, y).data

        sxf = dysin
        syf = dycos

        for x in 0..<dw
        {
            sxf += cos
            syf -= sin

            if sxf < 0 || syf < 0
            {
                continue;
            }

            sx = cast(u32)sxf
            sy = cast(u32)syf

            if sx >= sw || sy >= sh
            {
                continue
            }

            mp := img.pixel_at(src, sx, sy)
            d[x] = mask_set(mp, d[x])
        }

        dysin += sin
        dycos += cos
    }
}


@(private="file")
draw_masks :: proc(mv: $MV, on_off: $O)
{
    for _, id in mv
    {
        draw_map(&mv[id], on_off[id])
    }
}


@(private="file")
draw_mouse_coords :: proc(mv: ^MouseMaskViewMap, pos: Vec2Di32)
{
    font := ascii.Font.Joystick8
    color := COLOR_BLACK

    buffer_x: [16]u8
    buffer_y: [16]u8

    out := mv[.pos_x].out
    img.fill(out, COLOR_BACKGROUND)
    str_x := fmt.bprintf(buffer_x[:], "X: %i", pos.x)
    ascii.render_text(str_x, out, font, color)

    out = mv[.pos_y].out
    img.fill(out, COLOR_BACKGROUND)
    str_y := fmt.bprintf(buffer_y[:], "Y: %i", pos.y)
    ascii.render_text(str_y, out, font, color)
}


@(private="file")
draw_gamepad_thumbsticks :: proc(mv: ^GamepadStickMaskViewMap, rot: GamepadStickRotation)
{
    is_on :: proc(v: Vec2Df32) -> bool { return v.x != 0 || v.y != 0 }

    if is_on(rot.stick_left)
    {
        draw_map_rotated(&mv.stick_left, rot.stick_left)
    }

    if is_on(rot.stick_right)
    {
        draw_map_rotated(&mv.stick_right, rot.stick_right)
    }
}


draw_map_list :: proc(mv: ^MaskViewMapList, input: InputList)
{    
    draw_map(&mv.keyboard, false)
    draw_map(&mv.mouse, false)
    draw_map(&mv.gamepad1, false)
    draw_map(&mv.gamepad2, false)
    
    draw_masks(&mv.keyboard_inputs, input.keyboard)

    draw_masks(&mv.mouse_inputs, input.mouse)
    draw_mouse_coords(&mv.mouse_inputs, input.mouse_pos)

    draw_masks(&mv.gamepad1_inputs, input.gamepad1)
    draw_masks(&mv.gamepad2_inputs, input.gamepad2)

    draw_gamepad_thumbsticks(&mv.gamepad1_thumbsticks, input.sticks1)
    draw_gamepad_thumbsticks(&mv.gamepad2_thumbsticks, input.sticks2)
}



