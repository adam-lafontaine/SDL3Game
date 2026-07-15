#+private
package game_io_test

import img "../image_view"
import "../util"


p32 :: img.Pixel32
MaskView :: img.GrayView
Mask :: img.GraySubView
Buffer8 :: img.Buffer8


to_u8_mask :: proc(p: p32) -> u8
{
    if p.alpha == 0 // transparent
    {
        return 0
    }

    sum := p.red + p.blue + p.alpha
    if sum == 0 // black
    {
        return 1
    }

    return 2
}


make_mask_view :: proc(img32: ImageView, buffer: ^Buffer8) -> MaskView
{
    w := img32.width
    h := img32.height

    mask := img.make_view(buffer, w, h)

    s := img32.data
    d := mask.data

    for i in 0..<len(s)
    {
        d[i] = to_u8_mask(s[i])
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


CtlrMaskList :: ControllerDef(Mask)
CtlrRectList :: ControllerDef(util.Rect2Du32)


set_mask_regions_ctlr :: proc(view: MaskView, reg: CtlrRectList, masks: ^CtlrMaskList)
{
    for i in 0..<len(reg.list)
    {
        masks.list[i] = img.sub_view(view, reg.list[i])
    }
}


get_region_rects_ctlr :: proc() -> CtlrRectList
{
    r: CtlrRectList

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


KbdMaskList :: KeyboardDef(Mask)
KbdRectList :: KeyboardDef(util.Rect2Du32)


set_mask_regions_kbd :: proc(view: MaskView, reg: KbdRectList, masks: ^KbdMaskList)
{
    for i in 0..<len(reg.list)
    {
        masks.list[i] = img.sub_view(view, reg.list[i])
    }
}


get_region_rects_kbd :: proc() -> KbdRectList
{
    r: KbdRectList

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
    controller: CtlrMaskList,
    keyboard: KbdMaskList,
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

    set_mask_regions_ctlr(cmv, cr, data.controller)
    set_mask_regions_kbd(kmv, kr, data.keyboard)
    set_mask_regions_mouse(mmv, mr, data.mouse)

    data.controller_view = cmv
    data.keyboard_view = kmv
    data.mouse_view = mmv
    data.arrow_view = amv

    return data
}