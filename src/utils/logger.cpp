#include "utils/logger.hpp"
#include <chrono>
#include <iostream>

namespace utils
{
    std::mutex logger::log_mtx_;

    inline void logger::emit(std::string_view level, std::string_view msg, std::string_view color)
    {
        auto now = std::chrono::system_clock::now();
        {
            std::lock_guard<std::mutex> lock(log_mtx_);
            std::cout << std::format("[{:%F %T }] {} {:<7}{} {} \n", now, color, level, RESET, msg);
        }
    }

    inline void logger::log(std::string_view msg)
    {
        emit("LOG", msg, BLUE);
    }

    inline void logger::log_err(std::string_view err_msg)
    {
        emit("ERROR", err_msg, RED);
    }
    inline void logger::log_warn(std::string_view warn_msg)
    {
        emit("WARNING", warn_msg, YELLOW);
    }
}