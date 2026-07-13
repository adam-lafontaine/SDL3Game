package input


ButtonState :: struct
{
    pressed: b8,
    is_down: b8,
    raised: b8,
    
}


@(private)
reset_button_state :: proc(btn: ^ButtonState)
{
    btn.is_down = false
    btn.pressed = false
    btn.raised = false
}


@(private)
record_button_input :: proc(btn_old: ButtonState, btn_new: ^ButtonState, is_down: b8)
{
    btn_new.pressed = !btn_old.is_down && is_down
    btn_new.is_down = is_down
    btn_new.raised = btn_old.is_down && !is_down
}


@(private)
copy_button_state :: proc(src: ButtonState, dst: ^ButtonState)
{
    dst.is_down = src.is_down
    dst.pressed = false
    dst.raised = false
}