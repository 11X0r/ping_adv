#pragma once

#include "core/types.hpp"
#include <vector>

namespace core {

class PingStats {
public:
    void add_result(Nanoseconds latency);
    bool is_valid() const { return latencies_.size() >= 2; }
    size_t successful_count() const { return latencies_.size(); }
    Milliseconds average_latency_ms() const;
    Nanoseconds jitter() const;
    const std::vector<Nanoseconds>& latencies() const { return latencies_; }

private:
    std::vector<Nanoseconds> latencies_;
};

} // namespace core