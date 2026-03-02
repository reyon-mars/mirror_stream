#pragma once
#include "net/sock_config.hpp"

namespace ui
{
    class terminal_ui
    {
    private:
        static net::sock_config config_menu();
        static net::Type type_submenu();

    public:
        static void run(net::sock_config &cfg);
    };
}