#pragma once
#include <stdexcept>
#include <string>
#include <cstring>
#include <cerrno>

namespace net
{
    class net_except : public std::runtime_error
    {
    private:
        static std::string build_message(const std::string &msg)
        {
            return msg + " | error : " +
                   std::to_string(errno) +
                   " ( " + std::strerror(errno) + " ). \n";
        }

    public:
        explicit net_except(const std::string &msg)
            : std::runtime_error(build_message(msg))
        {
        }

        explicit net_except(const char *msg)
            : std::runtime_error(msg)
        {
        }
    };
}