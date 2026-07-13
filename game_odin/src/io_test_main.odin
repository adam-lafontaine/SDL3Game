package io_test


import "core:fmt"
import "core:time"
import "util"
import img "image_view"
import win "lib_io/window"
import inp "lib_io/input"
import game "game_io_test"


Vec2Du32 :: util.Vec2Du32
ImageView :: img.ImageView


RunState :: enum {
    Begin,
    Run,
    Error,
    End
}


WINDOW_WIDTH :: 800
WINDOW_HEIGHT :: 600

NANO :: 1_000_000_000.0

TARGET_FPS :: 60.0
TARGET_NS_PER_FRAME :: NANO / TARGET_FPS

run_state := RunState.End


main_window: win.Window
main_inputs: inp.InputArray
main_sw: time.Stopwatch
main_app_state: game.AppState

// references
window: ^win.Window = nil
inputs: ^inp.InputArray = nil
sw: ^time.Stopwatch = nil
app_state: ^game.AppState = nil


fake_app_update :: proc (input: inp.Input)
{
    kbd := input.keyboard.keys
    mouse := input.mouse.buttons.buttons
    pos := input.mouse.window_pos
    vec := input.mouse.wheel

    if (kbd.kbd_SPACE.pressed || mouse.btn_right.pressed)
    {
        win.dbg_toggle_color(window)
    }
    
}


end_program :: proc()
{
    run_state = .End
}


is_running :: proc() -> bool
{
    return run_state != .End
}


cap_framerate :: proc()
{
    fudge :: 0.9
    x := f64(TARGET_NS_PER_FRAME)
    target_ns := i64(x)

    time.stopwatch_stop(sw)

    ns := cast(i64)time.stopwatch_duration(sw^)
    if (ns < target_ns)
    {
        fns := f64(fudge) * f64(target_ns - ns)
        d := time.Duration(fns)
        time.sleep(d)
    }

    time.stopwatch_start(sw)
}


create_window :: proc(game_dims: Vec2Du32) -> bool
{
    window_dims := Vec2Du32 {
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    }

    if (!win.create(window, "ODIN IO Test", window_dims, game_dims))
    {
        return false;
    }

    return true
}


make_window_view :: proc() -> ImageView
{
    view: ImageView

    assert(false, "*** NOT IMPLEMENTED ***")

    return view
}


main_init :: proc() -> bool
{
    window = &main_window
    inputs = &main_inputs
    sw = &main_sw
    app_state = &main_app_state

    if !win.init(window)
    {
        return false;
    }
    
    if !inp.init(inputs)
    {
        return false
    }

    result := game.init(app_state)
    if !result.success
    {
        return false
    }

    if !create_window(result.screen_dimensions)
    {
        return false
    }

    if !game.set_screen_memory(app_state, make_window_view())
    {
        return false
    }

    return true
}


main_close :: proc()
{   
    game.close(app_state)
    inp.close(inputs)
    win.close(window)
}


main_loop :: proc()
{
    time.stopwatch_reset(sw)
    time.stopwatch_start(sw)

    for is_running() // for loop runs at least once
    {
        inp.record_input(inputs)
        input := inp.get(inputs)

        if (input.cmd_end_program)
        {
            end_program()
        }

        resize := cast(b32)input.window_size_changed

        fake_app_update(input)
        game.update(app_state, input)

        win.render(window, resize)

        inp.swap(inputs)
        cap_framerate()
    }
}


main :: proc()
{
    fmt.println("IO Test from ODIN")
    fmt.println("Target frame time = %f ns", TARGET_NS_PER_FRAME)

    if (!main_init())
    {
        fmt.println("Error: main_init()")
        return
    }

    run_state = .Run
    fmt.println("Running =", run_state)

    image: img.ImageView;
    fmt.println("ImageView: ", image)

    main_loop()

    main_close()

    fmt.println("main() Exit OK")
}