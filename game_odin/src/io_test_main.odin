package io_test


import "core:fmt"
import img "image_view"
import io "app_io"


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

window: io.Window


end_program :: proc()
{
    run_state = RunState.End
}


is_running :: proc() -> bool
{
    return run_state != RunState.End
}


main_init :: proc() -> bool
{
    return true
}


main_close :: proc()
{

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

    //io.hide_mouse_cursor()
    //io.show_mouse_cursor()

    main_loop()

    main_close()

    fmt.println("main() Exit OK")
}