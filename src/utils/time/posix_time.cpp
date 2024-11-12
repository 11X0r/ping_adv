#include "utils/time/posix_time.hpp"

namespace utils::time {

timeval to_timeval(Nanoseconds duration) {
    auto duration_us = std::chrono::duration_cast<Microseconds>(duration);
    timeval tv{};
    tv.tv_sec = duration_us.count() / 1'000'000;
    tv.tv_usec = duration_us.count() % 1'000'000;
    return tv;
}

timespec to_timespec(Nanoseconds duration) {
    auto secs = std::chrono::duration_cast<Seconds>(duration);
    auto remaining = duration - secs;
    
    timespec ts{};
    ts.tv_sec = secs.count();
    ts.tv_nsec = remaining.count();
    return ts;
}

} // namespace utils::time