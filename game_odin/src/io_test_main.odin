package io_test


import "core:fmt"
import "util"
import img "image_view"
import win "app_io/window"
import inp "app_io/input"


Vec2Du32 :: util.Vec2Du32


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

// references
window: ^win.Window = nil
inputs: ^inp.InputArray = nil


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
    assert(false, "*** NOT IMPLEMENTED ***")
}


create_window :: proc() -> bool
{
    window_dims := Vec2Du32 {
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    }

    game_dims := window_dims

    if (!win.create(window, "ODIN IO Test", window_dims, game_dims))
    {
        return false;
    }

    return true
}


main_init :: proc() -> bool
{
    window = &main_window
    if (!win.init(window))
    {
        return false;
    }

    inputs = &main_inputs
    if (!inp.init(inputs))
    {
        return false
    }

    return create_window()
}


main_close :: proc()
{   
    inp.close(inputs)
    win.destroy(window)
    win.close(window)
}


main_loop :: proc()
{
    for is_running() // for loop runs at least once
    {
        inp.record_input(inputs)
        input := inp.curr(inputs)
        if (input.cmd_end_program)
        {
            end_program()
        }

        resize := cast(b32)input.window_size_changed
        win.render(window, resize)

        inp.swap(inputs)
        //cap_framerate()
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