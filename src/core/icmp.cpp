#include "core/icmp.hpp"
#include <netinet/ip_icmp.h>
#include <cstring>

namespace core {

namespace {
    constexpr uint8_t ECHO_TYPE = ICMP_ECHO;
    constexpr uint8_t REPLY_TYPE = ICMP_ECHOREPLY;
    constexpr uint8_t ECHO_CODE = 0;
}

ICMPPacket::ICMPPacket(uint16_t id, uint16_t sequence) 
    : packet_(ICMP_ECHO_SIZE) 
{
    auto* icmp = reinterpret_cast<icmphdr*>(packet_.data());
    std::memset(icmp, 0, sizeof(icmphdr));

    icmp->type = ECHO_TYPE;
    icmp->code = ECHO_CODE;
    icmp->un.echo.id = htons(id);
    icmp->un.echo.sequence = htons(sequence);

    auto* timestamp = reinterpret_cast<TimePoint::rep*>(
        packet_.data() + sizeof(icmphdr)
    );
    *timestamp = Clock::now().time_since_epoch().count();

    calculate_checksum();
}

void ICMPPacket::calculate_checksum() {
    auto* icmp = reinterpret_cast<icmphdr*>(packet_.data());
    icmp->checksum = 0;

    uint32_t sum = 0;
    const auto* data = reinterpret_cast<uint16_t*>(packet_.data());
    size_t words = packet_.size() / 2;

    while (words--) {
        sum += *data++;
    }

    if (packet_.size() & 1) {
        sum += *reinterpret_cast<const uint8_t*>(data);
    }

    sum = (sum & 0xFFFF) + (sum >> 16);
    sum += (sum >> 16);

    icmp->checksum = static_cast<uint16_t>(~sum);
}

bool verify_icmp_reply(
    const void* data, 
    size_t size, 
    uint16_t id, 
    uint16_t sequence
) {
    if (size < sizeof(iphdr) + sizeof(icmphdr)) {
        return false;
    }

    const auto* ip = reinterpret_cast<const iphdr*>(data);
    const auto* icmp = reinterpret_cast<const icmphdr*>(
        reinterpret_cast<const uint8_t*>(data) + (ip->ihl << 2)
    );

    return icmp->type == REPLY_TYPE &&
           icmp->code == ECHO_CODE &&
           icmp->un.echo.id == htons(id) &&
           icmp->un.echo.sequence == htons(sequence);
}

} // namespace core