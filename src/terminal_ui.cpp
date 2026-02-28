#include "ui/terminal_ui.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <limits>
#include <string>

namespace ui
{
    net::sock_config terminal_ui::config_menu()
    {
        net::sock_config cfg{};
        cfg.sock_addr.ip = "0.0.0.0";
        cfg.sock_addr.port = 8080;
        cfg.typ = net::Type::Sstream;

        uint16_t choice = 0;

        while (choice != 5)
        {
            std::cout << "--- Network Configuration ---\n";
            std::cout << "1. Set IP (current: " << cfg.sock_addr.ip << ")\n";
            std::cout << "2. Set Port (current: " << cfg.sock_addr.port << ")\n";
            std::cout << "3. Select Type (current: " << (cfg.typ == net::Type::Sstream ? "TCP" : "UDP") << ")\n";
            std::cout << "4. Reset. \n";
            std::cout << "5. Done. \n";

            get_input(choice);

            switch (choice)
            {
            case 1:
            {
                std::string user_in;
                std::cout << "Enter the IP: ";
                std::cin >> user_in;

                if (is_valid_ipv4(user_in))
                {
                    cfg.sock_addr.ip = user_in;
                }
                else
                {
                    std::cout << "Invalid format or ipv4 address. [ " + user_in + " ] \n";
                }
                break;
            }

            case 2:
            {
                std::cout << "Enter the Port: ";
                std::cin >> cfg.sock_addr.port;
                break;
            }

            case 3:
            {

                cfg.typ = terminal_ui::type_submenu();
                break;
            }

            case 4:
            {
                cfg = net::sock_config{};
                cfg.typ = net::Type::Sstream;
                break;
            }

            default:
            {

                std::cout << "Invalid choice. \n";
                break;
            }
            }
        }
        return cfg;
    }

    net::Type terminal_ui::type_submenu()
    {
        uint16_t sub_choice = 0;
        while (true)
        {
            std::cout << "1. TCP (Stream)\n2. UDP (Datagram)\n";
            get_input(sub_choice);

            switch (sub_choice)
            {
            case 1:
                return net::Type::Sstream;
            case 2:
                return net::Type::Datagram;
            default:
                std::cout << "Invalid selection. Try again.\n";
            }
        }
    }

    void terminal_ui::get_input(uint16_t &choice)
    {
        while (!(std::cin >> choice))
        {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}