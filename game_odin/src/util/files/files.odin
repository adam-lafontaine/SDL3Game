package files

import "core:os"
import "core:strings"
import mb "../memory_buffer"


ByteBuffer :: mb.MemoryBuffer(byte)


read_bytes :: proc(path: string) -> ByteBuffer
{
    buffer := ByteBuffer { ok = false, size = 0 }

    data, err := os.read_entire_file(path, context.allocator)
    if err == nil
    {
        buffer.data = data
        buffer.ok = true
    }

    return buffer
}
