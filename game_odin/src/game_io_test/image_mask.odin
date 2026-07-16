#+private
package game_io_test

import img "../image_view"
import "../util"


p32 :: img.Pixel32
MaskView :: img.GrayView
Mask :: img.GraySubView
Buffer8 :: img.Buffer8


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


/* controller */

ControllerDef :: struct($T: typeid) #raw_union
{
    list: [16]T,

    items: struct {
        dpad_up: T,
        dpad_down: T,
        dpad_left: T,
        dpad_right: T,

        a: T,
        b: T,
        x: T,
        y: T,

        start: T,
        back: T,

        shoulder_left: T,
        shoulder_right: T,

        stick_left: T,
        stick_right: T,

        trigger_left: T,
        trigger_right: T,
    }
}


ControllerStickDef :: struct($T: typeid)
{
    stick_left: T,
    stick_right: T
}


ControllerMaskList :: ControllerDef(Mask)
ControllerRectList :: ControllerDef(util.Rect2Du32)


set_mask_regions_ctlr :: proc(view: MaskView, reg: ControllerRectList, masks: ^ControllerMaskList)
{
    for i in 0..<len(reg.list)
    {
        masks.list[i] = img.sub_view(view, reg.list[i])
    }
}


get_region_rects_ctlr :: proc() -> ControllerRectList
{
    r: ControllerRectList

    r.items = {
        trigger_left  = img.make_rect(17, 4, 19, 15),
        trigger_right = img.make_rect(156, 4, 19, 15),

        shoulder_left  = img.make_rect(17, 21, 19, 8),
        shoulder_right = img.make_rect(156, 21, 19, 8),

        back  = img.make_rect(74, 23, 16, 9),
        start = img.make_rect(102, 23, 16, 9),

        dpad_up    = img.make_rect(21, 32, 10, 17),
        dpad_down  = img.make_rect(21, 60, 10, 17),
        dpad_left  = img.make_rect(5, 50, 17, 10),
        dpad_right = img.make_rect(32, 50, 17, 10),

        stick_left  = img.make_rect(59, 44, 25, 25),
        stick_right = img.make_rect(108, 44, 25, 25),

        a = img.make_rect(158, 62, 15, 15),
        b = img.make_rect(173, 47, 15, 15),
        x = img.make_rect(143, 47, 15, 15),
        y = img.make_rect(158, 32, 15, 15)
    }

    return r
}


/* keyboard */

KeyboardDef :: struct($T: typeid) #raw_union
{
    list: [9]T,

    items: struct {
        n_1: T,
        n_2: T,
        n_3: T,
        n_4: T,

        w: T,
        a: T,
        s: T,
        d: T,

        space: T,
    }
}


KeyboardMaskList :: KeyboardDef(Mask)
KeyboardRectList :: KeyboardDef(util.Rect2Du32)


set_mask_regions_kbd :: proc(view: MaskView, reg: KeyboardRectList, masks: ^KeyboardMaskList)
{
    for i in 0..<len(reg.list)
    {
        masks.list[i] = img.sub_view(view, reg.list[i])
    }
}


get_region_rects_kbd :: proc() -> KeyboardRectList
{
    r: KeyboardRectList

    r.items = {
        n_1 = img.make_rect(20, 2, 16, 16),
        n_2 = img.make_rect(38, 2, 16, 16),
        n_3 = img.make_rect(56, 2, 16, 16),
        n_4 = img.make_rect(74, 2, 16, 16),

        w = img.make_rect(47, 20, 16, 16),
        a = img.make_rect(34, 38, 16, 16),
        s = img.make_rect(52, 38, 16, 16),
        d = img.make_rect(70, 38, 16, 16),

        space = img.make_rect(83, 74, 106, 16),
    }

    return r
}


/* mouse */

MouseDef :: struct($T: typeid) #raw_union
{
    list: [5]T,

    items: struct {
        left: T,
        right: T,
        middle: T,

        pos_x: T,
        pos_y: T,
    }
}


MouseMaskList :: MouseDef(Mask)
MouseRectList :: MouseDef(util.Rect2Du32)


set_mask_regions_mouse :: proc(view: MaskView, reg: MouseRectList, masks: ^MouseMaskList)
{
    for i in 0..<len(reg.list)
    {
        masks.list[i] = img.sub_view(view, reg.list[i])
    }
}


get_region_rects_mouse :: proc() -> MouseRectList
{
    r: MouseRectList

    r.items = {
        left   = img.make_rect(1, 1, 30, 30),
        right  = img.make_rect(49, 1, 30, 30),
        middle = img.make_rect(32, 1, 14, 30),
        pos_x  = img.make_rect(6, 44, 68, 8),
        pos_y  = img.make_rect(6, 56, 68, 8),
    }

    return r
}


/* mask data */

DrawMaskData :: struct
{
    controller: ControllerMaskList,
    keyboard: KeyboardMaskList,
    mouse: MouseMaskList,

    controller_view: MaskView,
    keyboard_view: MaskView,
    mouse_view: MaskView,

    arrow_view: MaskView
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

    set_mask_regions_ctlr(cmv, cr, &data.controller)
    set_mask_regions_kbd(kmv, kr, &data.keyboard)
    set_mask_regions_mouse(mmv, mr, &data.mouse)

    data.controller_view = cmv
    data.keyboard_view = kmv
    data.mouse_view = mmv
    data.arrow_view = amv

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


ControllerMaskViewMap :: ControllerDef(MaskViewMap)
KeyboardMaskViewMap :: KeyboardDef(MaskViewMap)
MouseMaskViewMap :: MouseDef(MaskViewMap)
ControllerStickMaskViewMap :: ControllerStickDef(MaskViewMap)


draw_masks :: proc(mv: $MV, on_off: $O)
{
    N := len(mv.list)

    for i in 0..<N
    {
        draw_map(&mv.list[i], on_off.list[i])
    }
}


set_map_masks :: proc(m: $M, mv: $MV)
{
    N := len(mv.list)

    for i in 0..<N
    {
        item := &mv.list[i]
        item.mask = m.list[i]
    }
}


set_map_out :: proc(out: SubView, r: $R, mv: $KV)
{
    N := len(mv.list)

    for i in 0..<N
    {
        item := &mv.list[i]
        item.out = img.sub_view(out, r.list[i])
    }
}


MaskViewMapList :: struct
{
    controller1: MaskViewMap,
    controller2: MaskViewMap,
    keyboard: MaskViewMap,
    mouse: MaskViewMap,

    controller1_inputs: ControllerMaskViewMap,
    controller2_inputs: ControllerMaskViewMap,
    keyboard_inputs: KeyboardMaskViewMap,
    mouse_inputs: MouseMaskViewMap,

    controller1_thumbsticks: ControllerStickMaskViewMap,
    controller2_thumsticks: ControllerStickMaskViewMap,
}


draw_map_list :: proc(mv: ^MaskViewMapList, input: InputList)
{
    draw_map(&mv.controller1, false)
    draw_map(&mv.controller2, false)
    draw_map(&mv.keyboard, false)
    draw_map(&mv.mouse, false)

    draw_masks(&mv.controller1_inputs, input.controller1)
    draw_masks(&mv.controller2_inputs, input.controller2)
    draw_masks(&mv.keyboard_inputs, input.keyboard)
    draw_masks(&mv.mouse_inputs, input.mouse)

    // thumbsticks, mouse coords
}


set_mask_list_views :: proc(masks: DrawMaskData, out: ImageView, mv: ^MaskViewMapList)
{
    sub_full :: proc(v: GrayView) -> GraySubView { return img.sub_view(v, img.make_rect(v.width, v.height)) }

    c_mask := masks.controller_view    
    cw := c_mask.width
    ch := c_mask.height

    k_mask := masks.keyboard_view
    kw := k_mask.width
    kh := k_mask.height

    m_mask := masks.mouse_view
    mw := m_mask.width
    mh := m_mask.height

    sw := out.width
    sh := out.height

    c_out1 := img.sub_view(out, img.make_rect(0, 0, cw, ch))
    mv.controller1.mask = sub_full(c_mask)
    mv.controller1.out = c_out1

    c_out2 := img.sub_view(out, img.make_rect(sw - cw, 0, cw, ch))
    mv.controller2.mask = sub_full(c_mask)
    mv.controller2.out = c_out2

    k_out := img.sub_view(out, img.make_rect(0, sh - kh, kw, kh))
    mv.keyboard.mask = sub_full(k_mask)
    mv.keyboard.out = k_out

    m_out := img.sub_view(out, img.make_rect(sw - mw, sh - mh, mw, mh))
    mv.mouse.mask = sub_full(m_mask)
    mv.mouse.out = m_out

    c_reg := get_region_rects_ctlr()
    k_reg := get_region_rects_kbd()
    m_reg := get_region_rects_mouse()

    set_map_out(c_out1, c_reg, &mv.controller1_inputs)
    set_map_out(c_out2, c_reg, &mv.controller2_inputs)
    set_map_out(k_out, k_reg, &mv.keyboard_inputs)
    set_map_out(m_out, m_reg, &mv.mouse_inputs)

    set_map_masks(masks.controller, &mv.controller1_inputs)
    set_map_masks(masks.controller, &mv.controller2_inputs)
    set_map_masks(masks.keyboard, &mv.keyboard_inputs)
    set_map_masks(masks.mouse, &mv.mouse_inputs)
}
