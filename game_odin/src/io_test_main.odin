package io_test


import "core:fmt"
import "util"
import img "image_view"
import win "app_io/window"


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

window: win.Window


end_program :: proc()
{
    run_state = RunState.End
}


is_running :: proc() -> bool
{
    return run_state != RunState.End
}


create_window :: proc() -> bool
{
    window_dims := Vec2Du32 {
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    }

    game_dims := window_dims

    if (!win.create(&window, "ODIN IO Test", window_dims, game_dims))
    {
        return false;
    }

    return true
}


main_init :: proc() -> bool
{
    return create_window()
}


main_close :: proc()
{
    win.destroy(&window)
}


main_loop :: proc()
{

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

    run_state = RunState.Run
    fmt.println("Running =", run_state)

    image: img.ImageView;
    fmt.println("ImageView: ", image)

    main_loop()

    main_close()

    fmt.println("main() Exit OK")
}