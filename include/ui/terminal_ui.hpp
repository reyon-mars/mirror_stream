#pragma once
#include "net/socket.hpp"

namespace ui
{
    class terminal_ui
    {
    public:
        static net::sock_config config_menu();
        static void get_input(uint16_t &choice);
        static net::Type type_submenu();
    };
}
