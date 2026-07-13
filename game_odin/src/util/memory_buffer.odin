package util

import "core:mem"


T :: u32 // !!! generics


MemoryBuffer :: struct
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


create_buffer :: proc(buffer: ^MemoryBuffer, n_elements: u32) -> Result
{
    assert(n_elements > 0, "*** NO ELEMENTS SPECIFIED ***")
    assert(len(buffer.data) == 0, "*** DATA ALREADY ALLOCATED ***")

    if n_elements == 0 || buffer.data == nil
    {
        return .Fail
    }

    data, err := make([]T, n_elements, context.allocator)
    if err != nil
    {
        return .Fail
    }

    buffer.data = data
    buffer.size = 0
    buffer.ok = true

    return .OK
}


destroy_buffer :: proc(buffer: ^MemoryBuffer)
{
    delete(buffer.data)
    buffer.size = 0
    buffer.ok = false
}


reset_buffer :: proc(buffer: ^MemoryBuffer)
{
    buffer.size = 0
}


zero_buffer :: proc(buffer: ^MemoryBuffer)
{
    mem.zero_slice(buffer.data)
}


push_elements :: proc(buffer: ^MemoryBuffer, n_elements: u32) -> (^T, Result)
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

    data := &buffer.data[buffer.size]
    buffer.size += n_elements

    return data, .OK
}


pop_elements :: proc(buffer: ^MemoryBuffer, n_elements: u32)
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