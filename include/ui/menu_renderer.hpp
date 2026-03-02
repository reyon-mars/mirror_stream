#pragma once
#include "net/socket.hpp"
#include "net/socket_type.hpp"

namespace ui
{
    class menu_renderer
    {
    public:
        static void show_main_menu(const std::string &ip, uint16_t port, const std::string &type);
        static void show_type_menu();
    };
}