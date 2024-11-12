#include "core/socket.hpp"
#include <arpa/inet.h>
#include <cerrno>

namespace core {

Error Socket::translate_errno() const {
    switch (errno) {
        case EPERM:
        case EACCES:
            return Error::PERMISSION_DENIED;
        case ENETUNREACH:
            return Error::NETWORK_UNREACHABLE;
        case EADDRNOTAVAIL:
            return Error::INVALID_ADDRESS;
        default:
            return Error::UNKNOWN;
    }
}

std::string Socket::error_to_string(Error error) {
    switch (error) {
        case Error::NONE:
            return "Success";
        case Error::PERMISSION_DENIED:
            return "Permission denied (try running as root)";
        case Error::NETWORK_UNREACHABLE:
            return "Network is unreachable";
        case Error::INVALID_ADDRESS:
            return "Invalid address";
        case Error::UNKNOWN:
            return "Unknown error";
        default:
            return "Unhandled error";
    }
}

Result<void> Socket::make_success() {
    return Result<void>{true, Error::NONE, ""};
}

Result<void> Socket::make_error(Error error) {
    return Result<void>{false, error, error_to_string(error)};
}

Result<void> Socket::init(const std::string& target_ip) {
    fd_ = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (fd_ < 0) {
        return make_error(translate_errno());
    }

    addr_.sin_family = AF_INET;
    if (inet_pton(AF_INET, target_ip.c_str(), &addr_.sin_addr) <= 0) {
        close(fd_);
        fd_ = -1;
        return make_error(Error::INVALID_ADDRESS);
    }

    return make_success();
}

Result<void> Socket::set_timeout(Nanoseconds timeout) {
    auto tv = utils::time::to_timeval(timeout);
    
    if (setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        return make_error(translate_errno());
    }
    return make_success();
}

Result<void> Socket::send_packet(const void* data, size_t size) {
    ssize_t sent = sendto(fd_, data, size, 0,
                         reinterpret_cast<struct sockaddr*>(&addr_),
                         sizeof(addr_));
    
    if (sent != static_cast<ssize_t>(size)) {
        return make_error(translate_errno());
    }
    return make_success();
}

Result<void> Socket::receive_packet(void* buffer, size_t size) {
    sockaddr_in from;
    socklen_t from_len = sizeof(from);

    ssize_t received = recvfrom(fd_, buffer, size, 0,
                               reinterpret_cast<struct sockaddr*>(&from),
                               &from_len);

    if (received <= 0) {
        return make_error(translate_errno());
    }
    return make_success();
}

} // namespace core