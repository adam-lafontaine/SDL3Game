#include "../io/filesystem.hpp"
#include "../alloc_type/alloc_type.hpp"
#include "sdl_include.hpp"


#define ASSERT_FILESYSTEM
#define LOG_FILESYSTEM


#ifndef NDEBUG

#ifdef LOG_FILESYSTEM
#define filesystem_log(...) SDL_Log(__VA_ARGS__)
#else
#define filesystem_log(...)
#endif

#ifdef ASSERT_FILESYSTEM
#define filesystem_assert(condition) SDL_assert(condition)
#else
#define filesystem_assert(...)
#endif

#else

#define filesystem_log(...)
#define filesystem_assert(...)

#endif


namespace fs
{
    namespace mb = memory_buffer;
}


namespace fs
{
    u32 file_size(cstr file_path)
    {
        auto file = SDL_IOFromFile(file_path, "rb");
        if (!file)
        {
            filesystem_log("SDL_IOFromFile error (%s): %s", file_path, SDL_GetError());
            filesystem_assert("*** SDL_IOFromFile error ***" && false);
            return 0;
        }

        auto size = SDL_GetIOSize(file);
        if (size < 0)
        {
            filesystem_log("SDL_GetIOSize error (%s): %s", file_path, SDL_GetError());
            filesystem_assert("*** SDL_GetIOSize error ***" && false);
            return 0;
        }

        SDL_CloseIO(file);

        return (u32)size;
    }


    MemoryBuffer<u8> read_bytes(cstr file_path)
    {
        MemoryBuffer<u8> buffer;

        u64 size = 0;

        auto data = SDL_LoadFile(file_path, &size);
        if (!data)
        {
            filesystem_log("SDL_LoadFile() error (%s): %s", file_path, SDL_GetError());
            //filesystem_assert("*** SDL_LoadFile() error ***" && false);
            return buffer;
        }

        buffer.data_ = (u8*)data;
        buffer.capacity_ = (u32)size;
        buffer.ok = 1;

        mem::add_allocation(buffer.data_, buffer.size_, get_file_name(file_path));

        return buffer;
    }
}