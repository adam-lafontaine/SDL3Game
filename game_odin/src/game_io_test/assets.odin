#+private
package game_io_test


import "../util"
import mb "../util/memory_buffer"
import sv "../util/span_view"
import img "../image_view"
import fs "../util/files"
import "../res"


BIN_DATA_PATH :: "./io_test_data.bin";
BIN_DATA_FALLBACK :: "/home/adam/Repos/SDL3Game/game_io_test/src/res/io_test_data.bin";


ByteBuffer :: mb.MemoryBuffer(byte)
ByteView :: sv.ByteView


AssetStatus :: enum 
{
    None,
    Load,
    Process,
    Ready,
    Fail
}


AssetMemory :: struct 
{
    image: struct
    {
        controller: ImageView,
        keyboard: ImageView,
        mouse: ImageView,
        arrow: ImageView
    },

    music: struct
    {
        A: ByteView,
        B: ByteView,
        C: ByteView,
        D: ByteView,
    },

    sound: struct
    {
        A: ByteView,
        B: ByteView,
        C: ByteView,
        D: ByteView,
    },

    pixels: img.Buffer32,
    bytes: ByteBuffer,

    status: AssetStatus
}


destroy_asset_memory :: proc(memory: ^AssetMemory)
{
    img.destroy_buffer32(&memory.pixels)
    mb.destroy_buffer(&memory.bytes)

    memory.status = .None
}


count_asset_pixels :: proc() -> u32
{
    w := res.masks[.keyboard].width
    h := res.masks[.keyboard].height
    count := w * h

    w = res.masks[.controller].width
    h = res.masks[.controller].height
    count += w * h

    w = res.masks[.mouse].width
    h = res.masks[.mouse].height
    count += w * h

    w = res.masks[.arrow].width
    h = res.masks[.arrow].height
    count += w * h

    return count
}


read_image :: proc(memory: ^AssetMemory, id: res.ImageID) -> bool
{
    pixels := &memory.pixels

    dst: ^ImageView = nil

    switch id
    {
    case .keyboard:   dst = &memory.image.keyboard
    case .controller: dst = &memory.image.controller
    case .mouse:      dst = &memory.image.mouse
    case .arrow:      dst = &memory.image.arrow
    case: return false
    }

    info := res.masks[id]

    bv := sv.sub_view(memory.bytes, info.offset, info.size)
    ok := img.read_image_from_memory(bv, pixels, dst)

    return ok
}


read_asset_memory :: proc(memory: ^AssetMemory) -> bool
{
    buffer := &memory.bytes
    if !buffer.ok
    {
        assert(false, "*** BAD BUFFER ***")
        return false
    }
    
    pixels := &memory.pixels
    n_pixels := count_asset_pixels()

    result := mb.create_buffer(pixels, n_pixels)
    if result != .OK
    {
        assert(false, "*** ASSET PIXELS ***")
        return false
    }

    ok := true
    
    for id in res.ImageID
    {
        ok &= read_image(memory, id)
    }

    assert(ok, "*** READ IMAGE ***")
    
    return ok
}


load_asset_memory :: proc(memory: ^AssetMemory) -> bool
{
    memory.status = .Load

    buffer := fs.read_bytes(BIN_DATA_PATH)
    if !buffer.ok
    {
        buffer = fs.read_bytes(BIN_DATA_FALLBACK)
    }

    if !buffer.ok
    {
        assert(false, "*** ASSET BUFFER ***")
        memory.status = .Fail
        return false
    }

    assert(len(buffer.data) > 0, "*** WAT? ***")

    memory.bytes = buffer
    
    ok := read_asset_memory(memory)
    assert(ok, "*** ASSET READ ***")

    memory.status = ok ? .Process : .Fail

    return ok
}