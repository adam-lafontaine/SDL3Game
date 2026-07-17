#+private
package input

import rl "vendor:raylib"

import "core:math"

//import "core:fmt"


/* keyboard */

@(private="file")
record_keyboard_key_input :: proc(kbd_old: KeyboardKeyInput, kbd_new: ^KeyboardKeyInput)
{
    bd :: proc(id: rl.KeyboardKey) -> b8 { return cast(b8)rl.IsKeyDown(id) }
    
    record_button_input(kbd_old[.kbd_W], &kbd_new[.kbd_W], bd(.W))
    record_button_input(kbd_old[.kbd_A], &kbd_new[.kbd_A], bd(.A))
    record_button_input(kbd_old[.kbd_S], &kbd_new[.kbd_S], bd(.S))
    record_button_input(kbd_old[.kbd_D], &kbd_new[.kbd_D], bd(.D))
    record_button_input(kbd_old[.kbd_1], &kbd_new[.kbd_1], bd(.KP_1) || bd(.ONE))
    record_button_input(kbd_old[.kbd_2], &kbd_new[.kbd_2], bd(.KP_2) || bd(.TWO))
    record_button_input(kbd_old[.kbd_3], &kbd_new[.kbd_3], bd(.KP_3) || bd(.THREE))
    record_button_input(kbd_old[.kbd_4], &kbd_new[.kbd_4], bd(.KP_4) || bd(.FOUR))
    record_button_input(kbd_old[.kbd_SPACE], &kbd_new[.kbd_SPACE], bd(.SPACE))
}


/* mouse */

@(private="file")
record_mouse_button_input :: proc(src: MouseButtonInput, dst: ^MouseButtonInput)
{
    bd :: proc(id: rl.MouseButton) -> b8 { return cast(b8)rl.IsMouseButtonDown(id) }
    
    record_button_input(src[.btn_left],   &dst[.btn_left], bd(.LEFT))
    record_button_input(src[.btn_right],  &dst[.btn_right], bd(.RIGHT))
    record_button_input(src[.btn_middle], &dst[.btn_middle], bd(.MIDDLE))
}


@(private="file")
record_mouse_position_input :: proc(mouse: ^MouseInput)
{
    pos := rl.GetMousePosition()

    mouse.window_pos.x = i32(pos.x)
    mouse.window_pos.y = i32(pos.y)
}


@(private="file")
record_mouse_wheel_input :: proc(mouse: ^MouseInput)
{
    vec := rl.GetMouseWheelMoveV()

    mouse.wheel.x = i32(vec.x)
    mouse.wheel.y = i32(vec.y) // up < 0
}


/* gamepad */

@(private="file")
record_gamepad_button_input :: proc(handle: i32, gpd_old: GamepadButtonInput, gpd_new: ^GamepadButtonInput)
{
    bd :: proc(h: i32, id: rl.GamepadButton) -> b8 { return cast(b8)rl.IsGamepadButtonDown(h, id) }

    record :: proc(o: GamepadButtonInput, n: ^GamepadButtonInput, h: i32, b: GamepadButtons, id: rl.GamepadButton)
    {
        record_button_input(o[b], &n[b], bd(h, id))
    }

    record(gpd_old, gpd_new, handle, .btn_dpad_up,    .LEFT_FACE_UP)
    record(gpd_old, gpd_new, handle, .btn_dpad_right, .LEFT_FACE_RIGHT)
    record(gpd_old, gpd_new, handle, .btn_dpad_down,  .LEFT_FACE_DOWN)
    record(gpd_old, gpd_new, handle, .btn_dpad_left,  .LEFT_FACE_LEFT)

    record(gpd_old, gpd_new, handle, .btn_north, .RIGHT_FACE_UP)
    record(gpd_old, gpd_new, handle, .btn_east,  .RIGHT_FACE_RIGHT)
    record(gpd_old, gpd_new, handle, .btn_south, .RIGHT_FACE_DOWN)
    record(gpd_old, gpd_new, handle, .btn_west,  .RIGHT_FACE_LEFT)

    record(gpd_old, gpd_new, handle, .btn_start, .MIDDLE_RIGHT)
    record(gpd_old, gpd_new, handle, .btn_back,  .MIDDLE_LEFT)
    // MIDDLE,               // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)

    record(gpd_old, gpd_new, handle, .btn_shoulder_left,  .LEFT_TRIGGER_1)
    record(gpd_old, gpd_new, handle, .btn_shoulder_right, .RIGHT_TRIGGER_1)

    // triggers? LEFT_TRIGGER_2, RIGHT_TRIGGER_2, 

    record(gpd_old, gpd_new, handle, .btn_stick_left,  .LEFT_THUMB)
    record(gpd_old, gpd_new, handle, .btn_stick_right, .RIGHT_THUMB)
}


@(private="file")
record_gamepad_axis_input :: proc(handle: i32, gpd: ^GamepadInput)
{
    to_f32 :: proc(val: b8) -> f32 { return val ? f32(1.0) : f32(0.0) }

    get_axis :: proc(h: i32, id: rl.GamepadAxis) -> f32
    {
        deadzone :: 0.2
        val := rl.GetGamepadAxisMovement(h, id)
        return math.abs(val) < deadzone ? 0 : val
    }
    
    gpd.trigger_left = get_axis(handle, .LEFT_TRIGGER)
    gpd.trigger_right = get_axis(handle, .RIGHT_TRIGGER)

    // buttons already recorded
    right := to_f32(gpd.buttons[.btn_dpad_right].is_down)
    left  := to_f32(gpd.buttons[.btn_dpad_left].is_down)
    up    := to_f32(gpd.buttons[.btn_dpad_up].is_down)
    down  := to_f32(gpd.buttons[.btn_dpad_down].is_down)

    x := right - left
    y := up - down    
    set_vector_state(&gpd.vec_dpad, x, y)

    x = get_axis(handle, .LEFT_X)
    y = get_axis(handle, .LEFT_Y)
    set_vector_state(&gpd.vec_stick_left, x, y)

    x = get_axis(handle, .RIGHT_X)
    y = get_axis(handle, .RIGHT_Y)
    set_vector_state(&gpd.vec_stick_right, x, y)
}


/* api for the api */

api_init :: proc(inputs: ^InputArray) -> bool
{
    prev := prev(inputs)
    curr := curr(inputs)

    N := cast(i32)MAX_GAMEPADS

    for i in 0..<N
    {
        handle := i
        prev.gamepads[i].handle = handle
        curr.gamepads[i].handle = handle

        //avail := rl.IsGamepadAvailable(i)
        //fmt.println("*** Gamepad: ", i, avail)
    }

    return true
}


api_close :: proc()
{
    //assert(false, "*** NOT IMPLEMENTED ***")
}


api_record_input :: proc(inputs: ^InputArray)
{
    prev := prev(inputs)
    curr := curr(inputs)
    
    record_keyboard_key_input(prev.keyboard.keys, &curr.keyboard.keys)
    record_mouse_button_input(prev.mouse.buttons, &curr.mouse.buttons)
    record_mouse_position_input(&curr.mouse)
    record_mouse_wheel_input(&curr.mouse)

    N := cast(i32)MAX_GAMEPADS

    for i in 0..<N
    {
        h := prev.gamepads[i].handle
        if rl.IsGamepadAvailable(h)
        {
            record_gamepad_button_input(h, prev.gamepads[i].buttons, &curr.gamepads[i].buttons)
            record_gamepad_axis_input(h, &curr.gamepads[i])
        }
    }

    if (rl.WindowShouldClose()) // ESC key
    {
        curr.cmd_end_program = true
    }
}
