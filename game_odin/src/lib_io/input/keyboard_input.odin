package input


N_KEYBOARD_KEYS :: 9

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
}


@(private)
reset_keyboard_state :: proc(keyboard: ^KeyboardInput)
{
    for &key in keyboard.list
    {
        reset_button_state(&key)
    }
}


@(private)
copy_keyboard_state :: proc(src: KeyboardInput, dst: ^KeyboardInput)
{
    for i in 0..<N_KEYBOARD_KEYS
    {
        copy_button_state(src.list[i], &dst.list[i])
    }
}