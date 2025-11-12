#include "datetime.hpp"

#include <chrono>


// TODO

namespace datetime
{
    u64 query_nanoseconds_u64()
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now).time_since_epoch().count();

        return (u64)ns;
    }

    u64 query_performance_counter_u64()
    {
        return query_nanoseconds_u64();
    }
}