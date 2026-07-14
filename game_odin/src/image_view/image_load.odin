package image_view

import "core:image/png"
import "core:image"
import "core:mem"
import "core:bytes"
import sv "../util/span_view"

// Avoids 'unused import' error: "core:image/png" needs to be imported in order
// to make `img.load_from_bytes` understand PNG format.
_ :: png

ByteView :: sv.ByteView


read_image_from_file :: proc() -> bool
{
    return false
}


read_image_from_memory :: proc(bv: ByteView, buffer: ^Buffer32, dst: ^ImageView) -> bool
{
    data, err := image.load_from_bytes(bv.data)
    if err != nil
    {
        return false
    }

    defer { image.destroy(data) }

    dst.width = cast(u32)data.width
    dst.height = cast(u32)data.height

    if !push_view(buffer, dst)
    {
        return false
    }

    src_bytes := bytes.buffer_to_bytes(&data.pixels)
    len := len(src_bytes)

    s := raw_data(src_bytes)
    d := raw_data(dst.data)
    
    mem.copy(d, s, len)

    return true
}