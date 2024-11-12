#pragma once

#include "core/types.hpp"
#include "core/ping_stats.hpp"
#include <string_view>

namespace utils::cli {

core::Config parse_arguments(int argc, char* argv[]);
void print_usage(std::string_view program_name);
std::string format_results(const std::string& ip, const core::PingStats& stats);

} // namespace utils::cli