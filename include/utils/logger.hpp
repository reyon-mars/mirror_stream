#pragma once
#include <string_view>
#include <mutex>

namespace utils
{
    constexpr std::string_view RESET = "\033[0m";
    constexpr std::string_view RED = "\033[31m";
    constexpr std::string_view YELLOW = "\033[33m";
    constexpr std::string_view BLUE = "\033[34m";
    class logger
    {
    private:
        static std::mutex log_mtx_;

        static void emit(std::string_view level, std::string_view msg, std::string_view color);

    public:
        static void log(std::string_view msg);
        static void log_err(std::string_view err_msg);
        static void log_warn(std::string_view warn_msg);
    };
}