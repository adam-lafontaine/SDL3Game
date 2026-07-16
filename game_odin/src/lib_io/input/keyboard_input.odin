package input


/*N_KEYBOARD_KEYS :: 9

KeyboardKeyArray :: [N_KEYBOARD_KEYS]ButtonState

KeyboardKeys :: struct
{
    kbd_W: ButtonState,
    kbd_A: ButtonState,
    kbd_S: ButtonState,
    kbd_D: ButtonState,
    kbd_1: ButtonState,
    kbd_2: ButtonState,
    kbd_3: ButtonState,
    kbd_4: ButtonState,
    kbd_SPACE: ButtonState,
}


KeyboardInput :: struct #raw_union
{
    list: KeyboardKeyArray,
    keys: KeyboardKeys
}*/


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
    /*for &key in keyboard.list
    {
        reset_button_state(&key)
    }*/

    for &key in keyboard
    {
        reset_button_state(&key)
    }
}


@(private)
copy_keyboard_state :: proc(src: KeyboardInput, dst: ^KeyboardInput)
{
    /*for i in 0..<N_KEYBOARD_KEYS
    {
        copy_button_state(src.list[i], &dst.list[i])
    }*/

    for s, id in src
    {
        copy_button_state(s, &dst[id])
    }
}