#+private
package game_io_test

import img "../image_view"
import "../util"


p32 :: img.Pixel32
MaskView :: img.GrayView
Mask :: img.GraySubView
Buffer8 :: img.Buffer8
RectPx :: util.Rect2Du32


MaskPixel :: enum
{
    Default = 0,
    Black,
    Color
}


to_mask_pixel :: proc(p: p32) -> MaskPixel
{
    if p.alpha == 0 // transparent
    {
        return .Default
    }

    sum := p.red + p.blue + p.alpha
    if sum == 0 // black
    {
        return .Black
    }

    return .Color
}


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


set_mask_regions :: proc(view: MaskView, r: [$N]RectPx, m: ^[N]Mask)
{
    for rect, id in r
    {
        m[id] = img.sub_view(view, rect)
    }
}


/* controller */

ControllerStickDef :: struct($T: typeid)
{
    stick_left: T,
    stick_right: T
}


ControllerMaskList :: [ControllerId]Mask
ControllerRectList :: [ControllerId]RectPx


get_region_rects_ctlr :: proc() -> ControllerRectList
{
    list: ControllerRectList

    for id in ControllerId
    {
        r := CTLR_MASK_RECT[id]
        list[id] = img.make_rect(r.x, r.y, r.w, r.h)
    }

    return list
}


/* keyboard */

KeyboardMaskList :: [KeyboardId]Mask
KeyboardRectList :: [KeyboardId]RectPx


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


/* mouse */

MouseMaskList :: [MouseId]Mask
MouseRectList :: [MouseId]RectPx


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
    //ontroller: ControllerMaskList,
    keyboard: KeyboardMaskList,
    //mouse: MouseMaskList,

    //controller_view: MaskView,
    keyboard_view: MaskView,
    //mouse_view: MaskView,

    //arrow_view: MaskView
}


draw_mask_pixel_count ::proc(am: AssetMemory) -> u32
{
    c := am.image.controller
    k := am.image.keyboard
    m := am.image.mouse
    a := am.image.arrow

    cn := c.width * c.height
    kn := k.width * k.height
    mn := m.width * m.height
    an := a.width * a.height

    return cn + kn + mn + an
}


create_draw_mask_data :: proc(am: AssetMemory, buffer: ^Buffer8) -> DrawMaskData
{
    cr := get_region_rects_ctlr()
    kr := get_region_rects_kbd()
    mr := get_region_rects_mouse()

    data: DrawMaskData

    cmv := make_mask_view(am.image.controller, buffer)
    kmv := make_mask_view(am.image.keyboard, buffer)
    mmv := make_mask_view(am.image.mouse, buffer)
    amv := make_mask_view(am.image.arrow, buffer)

    //set_mask_regions(cmv, cr.list, &data.controller.list)
    set_mask_regions(kmv, kr, &data.keyboard)
    //set_mask_regions(mmv, mr.list, &data.mouse.list)

    //data.controller_view = cmv
    data.keyboard_view = kmv
    //data.mouse_view = mmv
    //data.arrow_view = amv

    return data
}


MaskViewMap :: struct
{
    mask: Mask,
    out: SubView
}


draw_map :: proc(mv_map: ^MaskViewMap, is_on: b8)
{
    set_mask := is_on ? mask_set_on : mask_set_off

    s := mv_map.mask.data
    d := mv_map.out.data    

    for i in 0..<len(s)
    {
        d[i] = set_mask(s[i], d[i])
    }
}


//ControllerMaskViewMap :: ControllerDef(MaskViewMap)
KeyboardMaskViewMap :: [KeyboardId]MaskViewMap
//MouseMaskViewMap :: MouseDef(MaskViewMap)
//ControllerStickMaskViewMap :: ControllerStickDef(MaskViewMap)


sub_full :: proc(v: GrayView) -> GraySubView 
{ 
    return img.sub_view(v, img.make_rect(v.width, v.height)) 
}


draw_masks :: proc(mv: $MV, on_off: $O)
{
    for _, id in mv
    {
        draw_map(&mv[id], on_off[id])
    }
}


set_map_masks :: proc(m: $M, mv: $MV)
{
    for mask, id in m
    {
        mv[id].mask = mask
    }
}


set_map_out :: proc(out: SubView, r: $R, mv: $MV)
{
    for rect, id in r
    {
        mv[id].out = img.sub_view(out, rect)
    }
}


/*set_map_masks_ts :: proc(m: GraySubView, mv: ^ControllerStickMaskViewMap)
{
    mv.stick_left.mask = m;
    mv.stick_right.mask = m;
}


set_map_out_ts :: proc(out: SubView, r: ControllerRectList, mv: ^ControllerStickMaskViewMap)
{
    mv.stick_left.out = img.sub_view(out, r.items.stick_left)
    mv.stick_right.out = img.sub_view(out, r.items.stick_right)
}*/


MaskViewMapList :: struct
{
    //controller1: MaskViewMap,
    //controller2: MaskViewMap,
    keyboard: MaskViewMap,
    //mouse: MaskViewMap,

    //controller1_inputs: ControllerMaskViewMap,
    //controller2_inputs: ControllerMaskViewMap,
    keyboard_inputs: KeyboardMaskViewMap,
    //mouse_inputs: MouseMaskViewMap,

    //controller1_thumbsticks: ControllerStickMaskViewMap,
    //controller2_thumbsticks: ControllerStickMaskViewMap,
}


draw_map_list :: proc(mv: ^MaskViewMapList, input: InputList)
{
    //draw_map(&mv.controller1, false)
    //draw_map(&mv.controller2, false)
    draw_map(&mv.keyboard, false)
    //draw_map(&mv.mouse, false)

    //draw_masks(&mv.controller1_inputs, input.controller1)
    //draw_masks(&mv.controller2_inputs, input.controller2)
    draw_masks(&mv.keyboard_inputs, input.keyboard)
    //draw_masks(&mv.mouse_inputs, input.mouse)

    // thumbsticks, mouse coords
}


set_mask_list_views :: proc(masks: DrawMaskData, out: ImageView, mv: ^MaskViewMapList)
{
    sw := out.width
    sh := out.height

    // controller
    /*c_mask := sub_full(masks.controller_view)
    a_mask := sub_full(masks.arrow_view)
    cw := c_mask.width
    ch := c_mask.height
    c_reg := get_region_rects_ctlr()

    // controller 1
    c_out1 := img.sub_view(out, img.make_rect(0, 0, cw, ch))
    mv.controller1.mask = c_mask
    mv.controller1.out = c_out1
    set_map_masks(masks.controller, &mv.controller1_inputs)
    set_map_out(c_out1, c_reg, &mv.controller1_inputs)
    // thumbstics
    set_map_masks_ts(a_mask, &mv.controller1_thumbsticks)
    set_map_out_ts(c_out1, c_reg, &mv.controller1_thumbsticks)

    // controller 2
    c_out2 := img.sub_view(out, img.make_rect(sw - cw, 0, cw, ch))
    mv.controller2.mask = c_mask
    mv.controller2.out = c_out2
    set_map_masks(masks.controller, &mv.controller2_inputs)
    set_map_out(c_out2, c_reg, &mv.controller2_inputs)
    // thumbstics
    set_map_masks_ts(a_mask, &mv.controller2_thumbsticks)
    set_map_out_ts(c_out2, c_reg, &mv.controller2_thumbsticks) */   

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
    /*m_mask := sub_full(masks.mouse_view)
    mw := m_mask.width
    mh := m_mask.height
    m_out := img.sub_view(out, img.make_rect(sw - mw, sh - mh, mw, mh))
    mv.mouse.mask = m_mask
    mv.mouse.out = m_out
    m_reg := get_region_rects_mouse()
    set_map_out(m_out, m_reg, &mv.mouse_inputs)
    set_map_masks(masks.mouse, &mv.mouse_inputs) */   
}
