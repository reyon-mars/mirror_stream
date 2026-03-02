#include "utils/utils.hpp"
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace utils
{
    inline bool is_valid_ipv4(const std::string &ip)

    {
        sockaddr_in addr{};
        return inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr) == 1;
    }
}
