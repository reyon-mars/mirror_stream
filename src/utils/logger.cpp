#include "utils/logger.hpp"
#include "ui/color.hpp"
#include <chrono>
#include <iostream>

namespace utils
{
    std::mutex logger::log_mtx_;

    void logger::emit(std::string_view level, std::string_view msg, std::string_view color)
    {
        auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
        {
            std::lock_guard<std::mutex> lock(log_mtx_);
            std::cout << std::format("[{:%F %T}] {} {:<5}{} {}\n", now, color, level, ui::color::reset, msg);
        }
    }

    void logger::log(std::string_view msg)
    {
        emit("LOG", msg, ui::color::blue);
    }

    void logger::log_err(std::string_view err_msg)
    {
        emit("ERROR", err_msg, ui::color::red);
    }
    void logger::log_warn(std::string_view warn_msg)
    {
        emit("WARNING", warn_msg, ui::color::blue);
    }
}