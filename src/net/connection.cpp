#include "net/socket.hpp"
#include "net/net_except.hpp"
#include "utils/utils.hpp"
#include <vector>

namespace net
{
    void echo(net::Socket &&client) noexcept
    {
        try
        {
            std::vector<std::byte> buffer(4096);
            ssize_t bytes_received{0};

            while ((bytes_received = client.receive(std::span(buffer))) > 0)
            {

                std::span<const std::byte> data(buffer.data(), static_cast<size_t>(bytes_received));
                utils::log(std::string(reinterpret_cast<const char *>(buffer.data()), data.size()));

                client.send(data);
            }
            utils::log("Client disconnected.");
        }
        catch (const net::net_except &e)
        {
            utils::log(std::string("Network error in client thread : ") + e.what());
        }
        catch (const std::exception &e)
        {
            utils::log(std::string("General error : ") + e.what());
        }
    }
}