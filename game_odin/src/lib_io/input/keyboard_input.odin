package input


KeyboardKeys :: enum
{
    kbd_W,
    kbd_A,
    kbd_S,
    kbd_D,
    kbd_1,
    kbd_2,
    kbd_3,
    kbd_4,
    kbd_SPACE,
}


KeyboardInput :: [KeyboardKeys]ButtonState


@(private)
reset_keyboard_state :: proc(keyboard: ^KeyboardInput)
{
    for &key in keyboard
    {
        reset_button_state(&key)
    }
}


@(private)
copy_keyboard_state :: proc(src: KeyboardInput, dst: ^KeyboardInput)
{
    for s, id in src
    {
        copy_button_state(s, &dst[id])
    }
}