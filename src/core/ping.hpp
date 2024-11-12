#pragma once

#include "core/types.hpp"
#include "core/socket.hpp"
#include "core/icmp.hpp"
#include "core/ping_stats.hpp"
#include <stdexcept>

namespace core {

class Ping {
public:
    explicit Ping(const Config& config);
    bool execute();
    const PingStats& get_statistics() const;

private:
    bool send_and_receive_ping();

    static constexpr Nanoseconds RECV_TIMEOUT{std::chrono::seconds(1)};
    static constexpr size_t RECV_BUFFER_SIZE = 1024;

    Config config_;
    Socket socket_;
    PingStats stats_;
    uint16_t sequence_{0};
    const uint16_t id_;
};

} // namespace core