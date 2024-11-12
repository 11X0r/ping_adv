#pragma once

#include "core/types.hpp"
#include <sys/time.h>
#include <time.h>

namespace utils::time {

using core::Nanoseconds;
using core::Microseconds;
using core::Seconds;

[[nodiscard]] timeval to_timeval(Nanoseconds duration);
[[nodiscard]] timespec to_timespec(Nanoseconds duration);

} // namespace utils::time
