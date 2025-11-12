#pragma once

#include "../util/types.hpp"

namespace datetime
{
    u64 query_nanoseconds_u64();

    u64 query_performance_counter_u64();

    i64 current_timestamp_i64();
}


namespace datetime
{
    class StopwatchNS
    {
    private:
        u64 start_ = 0;

        u64 now() { return query_nanoseconds_u64(); }


    public:
        StopwatchNS() { start_ = now(); }

        void start() { start_ = now(); }

        u64 get_time_nano() { return now() - start_; }

        f64 get_time_nano_f64() { return (f64)get_time_nano(); }

    };
}


namespace datetime
{
    class StopwatchPC
    {
    private:
        u64 start_ = 0;

        u64 now() { return query_performance_counter_u64(); }

    public:
        StopwatchPC() { start_ = now(); }

        void start() { start_ = now(); }

        u64 get_counter() { return now() - start_; }
    
    };
}