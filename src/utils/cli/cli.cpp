#include "utils/cli/cli.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace utils::cli {

void print_usage(std::string_view program_name) {
    std::cerr << "Usage: " << program_name 
              << " <target_ip> <packet_count> <interval_ms>\n\n"
              << "Parameters:\n"
              << "  target_ip    : IP address to ping\n"
              << "  packet_count : Number of packets (2-50)\n"
              << "  interval_ms  : Interval between packets (0.1-10ms)\n\n"
              << "Example:\n"
              << "  " << program_name << " 192.168.1.1 10 1.0\n";
}

core::Config parse_arguments(int argc, char* argv[]) {
    if (argc != 4) {
        throw std::invalid_argument("Invalid number of arguments");
    }

    try {
        return core::Config{
            argv[1],
            std::stoi(argv[2]),
            std::stod(argv[3])
        };
    } catch(const std::invalid_argument&) {
        throw std::invalid_argument("Invalid parameter value");
    }
}

std::string format_results(const std::string& ip, const core::PingStats& stats) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    if (!stats.is_valid()) {
        oss << "[" << ip << "] Test Failed";
        return oss.str();
    }

    auto avg_ms = stats.average_latency_ms();
    auto jitter_ns = stats.jitter();

    oss << "[" << ip << "] Test Result: "
        << "Average Latency " << avg_ms.count() << "ms, "
        << "Jitter " << jitter_ns.count() << "ns "
        << "(" << stats.successful_count() << " results)";

    return oss.str();
}

} // namespace utils::cli