#include "net/socket.hpp"
#include "utils/utils.hpp"
#include <vector>

namespace net
{
    void echo(net::Socket &&client)
    {
        std::vector<std::byte> buffer(4096);
        int bytes_received{0};

        while ((bytes_received = client.receive(std::span(buffer))) >= 0)
        {
            if (bytes_received == 0)
            {
                utils::log("Client disconnected.");
                return;
            }
            std::span<const std::byte> data(buffer.data(), static_cast<size_t>(bytes_received));
            utils::log(std::string(reinterpret_cast<const char *>(buffer.data()), data.size()));

            client.send(data);
        }
        utils::log("Socket error occurred during receive.");
    }
}