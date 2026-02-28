#include "net/socket.hpp"
#include <iostream>
#include <vector>
#include <span>
#include <string>
#include <cstddef>

int main()
{

    try
    {
        // Create a TCP IPv4 socket
        net::Socket server(net::Domain::IPv4, net::Type::Sstream, net::Protocol::TCP);

        // Bind to all interfaces on port 8080
        server.bind("0.0.0.0", 8080);

        // Listen for incoming connections
        server.listen();

        std::cout << "Waiting for a client to connect...\n";

        // Accept one connection
        net::Socket client = server.accept();
        std::cout << "Client connected!\n";

        // Receive and echo back data
        std::vector<std::byte> buffer(1024);
        int bytes_received = client.receive(buffer);
        std::string message(reinterpret_cast<char *>(buffer.data()), bytes_received);
        std::cout << "Received: " << message << "\n";

        // Echo back
        client.send(std::span<const std::byte>(buffer.data(), bytes_received));
        std::cout << "Echoed back to client\n";
    }
    catch (const net::net_except &e)
    {
        std::cerr << "Network error: " << e.what() << "\n";
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
