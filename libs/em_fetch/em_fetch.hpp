#pragma once

#include "../../libs/stb_libs/qsprintf.hpp"
#include "../../libs/datetime/datetime.hpp"
#include "../../libs/io/filesystem.hpp"

#include <emscripten/fetch.h>


namespace em_fetch
{
    namespace sb = stack_buffer;
    namespace mb = memory_buffer;
    namespace dt = datetime;

    using FetchReadCallback = void (*)(ByteView const&, void*);
    using FetchFailCallback = void (*)(void*);


    class FetchContext
    {
    public:

        cstr url = 0;
        cstr url_backup = 0;
        FetchReadCallback read_bytes;
        FetchFailCallback fetch_failed;

        void* user_data = 0;
    };


namespace internal
{
    using FetchAttr = emscripten_fetch_attr_t;
    using FetchResponse = emscripten_fetch_t;


    inline ByteView make_byte_view(FetchResponse* res)
    {
        ByteView bytes;
        bytes.data = (u8*)res->data;
        bytes.length = res->numBytes;

        return bytes;
    }


    class FetchRequest
    {
    public:
        StringView data_url;
        StringView data_url_backup;

        StackBuffer<u8, 512> url_buffer;

        FetchContext context;

        static FetchRequest* create(FetchContext const& ctx)
        {
            if (!ctx.url || !ctx.read_bytes || !ctx.user_data)
            {
                return 0;
            }

            auto req = mem::alloc<FetchRequest>(1, "fetch");
            auto cache_bust = (u32)dt::current_timestamp_i64();
            auto fmt = "%s?%u";

            auto& buffer = req->url_buffer;
            sb::reset_buffer(buffer);
            req->data_url = span::make_string_view(256, buffer);
            req->data_url_backup = span::make_string_view(256, buffer);

            span::sprintf(req->data_url, fmt, ctx.url, cache_bust);
            if (ctx.url_backup)
            {
                span::sprintf(req->data_url_backup, fmt, ctx.url_backup, cache_bust);
            }

            req->context = ctx;

            return req;
        }

        static void destroy(FetchRequest* req) { mem::free(req); }
    };


    inline void process_fetch_fail(FetchRequest* req)
    {
        auto& ctx = (*req).context;
        ctx.fetch_failed(ctx.user_data);

        FetchRequest::destroy(req);
    }


    inline void process_fetch_success(FetchRequest* req, ByteView const& bytes)
    {
        auto& ctx = (*req).context;
        ctx.read_bytes(bytes, ctx.user_data);

        FetchRequest::destroy(req);
    }


    inline void fetch_backup_fail(FetchResponse* res)
    {
        auto req = (FetchRequest*)(res->userData);

        process_fetch_fail(req);
        emscripten_fetch_close(res);
    }


    inline void fetch_backup_success(FetchResponse* res)
    {
        auto req = (FetchRequest*)(res->userData);
        auto status = res->status;

        if (status == 200)
        {
            auto bytes = make_byte_view(res);
            process_fetch_success(req, bytes);
        }
        else
        {
            process_fetch_fail(req);
        }

        emscripten_fetch_close(res);
    }


    inline void fetch_backup(FetchRequest* req)
    {
        auto url = span::to_cstr(req->data_url_backup);

        if (!url)
        {
            process_fetch_fail(req);
            return;
        }

        FetchAttr attr;
        emscripten_fetch_attr_init(&attr);
        stb::qsnprintf(attr.requestMethod, 4, "GET");
        attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
        attr.userData = (void*)req;
        attr.onsuccess = fetch_backup_success;
        attr.onerror = fetch_backup_fail;

        emscripten_fetch(&attr, url);
    }


    inline void fetch_fail(FetchResponse* res)
    {
        auto req = (FetchRequest*)(res->userData);

        emscripten_fetch_close(res);

        fetch_backup(req);
    }


    inline void fetch_success(FetchResponse* res)
    {
        auto req = (FetchRequest*)(res->userData);
        auto status = res->status;

        if (status == 200)
        {
            auto bytes = make_byte_view(res);
            process_fetch_success(req, bytes);
        }
        else
        {
            fetch_backup(req);
        }

        emscripten_fetch_close(res);
    }


    inline void fetch_start(FetchRequest* req)
    {
        auto url = span::to_cstr(req->data_url);

        FetchAttr attr;
        emscripten_fetch_attr_init(&attr);
        stb::qsnprintf(attr.requestMethod, 4, "GET");
        attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
        attr.userData = (void*)req;
        attr.onsuccess = fetch_success;
        attr.onerror = fetch_fail;

        emscripten_fetch(&attr, url);
    }
}


    inline void fetch_async(FetchContext const& ctx)
    {
        auto req = internal::FetchRequest::create(ctx);
        if (!req)
        {
            ctx.fetch_failed(ctx.user_data);
            return;
        }

        internal::fetch_start(req);
    }
}