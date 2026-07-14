#+private
package game_io_test

import img "../image_view"


p32 :: img.Pixel32
GrayView :: img.GrayView
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


make_mask :: proc(img32: ImageView, buffer: ^Buffer8) -> GrayView
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