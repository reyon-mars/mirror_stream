#include <iostream>
#include <unistd.h>
#include "net/socket.hpp"
#include "net/net_except.hpp"
namespace net
{
    sockaddr_in socket_address::to_sockaddr_in() const
    {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        if (ip.empty() || ip == "0.0.0.0")
        {
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        else
        {
            int result = ::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
            if (result <= 0)
            {
                throw net_except("Invalid IP address format: " + ip);
            }
        }
        return addr;
    }

    Socket::Socket(int fd, const sock_config &cfg)
    {
        sock_cfg = cfg;
        sock_fd = fd;
    }

    Socket::~Socket()
    {
        if (sock_fd >= 0)
        {
            close(sock_fd);
        }
    }

    Socket::Socket(const sock_config &cfg)
    {
        sock_cfg = cfg;

        sock_fd = socket(
            static_cast<int>(sock_cfg.domain),
            static_cast<int>(sock_cfg.typ),
            static_cast<int>(sock_cfg.proto));

        if (sock_fd < 0)
        {
            throw net_except("Failed to create a socket.");
        }
    }

    Socket::Socket(Socket &&other) noexcept
    {
        sock_cfg = other.sock_cfg;
        sock_fd = other.sock_fd;
        other.sock_cfg = {};
        other.sock_fd = -1;
    }

    Socket &Socket::operator=(Socket &&other) noexcept
    {
        if (this != &other)
        {
            if (sock_fd >= 0)
            {
                close(sock_fd);
            }
            sock_cfg = std::move(other.sock_cfg);
            sock_fd = other.sock_fd;

            other.sock_cfg = {};
            other.sock_fd = -1;
        }
        return *this;
    }

    void Socket::bind(const socket_address &sock_addr)
    {
        sockaddr_in addr = sock_addr.to_sockaddr_in();

        if (::bind(sock_fd,
                   reinterpret_cast<sockaddr *>(&addr),
                   sizeof(addr)) < 0)
        {
            throw net_except(
                "Failed to bind to " + sock_addr.ip + ":" +
                std::to_string(sock_addr.port));
        }
    }

    void Socket::listen(int backlog)
    {
        if (sock_fd < 0)
        {
            throw net_except("Cannot listen: Socket uninitialized.");
        }

        if (::listen(sock_fd, backlog) < 0)
        {
            throw net_except("Failed to listen: ");
        }

        std::cout << "Socket is now listening...\n";
    }

    Socket Socket::accept()
    {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_fd = ::accept(
            sock_fd,
            reinterpret_cast<sockaddr *>(&client_addr),
            &client_len);

        if (client_fd < 0)
        {
            throw net_except("Accept failed: ");
        }

        return Socket(client_fd, sock_cfg);
    }

    int Socket::send(std::span<const std::byte> data)
    {
        size_t total_sent = 0;

        while (total_sent < data.size())
        {
            ssize_t sent = ::send(
                sock_fd,
                data.data() + total_sent,
                data.size() - total_sent,
                0);

            if (sent < 0)
            {
                if (errno == EINTR)
                    continue;

                throw net_except(
                    "Send failed after " +
                    std::to_string(total_sent) +
                    " bytes: ");
            }

            total_sent += sent;
        }

        return static_cast<int>(total_sent);
    }

    int Socket::receive(std::span<std::byte> buffer)
    {
        if (buffer.empty())
            return 0;

        ssize_t bytes_read = ::recv(
            sock_fd,
            buffer.data(),
            buffer.size(),
            0);

        if (bytes_read < 0)
        {
            if (errno == EINTR)
                return receive(buffer);

            throw net_except("Receive failed: ");
        }

        return static_cast<int>(bytes_read);
    }
}