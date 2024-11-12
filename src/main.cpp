#include "core/ping.hpp"
#include "utils/cli/cli.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        auto config = utils::cli::parse_arguments(argc, argv);
        
        core::Ping ping(config);
        const bool success = ping.execute();
        
        std::cout << utils::cli::format_results(config.target_ip, ping.get_statistics()) << std::endl;
        
        return success ? EXIT_SUCCESS : EXIT_FAILURE;

    } catch (const std::invalid_argument& e) {
        utils::cli::print_usage(argv[0]);
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}