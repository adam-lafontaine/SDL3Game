#pragma once

#include "../span/span.hpp"


namespace fs
{
    u32 file_size(cstr file_path);

    MemoryBuffer<u8> read_bytes(cstr path);
}


namespace fs
{
    inline cstr get_file_name(cstr full_path)
    {
        auto str = span::to_string_view(full_path);
        auto c = str.data + str.length;

        for (; c >= str.data && *c != '/'; c--)
        { }

        //return (cstr)(c + 1); // no leading '/'
        return (cstr)c; // keep leading '/'
    }
}