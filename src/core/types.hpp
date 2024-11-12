#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <system_error>

namespace core {

using Clock = std::chrono::steady_clock;
using Seconds = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;
using Nanoseconds = std::chrono::nanoseconds;
using TimePoint = Clock::time_point;

enum class Error {
    NONE,
    PERMISSION_DENIED,
    NETWORK_UNREACHABLE,
    INVALID_ADDRESS,
    TIMEOUT,
    UNKNOWN
};

template<typename T>
struct Result {
    bool success;
    T value;
    Error error;
    std::string message;
};

template<>
struct Result<void> {
    bool success;
    Error error;
    std::string message;
};

struct Config {
    std::string target_ip;
    int packet_count;
    Nanoseconds interval;

    static constexpr int MIN_PACKETS = 2;
    static constexpr int MAX_PACKETS = 50;
    static constexpr double MIN_INTERVAL_MS = 0.1;
    static constexpr double MAX_INTERVAL_MS = 10.0;

    Config(const std::string& ip, int count, double interval_ms)
        : target_ip(ip)
        , packet_count(count)
        , interval(std::chrono::nanoseconds(
              static_cast<int64_t>(interval_ms * 1'000'000)
          ))
    {
        if (packet_count < MIN_PACKETS || packet_count > MAX_PACKETS) {
            throw std::invalid_argument("Packet count must be between 2 and 50");
        }
        if (interval_ms < MIN_INTERVAL_MS || interval_ms > MAX_INTERVAL_MS) {
            throw std::invalid_argument("Interval must be between 0.1ms and 10ms");
        }
    }
};

} // namespace core