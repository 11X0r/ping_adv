#include "core/ping_stats.hpp"
#include <cstdlib>

namespace core {

void PingStats::add_result(Nanoseconds latency) {
    latencies_.push_back(latency);
}

Milliseconds PingStats::average_latency_ms() const {
    if (latencies_.empty()) {
        return Milliseconds(0);
    }

    auto sum = Nanoseconds::zero();
    for (const auto& latency : latencies_) {
        sum += latency;
    }

    return std::chrono::duration_cast<Milliseconds>(
        sum / latencies_.size()
    );
}

Nanoseconds PingStats::jitter() const {
    if (latencies_.size() < 2) {
        return Nanoseconds::zero();
    }

    auto sum = Nanoseconds::zero();
    for (const auto& latency : latencies_) {
        sum += latency;
    }
    auto mean = sum / latencies_.size();

    auto deviation_sum = Nanoseconds::zero();
    for (const auto& latency : latencies_) {
        deviation_sum += Nanoseconds(
            std::llabs((latency - mean).count())
        );
    }

    return deviation_sum / latencies_.size();
}

} // namespace core