#+private
package game_io_test


import "../util"
import mb "../util/memory_buffer"
import sv "../util/span_view"
import img "../image_view"
import fs "../util/files"


BIN_DATA_PATH :: "./io_test_data.bin";
BIN_DATA_FALLBACK :: "/home/adam/Repos/SDL3Game/game_io_test/src/res/io_test_data.bin";


ByteBuffer :: mb.MemoryBuffer(byte)
ByteView :: sv.SpanView(byte)


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


read_asset_memory :: proc(memory: ^AssetMemory) -> bool
{
    return true
}


load_asset_memory :: proc(memory: ^AssetMemory)
{
    memory.status = .Load

    buffer := fs.read_bytes(BIN_DATA_PATH)
    if !buffer.ok
    {
        buffer = fs.read_bytes(BIN_DATA_FALLBACK)
    }

    if !buffer.ok
    {
        memory.status = .Fail
        return
    }

    memory.bytes = buffer
    
    ok := read_asset_memory(memory)

    memory.status = ok ? .Process : .Fail
}