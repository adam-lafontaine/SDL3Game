package span_view

import mb "../memory_buffer"


SpanView :: struct($T: typeid)
{
    data: []T,
    //length: u32
}


ByteView :: SpanView(byte)


make_view :: proc(buffer: mb.MemoryBuffer($T)) -> SpanView(T)
{
    return SpanView {
        data = buffer.data
    }
}


sub_view :: proc(buffer: mb.MemoryBuffer($T), offset: u32, length: u32) -> SpanView(T)
{
    assert(len(buffer.data) > 0, "*** BAD BUFFER ***")
    
    return SpanView(T) {
        data = buffer.data[offset:offset + length]
    }
}


/*sub_view :: proc(view: SpanView($T), offset: u32, length: u32) -> SpanView($T)
{
    return SpanView {
        data = view.data[offset:length]
    }
}*/