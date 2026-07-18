#+private
package game_io_test

import img "../image_view"

import "core:fmt"
import "core:math"


p32 :: img.Pixel32
MaskView :: img.GrayView
Mask :: img.GraySubView
Buffer8 :: img.Buffer8
RectPx :: img.Rect2Du32


KeyboardRectList :: [KeyboardId]RectPx
MouseRectList :: [MouseId]RectPx
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


KeyboardMaskList :: [KeyboardId]Mask
MouseMaskList :: [MouseId]Mask
GamepadMaskList :: [GamepadId]Mask


/* mask data */

MaskViewData :: struct
{
    keyboard_masks: KeyboardMaskList,
    mouse_masks: MouseMaskList,
    gamepad_masks: GamepadMaskList,
    
    keyboard_view: MaskView,
    mouse_view: MaskView,
    gamepad_view: MaskView,

    arrow_view: MaskView
}


mask_view_pixel_count ::proc(am: AssetMemory) -> u32
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


create_mask_view_data :: proc(am: AssetMemory, buffer: ^Buffer8) -> MaskViewData
{
    gr := get_region_rects_ctlr()
    kr := get_region_rects_kbd()
    mr := get_region_rects_mouse()

    data: MaskViewData

    gmv := make_mask_view(am.image.gamepad, buffer)
    kmv := make_mask_view(am.image.keyboard, buffer)
    mmv := make_mask_view(am.image.mouse, buffer)
    amv := make_mask_view(am.image.arrow, buffer)
    
    set_mask_regions(kmv, kr, &data.keyboard_masks)
    set_mask_regions(mmv, mr, &data.mouse_masks)
    set_mask_regions(gmv, gr, &data.gamepad_masks)
    
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


GamepadStickDef :: struct($T: typeid)
{
    stick_left: T,
    stick_right: T
}


KeyboardMaskViewMap :: [KeyboardId]MaskViewMap
MouseMaskViewMap :: [MouseId]MaskViewMap
GamepadMaskViewMap :: [GamepadId]MaskViewMap
GamepadStickMaskViewMap :: GamepadStickDef(MaskViewMap)


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


@(private="file")
set_map_masks_ts :: proc(m: GraySubView, mv: ^GamepadStickMaskViewMap)
{
    mv.stick_left.mask = m;
    mv.stick_right.mask = m;
}


@(private="file")
set_map_out_ts :: proc(out: SubView, r: GamepadRectList, mv: ^GamepadStickMaskViewMap)
{
    mv.stick_left.out = img.sub_view(out, r[.stick_left])
    mv.stick_right.out = img.sub_view(out, r[.stick_right])
}


set_mask_list_views :: proc(masks: MaskViewData, out: ImageView, mv: ^MaskViewMapList)
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
    set_map_masks(masks.keyboard_masks, &mv.keyboard_inputs)

    // mouse
    m_mask := sub_full(masks.mouse_view)
    mw := m_mask.width
    mh := m_mask.height
    m_out := img.sub_view(out, img.make_rect(sw - mw, sh - mh, mw, mh))
    mv.mouse.mask = m_mask
    mv.mouse.out = m_out
    m_reg := get_region_rects_mouse()
    set_map_out(m_out, m_reg, &mv.mouse_inputs)
    set_map_masks(masks.mouse_masks, &mv.mouse_inputs)

    // gamepad
    g_mask := sub_full(masks.gamepad_view)
    
    cw := g_mask.width
    ch := g_mask.height
    g_reg := get_region_rects_ctlr()

    // gamepad 1
    g_out1 := img.sub_view(out, img.make_rect(0, 0, cw, ch))
    mv.gamepad1.mask = g_mask
    mv.gamepad1.out = g_out1
    set_map_masks(masks.gamepad_masks, &mv.gamepad1_inputs)
    set_map_out(g_out1, g_reg, &mv.gamepad1_inputs)    

    // gamepad 2
    g_out2 := img.sub_view(out, img.make_rect(sw - cw, 0, cw, ch))
    mv.gamepad2.mask = g_mask
    mv.gamepad2.out = g_out2
    set_map_masks(masks.gamepad_masks, &mv.gamepad2_inputs)
    set_map_out(g_out2, g_reg, &mv.gamepad2_inputs)    

    // thumbsticks
    a_mask := sub_full(masks.arrow_view)

    set_map_masks_ts(a_mask, &mv.gamepad1_thumbsticks)
    set_map_out_ts(g_out1, g_reg, &mv.gamepad1_thumbsticks)

    set_map_masks_ts(a_mask, &mv.gamepad2_thumbsticks)
    set_map_out_ts(g_out2, g_reg, &mv.gamepad2_thumbsticks)
}