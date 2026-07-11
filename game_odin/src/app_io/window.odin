package app_io

import "../util"
import rl "vendor:raylib"


Vec2Du32 :: util.Vec2Du32

Window :: struct 
{
    pixel_buffer: ^u32,
    width_px: u32,
    height_px: u32,

    handle: i32
}


Rotate :: enum
{
    None = 0,
    Clockwise_90,
    CounterClockwise_90
}


/* api */

create :: proc(window: ^Window, title: cstring, window_size: Vec2Du32, pixel_size: Vec2Du32) -> bool
{
    id := allocate_screen_memory()

    if (id < -1)
    {
        assert(false, "*** SCREEN NOT ALLOCATED ***")
        return false
    }

    screen := get_screen_by_id(id)
    if (screen == nil)
    {
        assert(false, "*** NO SCREEN ***")
        return false
    }

    if (!create_screen_memory(screen , title, window_size, pixel_size))
    {
        assert(false, "*** SCREEN MEMORY ***")
        return false
    }

    window.handle = cast(i32)id
    set_window_pixels(screen, window)
    resize_render_rect(screen)

    return true
}


create_rotated :: proc(window: ^Window, title: cstring, window_size: Vec2Du32, pixel_size: Vec2Du32, rotate: Rotate = Rotate.None) -> bool
{
    id := allocate_screen_memory()

    if (id < -1)
    {
        assert(false, "*** SCREEN NOT ALLOCATED ***")
        return false
    }

    screen := get_screen_by_id(id)
    if (screen == nil)
    {
        return false
    }

    if (!create_screen_memory(screen , title, window_size, pixel_size))
    {
        return false
    }

    window.handle = cast(i32)id
    set_window_pixels(screen, window)
    resize_render_rect(screen, rotate)

    return true
}


create_fullscreen :: proc(window: ^Window, title: cstring, pixel_size: Vec2Du32) -> bool
{
    id := allocate_screen_memory()

    if (id < -1)
    {
        assert(false, "*** SCREEN NOT ALLOCATED ***")
        return false
    }

    screen := get_screen_by_id(id)
    if (screen == nil)
    {
        return false
    }

    if (!create_screen_memory_fullscreen(screen , title, pixel_size))
    {
        return false
    }

    window.handle = cast(i32)id
    set_window_pixels(screen, window)
    resize_render_rect(screen)

    return true
}


create_fullscreen_rotated :: proc(window: ^Window, title: cstring, pixel_size: Vec2Du32, rotate: Rotate = Rotate.None) -> bool
{
    id := allocate_screen_memory()

    if (id < -1)
    {
        assert(false, "*** SCREEN NOT ALLOCATED ***")
        return false
    }

    screen := get_screen_by_id(id)
    if (screen == nil)
    {
        return false
    }

    if (!create_screen_memory_fullscreen(screen , title, pixel_size))
    {
        return false
    }

    window.handle = cast(i32)id
    set_window_pixels(screen, window)
    resize_render_rect(screen, rotate)

    return true
}


/*set_window_icon :: proc(window: ^Window)
{

}*/


destroy :: proc(window: ^Window)
{
    screen := get_screen(window)
    destroy_screen_memory(screen)
    window.handle = -1
}


resize_pixel_buffer :: proc(window: ^Window, width: u32, height: u32) -> bool
{
    screen := get_screen(window)
    pixels := screen.screen_pixels

    w := cast(i32)width
    h := cast(i32)height

    if (w == pixels.width && h == pixels.height)
    {
        return true
    }

    destroy_screen_texture(screen)
    if (!create_screen_texture(screen, width, height))
    {
        return false
    }

    set_window_pixels(screen, window)

    return true
}


render :: proc(window: ^Window, size_changed: b32 = false)
{
    screen := get_screen(window)

    if (size_changed)
    {
        resize_render_rect(screen)
    }

    render_screen_memory(screen)
}


render_rotated :: proc(window: ^Window, rotate: Rotate, size_changed: b32 = false)
{
    screen := get_screen(window)

    if (size_changed)
    {
        resize_render_rect(screen, rotate)
    }

    render_screen_memory(screen)
}


hide_mouse_cursor :: proc()
{
    rl.HideCursor()
}


show_mouse_cursor :: proc()
{
    rl.ShowCursor()
}





/* ========== PRIVATE ========== */

/* screen memory */

@(private="file")
ScreenMemory :: struct
{
    window_title: cstring,
    target: rl.RenderTexture2D,
    screen_pixels: rl.Image,
    render_rect: rl.Rectangle
}


@(private="file")
destroy_screen_texture :: proc(screen: ^ScreenMemory)
{
    rl.UnloadRenderTexture(screen.target)
    rl.UnloadImage(screen.screen_pixels)
}


@(private="file")
destroy_screen_memory :: proc(screen: ^ScreenMemory)
{
    destroy_screen_texture(screen)
    rl.CloseWindow()
}


@(private="file")
create_screen_window :: proc(screen: ^ScreenMemory, title: cstring, width: u32, height: u32) -> bool
{
    w := cast(i32)width
    h := cast(i32)height

    flags: rl.ConfigFlags
    flags = { .WINDOW_RESIZABLE, .VSYNC_HINT }

    rl.SetConfigFlags(flags)
    rl.InitWindow(w, h, title)

    screen.window_title = title

    ok: bool = rl.IsWindowReady()

    assert(ok, "*** create_screen_window() ***")

    return ok
}


@(private="file")
create_screen_window_fullscreen :: proc(screen: ^ScreenMemory, title: cstring) -> bool
{
    flags: rl.ConfigFlags
    flags = { .WINDOW_RESIZABLE, .VSYNC_HINT }

    rl.SetConfigFlags(flags)
    rl.InitWindow(0, 0, title)

    screen.window_title = title

    ok: bool = rl.IsWindowReady()

    assert(ok, "*** create_screen_window_fullscreen() ***")

    return ok
}


@(private="file")
create_screen_texture :: proc(screen: ^ScreenMemory, width: u32, height: u32) -> bool
{
    w := cast(i32)width
    h := cast(i32)height

    screen.target = rl.LoadRenderTexture(w, h)
    rl.SetTextureFilter(screen.target.texture, rl.TextureFilter.BILINEAR);

    ok: bool = rl.IsRenderTextureValid(screen.target)

    assert(ok, "*** create_screen_texture() ***")

    if (!ok)
    {
        return false;
    }

    screen.screen_pixels = rl.GenImageColor(w, h, rl.BLACK)

    tx := &screen.target.texture
    px := & screen.screen_pixels

    ok &= px.data != nil
    ok &= px.width == tx.width
    ok &= px.height == tx.height
    ok &= px.format == rl.PixelFormat.UNCOMPRESSED_R8G8B8A8

    assert(ok, "*** create_screen_texture() ***")

    return ok
}


@(private="file")
create_screen_memory :: proc(screen: ^ScreenMemory, title: cstring, window_size: Vec2Du32, pixel_size: Vec2Du32) -> bool
{
    ok := true;

    ok &= create_screen_window(screen, title, window_size.x, window_size.y)
    ok &= create_screen_texture(screen, pixel_size.x, pixel_size.y)

    if (!ok)
    {
        destroy_screen_memory(screen);
    }

    return ok
}


@(private="file")
create_screen_memory_fullscreen :: proc(screen: ^ScreenMemory, title: cstring, pixel_size: Vec2Du32) -> bool
{
    ok := true;

    ok &= create_screen_window_fullscreen(screen, title)
    ok &= create_screen_texture(screen, pixel_size.x, pixel_size.y)

    if (!ok)
    {
        destroy_screen_memory(screen);
    }

    return ok
}


/* stack data */

@(private="file")
ScreenID :: distinct int

@(private="file")
N_SCREEN_MEMORY :: ScreenID(2)

@(private="file")
screen_data := [N_SCREEN_MEMORY]ScreenMemory{}

@(private="file")
screen_data_id := ScreenID(0)


/* helpers */

@(private="file")
allocate_screen_memory :: proc() -> ScreenID
{
    if (screen_data_id >= N_SCREEN_MEMORY)
    {
        return -1
    }

    screen_data_id += 1

    return screen_data_id
}


@(private="file")
get_screen_by_id :: proc(id: ScreenID) -> ^ScreenMemory
{
    if (id < 0 || id >= N_SCREEN_MEMORY)
    {
        return nil
    }

    return &screen_data[id]
}


@(private="file")
get_screen :: proc(window: ^Window) -> ^ScreenMemory
{
    id := cast(ScreenID)window.handle
    return get_screen_by_id(id)
}


@(private="file")
set_window_pixels :: proc(screen: ^ScreenMemory, window: ^Window)
{
    pixels := screen.screen_pixels

    window.pixel_buffer = (^u32)(pixels.data)
    window.width_px = cast(u32)pixels.width
    window.height_px = cast(u32)pixels.height
}


@(private="file")
get_rotate_angle :: proc(r: Rotate) -> f32
{
    #partial switch r 
    {
        case Rotate.Clockwise_90: return 90.0
        case Rotate.CounterClockwise_90: return -90.0
    }

    return 0.0
}


resize_render_rect :: proc(screen: ^ScreenMemory, rotate: Rotate = Rotate.None)
{
    in_w := cast(f32)screen.screen_pixels.width
    in_h := cast(f32)screen.screen_pixels.height
    rot_w := in_w;
    rot_h := in_h;
    
    out_w := cast(f32)rl.GetScreenWidth()
    out_h := cast(f32)rl.GetScreenHeight()

    if (rotate == Rotate.Clockwise_90 || rotate == Rotate.CounterClockwise_90)
    {
        rot_w = in_h
        rot_h = in_w
    }

    scale_w := out_w / rot_w
    scale_h := out_h / rot_h
    scale := (scale_w < scale_h) ? scale_w : scale_h

    w := scale * in_w
    h := scale * in_h
    xb := (out_w - w) / 2
    yb := (out_h - h) / 2

    r := &screen.render_rect
    r.x = xb
    r.y = yb
    r.width = w
    r.height = h
}


render_screen_memory :: proc(screen: ^ScreenMemory, rotate: Rotate = Rotate.None)
{
    r_px : rl.Rectangle
    r_px.x = 0.0
    r_px.y = 0.0
    r_px.width = cast(f32)screen.screen_pixels.width
    r_px.height = cast(f32)screen.screen_pixels.height

    r_out := screen.render_rect

    origin : rl.Vector2
    origin.x = r_out.width / 2
    origin.y = r_out.height / 2

    tint := rl.WHITE

    angle := get_rotate_angle(rotate)

    rl.UpdateTexture(screen.target.texture, screen.screen_pixels.data)

    rl.BeginDrawing()

    rl.ClearBackground(rl.BLACK)

    rl.DrawTexturePro(
            screen.target.texture,
            r_px,
            screen.render_rect,
            origin,
            angle,
            tint
        )

    rl.EndDrawing()
}