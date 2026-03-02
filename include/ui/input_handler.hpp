#pragma once
#include <string>
#include <cstdint>
#include <iostream>

namespace ui
{
    class input_handler
    {

    public:
        static uint16_t get_uint16();
        static std::string get_string();
    };
}