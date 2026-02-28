#include "net/socket.hpp"
#include "net/net_except.hpp"
#include "ui/terminal_ui.hpp"
#include "utils/utils.hpp"
#include <iostream>
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
                std::cout << "Client connected. \n";

                std::vector<std::byte> buffer(4096);
                int bytes_received{0};

                while ((bytes_received = echo_server.receive(std::span(buffer))) > 0)
                {
                    std::span<const std::byte> data(buffer.data(), bytes_received);
                    client.send(data);
                }

                std::cout << "Client disconnected. \n";
            }
            catch (const net::net_except &e)
            {
                std::cerr << "Client error: " << e.what() << "\n";
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