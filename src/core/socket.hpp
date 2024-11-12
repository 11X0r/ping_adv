#pragma once

#include "core/types.hpp"
#include "utils/time/posix_time.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

namespace core {

class Socket {
public:
    ~Socket() {
        if (fd_ >= 0) {
            close(fd_);
        }
    }

    Result<void> init(const std::string& target_ip);
    Result<void> set_timeout(Nanoseconds timeout);
    Result<void> send_packet(const void* data, size_t size);
    Result<void> receive_packet(void* buffer, size_t size);

private:
    Error translate_errno() const;
    static std::string error_to_string(Error error);
    static Result<void> make_success();
    static Result<void> make_error(Error error);

    int fd_{-1};
    sockaddr_in addr_{};
};

} // namespace core