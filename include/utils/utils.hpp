#pragma once
#include <arpa/inet.h>
#include <string>
#include <chrono>
#include <format>
namespace utils
{
    inline bool is_valid_ipv4(const std::string &ip)
    {
        sockaddr_in addr{};
        return inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr) == 1;
    }

    inline void log(const std::string &msg)
    {
        auto now = std::chrono::system_clock::now();
        std::cout << "[" << std::format("{:%F %T}", now) << "]" << msg << "\n";
    }
}
