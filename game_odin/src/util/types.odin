package util


Vec2D_XY :: struct($T: typeid)
{
    x: T,
    y: T
}


Rect2D_XY :: struct($T: typeid)
{
    x_begin: T,
    x_end: T,
    y_begin: T,
    y_end: T
}


Vec2Du32 :: Vec2D_XY(u32)
Vec2Di32 :: Vec2D_XY(i32)
Vec2Df32 :: Vec2D_XY(f32)

Rect2Du32 :: Rect2D_XY(u32)