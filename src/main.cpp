#include "net/socket.hpp"
#include "net/net_except.hpp"
#include "ui/terminal_ui.hpp"
#include "utils/utils.hpp"
#include "net/connection.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <span>

int main()
{
    try
    {
        net::sock_config config = ui::terminal_ui::config_menu();

        net::Socket echo_server(config);

        echo_server.bind(config.sock_addr);
        echo_server.listen();

        std::cout << "Echo server listening on " << config.sock_addr.ip << " : " << config.sock_addr.port << "\n";

        while (true)
        {
            try
            {
                net::Socket client = echo_server.accept();
                utils::log("New client connected.");

                std::thread req_handler(net::echo, std::move(client));
                req_handler.detach();
            }
            catch (const net::net_except &e)
            {
                std::cerr << "Accept error: " << e.what() << "\n";
            }
        }
    }
    catch (const net::net_except &e)
    {
        std::cerr << "Server error: " << e.what() << "\n";
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 2;
    }
    return 0;
}