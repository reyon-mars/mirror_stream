#include "net/socket.hpp"
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

    Socket::Socket(int fd, Domain d, Type t, Protocol p)
    {
        cfg.sock_fd = fd;
        cfg.domain = d;
        cfg.typ = t;
        cfg.proto = p;
    }

    Socket::~Socket()
    {
        if (cfg.sock_fd >= 0)
        {
            close(cfg.sock_fd);
        }
    }

    Socket::Socket(Domain d, Type t, Protocol p)
    {
        cfg.domain = d;
        cfg.typ = t;
        cfg.proto = p;

        cfg.sock_fd = socket(
            static_cast<int>(cfg.domain),
            static_cast<int>(cfg.typ),
            static_cast<int>(cfg.proto));

        if (cfg.sock_fd < 0)
        {
            throw net_except("Failed to create a socket.");
        }
    }

    Socket::Socket(Socket &&other) noexcept
    {
        cfg = other.cfg;
        other.cfg.sock_fd = -1;
    }

    Socket &Socket::operator=(Socket &&other) noexcept
    {
        if (this != &other)
        {
            if (cfg.sock_fd >= 0)
            {
                close(cfg.sock_fd);
            }
            cfg = other.cfg;
            other.cfg.sock_fd = -1;
        }
        return *this;
    }

    void Socket::bind(const std::string &address, uint16_t port)
    {
        cfg.s_adr.ip = address;
        cfg.s_adr.port = port;

        sockaddr_in addr = cfg.s_adr.to_sockaddr_in();

        if (::bind(cfg.sock_fd,
                   reinterpret_cast<sockaddr *>(&addr),
                   sizeof(addr)) < 0)
        {
            throw net_except(
                "Failed to bind to " + address + ":" +
                std::to_string(port) +
                " - " + std::string(strerror(errno)));
        }
    }

    void Socket::listen(int backlog)
    {
        if (cfg.sock_fd < 0)
        {
            throw net_except("Cannot listen: Socket uninitialized.");
        }

        if (::listen(cfg.sock_fd, backlog) < 0)
        {
            throw net_except(
                "Failed to listen: " +
                std::string(strerror(errno)));
        }

        std::cout << "Socket is now listening...\n";
    }

    Socket Socket::accept()
    {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_fd = ::accept(
            cfg.sock_fd,
            reinterpret_cast<sockaddr *>(&client_addr),
            &client_len);

        if (client_fd < 0)
        {
            throw net_except(
                "Accept failed: " +
                std::string(strerror(errno)));
        }

        return Socket(client_fd, cfg.domain, cfg.typ, cfg.proto);
    }

    int Socket::send(std::span<const std::byte> data)
    {
        size_t total_sent = 0;

        while (total_sent < data.size())
        {
            ssize_t sent = ::send(
                cfg.sock_fd,
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
                    " bytes: " +
                    std::string(strerror(errno)));
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
            cfg.sock_fd,
            buffer.data(),
            buffer.size(),
            0);

        if (bytes_read < 0)
        {
            if (errno == EINTR)
                return receive(buffer);

            throw net_except(
                "Receive failed: " +
                std::string(strerror(errno)));
        }

        return static_cast<int>(bytes_read);
    }
}