package memory_buffer

import "core:mem"



MemoryBuffer :: struct($T: typeid)
{
    data: []T,
    size: u32,

    ok: bool
}


Result :: enum
{
    OK,
    Fail,
    Overflow,
    NoElements
}


create_buffer :: proc(buffer: ^MemoryBuffer($T), capacity: u32) -> Result
{
    assert(capacity > 0, "*** NO CAPACITY SPECIFIED ***")
    assert(len(buffer.data) == 0, "*** DATA ALREADY ALLOCATED ***")

    if capacity == 0 || len(buffer.data) > 0
    {
        return .Fail
    }

    data, err := make([]T, capacity, context.allocator)
    if err != nil
    {
        return .Fail
    }

    buffer.data = data
    buffer.size = 0
    buffer.ok = true

    return .OK
}


destroy_buffer :: proc(buffer: ^MemoryBuffer($T))
{
    if len(buffer.data) > 0
    {
        delete(buffer.data)
    }
    
    buffer.size = 0
    buffer.ok = false
}


reset_buffer :: proc(buffer: ^MemoryBuffer($T))
{
    buffer.size = 0
}


zero_buffer :: proc(buffer: ^MemoryBuffer($T))
{
    mem.zero_slice(buffer.data)
}


push_elements :: proc(buffer: ^MemoryBuffer($T), n_elements: u32) -> ([]T, Result)
{
    assert(n_elements > 0, "*** NO ELEMENTS SPECIFIED ***")

    if n_elements == 0
    {
        return nil, .Overflow
    }

    cap := cast(u32)len(buffer.data)
    assert(cap > 0, "*** BUFFER NOT ALLOCATED ***")

    is_valid := cap > 0 && buffer.size < cap
    elements_available := (cap - buffer.size) >= n_elements

    if !is_valid || !elements_available
    {
        return nil, .NoElements
    }

    begin := buffer.size
    end := begin + n_elements

    data := buffer.data[begin:end]
    buffer.size = end

    return data, .OK
}


pop_elements :: proc(buffer: ^MemoryBuffer($T), n_elements: u32)
{
    if n_elements == 0
    {
        return
    }

    if n_elements > buffer.size
    {
        buffer.size = 0
    }
    else
    {
        buffer.size -= n_elements
    }
}