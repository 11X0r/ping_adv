#pragma once

#include "core/types.hpp"
#include <vector>

namespace core {

class ICMPPacket {
public:
    static constexpr size_t ICMP_ECHO_SIZE = 64;

    ICMPPacket(uint16_t id, uint16_t sequence);
    
    const uint8_t* data() const { return packet_.data(); }
    size_t size() const { return packet_.size(); }

private:
    std::vector<uint8_t> packet_;
    void calculate_checksum();
};

bool verify_icmp_reply(const void* data, size_t size, uint16_t id, uint16_t sequence);

} // namespace core