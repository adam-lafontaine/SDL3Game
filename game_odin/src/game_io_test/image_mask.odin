#+private
package game_io_test

import img "../image_view"
import ascii "../ascii_image"

import "core:fmt"
import "core:math"


p32 :: img.Pixel32
MaskView :: img.GrayView
Mask :: img.GraySubView
Buffer8 :: img.Buffer8
RectPx :: img.Rect2Du32


MaskPixel :: enum
{
    Default = 0,
    Black = 1,
    Color = 2
}


@(private="file")
to_mask_pixel :: proc(p: p32) -> MaskPixel
{
    if p.alpha == 0 // transparent
    {
        return .Default
    }

    sum := p.red + p.green + p.blue
    if sum == 0 // black
    {
        return .Black
    }

    return .Color
}


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
make_mask_view :: proc(img32: ImageView, buffer: ^Buffer8) -> MaskView
{
    w := img32.width
    h := img32.height

    mask := img.make_view(buffer, w, h)

    s := img32.data
    d := mask.data

    for i in 0..<len(s) // transform?
    {
        d[i] = cast(u8)to_mask_pixel(s[i])
    }

    return mask
}


@(private="file")
set_mask_regions :: proc(view: MaskView, r: [$N]RectPx, m: ^[N]Mask)
{
    for rect, id in r
    {
        m[id] = img.sub_view(view, rect)
    }
}


/* gamepad */

GamepadStickDef :: struct($T: typeid)
{
    stick_left: T,
    stick_right: T
}



KeyboardMaskList :: [KeyboardId]Mask
KeyboardRectList :: [KeyboardId]RectPx

MouseMaskList :: [MouseId]Mask
MouseRectList :: [MouseId]RectPx

GamepadMaskList :: [GamepadId]Mask
GamepadRectList :: [GamepadId]RectPx



@(private="file")
get_region_rects_ctlr :: proc() -> GamepadRectList
{
    list: GamepadRectList

    for id in GamepadId
    {
        r := CTLR_MASK_RECT[id]
        list[id] = img.make_rect(r.x, r.y, r.w, r.h)
    }

    return list
}


@(private="file")
get_region_rects_kbd :: proc() -> KeyboardRectList
{
    list: KeyboardRectList

    for id in KeyboardId
    {
        r := KBD_MASK_RECT[id]
        list[id] = img.make_rect(r.x, r.y, r.w, r.h)
    }

    return list
}


@(private="file")
get_region_rects_mouse :: proc() -> MouseRectList
{
    list: MouseRectList

    for id in MouseId
    {
        r := MSE_MASK_RECT[id]
        list[id] = img.make_rect(r.x, r.y, r.w, r.h)
    }

    return list
}


/* mask data */

DrawMaskData :: struct
{
    keyboard: KeyboardMaskList,
    mouse: MouseMaskList,
    gamepad: GamepadMaskList,
    
    keyboard_view: MaskView,
    mouse_view: MaskView,
    gamepad_view: MaskView,

    arrow_view: MaskView
}


draw_mask_pixel_count ::proc(am: AssetMemory) -> u32
{
    g := am.image.gamepad
    k := am.image.keyboard
    m := am.image.mouse
    a := am.image.arrow

    gn := g.width * g.height
    kn := k.width * k.height
    mn := m.width * m.height
    an := a.width * a.height

    return gn + kn + mn + an
}


create_draw_mask_data :: proc(am: AssetMemory, buffer: ^Buffer8) -> DrawMaskData
{
    gr := get_region_rects_ctlr()
    kr := get_region_rects_kbd()
    mr := get_region_rects_mouse()

    data: DrawMaskData

    gmv := make_mask_view(am.image.gamepad, buffer)
    kmv := make_mask_view(am.image.keyboard, buffer)
    mmv := make_mask_view(am.image.mouse, buffer)
    amv := make_mask_view(am.image.arrow, buffer)
    
    set_mask_regions(kmv, kr, &data.keyboard)
    set_mask_regions(mmv, mr, &data.mouse)
    set_mask_regions(gmv, gr, &data.gamepad)
    
    data.keyboard_view = kmv
    data.mouse_view = mmv
    data.gamepad_view = gmv
    data.arrow_view = amv

    return data
}


MaskViewMap :: struct
{
    mask: Mask,
    out: SubView
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
draw_map_rotated :: proc(mv_map: ^MaskViewMap, sin_cos: Vec2Df32, is_on: b8)
{
    set_mask := is_on ? mask_set_on : mask_set_off

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

        sxf += cos
        syf -= sin

        for x in 0..<dw
        {
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
            d[x] = set_mask(mp, d[x])
        }

        dysin += sin
        dycos += cos
    }
}


KeyboardMaskViewMap :: [KeyboardId]MaskViewMap
MouseMaskViewMap :: [MouseId]MaskViewMap
GamepadMaskViewMap :: [GamepadId]MaskViewMap
GamepadStickMaskViewMap :: GamepadStickDef(MaskViewMap)


@(private="file")
sub_full :: proc(v: GrayView) -> GraySubView 
{ 
    return img.sub_view(v, img.make_rect(v.width, v.height)) 
}


@(private="file")
set_map_masks :: proc(m: $M, mv: $MV)
{
    for mask, id in m
    {
        mv[id].mask = mask
    }
}


@(private="file")
set_map_out :: proc(out: SubView, r: $R, mv: $MV)
{
    for rect, id in r
    {
        mv[id].out = img.sub_view(out, rect)
    }
}


set_map_masks_ts :: proc(m: GraySubView, mv: ^GamepadStickMaskViewMap)
{
    mv.stick_left.mask = m;
    mv.stick_right.mask = m;
}


set_map_out_ts :: proc(out: SubView, r: GamepadRectList, mv: ^GamepadStickMaskViewMap)
{
    mv.stick_left.out = img.sub_view(out, r[.stick_left])
    mv.stick_right.out = img.sub_view(out, r[.stick_right])
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
    is_on :: proc(v: Vec2Df32) -> bool { return v.x > 0 || v.y > 0 }

    if is_on(rot.stick_left)
    {
        draw_map_rotated(&mv.stick_left, rot.stick_left, true)
    }

    if is_on(rot.stick_right)
    {
        draw_map_rotated(&mv.stick_right, rot.stick_right, true)
    }
}


MaskViewMapList :: struct
{
    keyboard: MaskViewMap,
    mouse: MaskViewMap,
    gamepad1: MaskViewMap,
    gamepad2: MaskViewMap,
    
    keyboard_inputs: KeyboardMaskViewMap,
    mouse_inputs: MouseMaskViewMap,
    gamepad1_inputs: GamepadMaskViewMap,
    gamepad2_inputs: GamepadMaskViewMap,

    gamepad1_thumbsticks: GamepadStickMaskViewMap,
    gamepad2_thumbsticks: GamepadStickMaskViewMap,
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


set_mask_list_views :: proc(masks: DrawMaskData, out: ImageView, mv: ^MaskViewMapList)
{
    sw := out.width
    sh := out.height    

    // keyboard
    k_mask := sub_full(masks.keyboard_view)
    kw := k_mask.width
    kh := k_mask.height
    k_out := img.sub_view(out, img.make_rect(0, sh - kh, kw, kh))    
    mv.keyboard.mask = k_mask
    mv.keyboard.out = k_out
    k_reg := get_region_rects_kbd()
    set_map_out(k_out, k_reg, &mv.keyboard_inputs)
    set_map_masks(masks.keyboard, &mv.keyboard_inputs)

    // mouse
    m_mask := sub_full(masks.mouse_view)
    mw := m_mask.width
    mh := m_mask.height
    m_out := img.sub_view(out, img.make_rect(sw - mw, sh - mh, mw, mh))
    mv.mouse.mask = m_mask
    mv.mouse.out = m_out
    m_reg := get_region_rects_mouse()
    set_map_out(m_out, m_reg, &mv.mouse_inputs)
    set_map_masks(masks.mouse, &mv.mouse_inputs)

    // gamepad
    g_mask := sub_full(masks.gamepad_view)
    
    cw := g_mask.width
    ch := g_mask.height
    g_reg := get_region_rects_ctlr()

    // gamepad 1
    g_out1 := img.sub_view(out, img.make_rect(0, 0, cw, ch))
    mv.gamepad1.mask = g_mask
    mv.gamepad1.out = g_out1
    set_map_masks(masks.gamepad, &mv.gamepad1_inputs)
    set_map_out(g_out1, g_reg, &mv.gamepad1_inputs)    

    // gamepad 2
    g_out2 := img.sub_view(out, img.make_rect(sw - cw, 0, cw, ch))
    mv.gamepad2.mask = g_mask
    mv.gamepad2.out = g_out2
    set_map_masks(masks.gamepad, &mv.gamepad2_inputs)
    set_map_out(g_out2, g_reg, &mv.gamepad2_inputs)    

    // thumbsticks
    a_mask := sub_full(masks.arrow_view)

    set_map_masks_ts(a_mask, &mv.gamepad1_thumbsticks)
    set_map_out_ts(g_out1, g_reg, &mv.gamepad1_thumbsticks)

    set_map_masks_ts(a_mask, &mv.gamepad2_thumbsticks)
    set_map_out_ts(g_out2, g_reg, &mv.gamepad2_thumbsticks)

}
