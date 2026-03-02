#pragma once
#include <string>
#include <cstdint>
#include <netinet/in.h>
namespace net
{
    struct socket_address
    {
        std::string ip{"0.0.0.0"};
        uint16_t port{8080};

        sockaddr_in to_sockaddr_in() const;
    };
}