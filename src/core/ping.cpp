#include "ping.hpp"
#include <unistd.h>
#include <thread>

namespace core {

Ping::Ping(const Config& config) 
   : config_(config)
   , id_(static_cast<uint16_t>(getpid() & 0xFFFF))
{
   auto result = socket_.init(config.target_ip);
   if (!result.success) {
       throw std::runtime_error(result.message);
   }
   result = socket_.set_timeout(RECV_TIMEOUT);
   if (!result.success) {
       throw std::runtime_error(result.message);
   }
}

bool Ping::execute() {
   const int max_failures = std::min(5, config_.packet_count / 2);
   int failures = 0;

   for (int i = 0; i < config_.packet_count && failures < max_failures; ++i) {
       if (!send_and_receive_ping()) {
           failures++;
           continue;
       }

       if (i < config_.packet_count - 1) {
           std::this_thread::sleep_for(config_.interval);
       }
   }

   return stats_.is_valid();
}

bool Ping::send_and_receive_ping() {
   ICMPPacket packet(id_, sequence_++);
   
   const auto start = Clock::now();
   
   if (!socket_.send_packet(packet.data(), packet.size()).success) {
       return false;
   }

   std::vector<uint8_t> recv_buffer(RECV_BUFFER_SIZE);
   if (!socket_.receive_packet(recv_buffer.data(), recv_buffer.size()).success) {
       return false;
   }

   if (!verify_icmp_reply(recv_buffer.data(), recv_buffer.size(), 
                       id_, sequence_ - 1)) {
       return false;
   }

   stats_.add_result(Clock::now() - start);
   return true;
}

const PingStats& Ping::get_statistics() const {
   return stats_;
}

} // namespace core