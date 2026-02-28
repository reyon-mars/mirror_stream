#pragma once
#include <arpa/inet.h>
#include <string>

inline bool is_valid_ipv4(const std::string &ip)
{
    sockaddr_in addr{};
    return inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr) == 1;
}