#+private
package game_io_test




Rect_WH :: struct
{
    x: u32,
    y: u32,
    w: u32,
    h: u32
}


ControllerId :: enum
{
    dpad_up,
    dpad_down,
    dpad_left,
    dpad_right,

    a,
    b,
    x,
    y,

    start,
    back,

    shoulder_left,
    shoulder_right,

    stick_left,
    stick_right,

    trigger_left,
    trigger_right,
}


@(rodata)
CTLR_MASK_RECT := [ControllerId]Rect_WH {
    .trigger_left  = { 17, 4, 19, 15 },
    .trigger_right = { 156, 4, 19, 15 },

    .shoulder_left  = { 17, 21, 19, 8 },
    .shoulder_right = { 156, 21, 19, 8 },

    .back  = { 74, 23, 16, 9 },
    .start = { 102, 23, 16, 9 },

    .dpad_up    = { 21, 32, 10, 17 },
    .dpad_down  = { 21, 60, 10, 17 },
    .dpad_left  = { 5, 50, 17, 10 },
    .dpad_right = { 32, 50, 17, 10 },

    .stick_left  = { 59, 44, 25, 25 },
    .stick_right = { 108, 44, 25, 25 },

    .a = { 158, 62, 15, 15 },
    .b = { 173, 47, 15, 15 },
    .x = { 143, 47, 15, 15 },
    .y = { 158, 32, 15, 15 }
}


KeyboardId :: enum
{
    n_1,
    n_2,
    n_3,
    n_4,

    w,
    a,
    s,
    d,

    space,
}

@(rodata)
KBD_MASK_RECT := [KeyboardId]Rect_WH {
    .n_1 = { 20, 2, 16, 16 },
    .n_2 = { 38, 2, 16, 16 },
    .n_3 = { 56, 2, 16, 16 },
    .n_4 = { 74, 2, 16, 16 },

    .w = { 47, 20, 16, 16 },
    .a = { 34, 38, 16, 16 },
    .s = { 52, 38, 16, 16 },
    .d = { 70, 38, 16, 16 },

    .space = { 83, 74, 106, 16 },
}


MouseId :: enum
{
    left,
    right,
    middle,

    pos_x,
    pos_y,
}

@(rodata)
MSE_MASK_RECT := [MouseId]Rect_WH {
    .left   = { 1, 1, 30, 30 },
    .right  = { 49, 1, 30, 30 },
    .middle = { 32, 1, 14, 30 },
    .pos_x  = { 6, 44, 68, 8 },
    .pos_y  = { 6, 56, 68, 8 },
}

