#pragma once
#include <span>
#include <string>
#include <cstdint>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace net
{
    using sockfd_t = int;

    enum class Domain
    {
        IPv4 = AF_INET,
        IPv6 = AF_INET6,
        UNIX = AF_UNIX,
        UNSPECIFIED = AF_UNSPEC
    };

    enum class Type
    {
        Sstream = SOCK_STREAM,
        Datagram = SOCK_DGRAM,
        Raw = SOCK_RAW
    };

    enum class Protocol
    {
        Default = 0,
        TCP = IPPROTO_TCP,
        UDP = IPPROTO_UDP
    };

    struct socket_address
    {
        std::string ip{"0.0.0.0"};
        uint16_t port{8080};

        sockaddr_in to_sockaddr_in() const;
    };

    struct sock_config
    {
        Domain domain{Domain::IPv4};
        Type typ{Type::Sstream};
        Protocol proto{Protocol::Default};
        socket_address sock_addr{"0.0.0.0", 8080};
    };

    class Socket
    {
    private:
        sock_config sock_cfg{};
        sockfd_t sock_fd{-1};

        Socket(int fd, const sock_config &cfg);

    public:
        ~Socket();

        Socket(const sock_config &cfg);

        Socket(const Socket &) = delete;
        Socket &operator=(const Socket &) = delete;

        Socket(Socket &&other) noexcept;
        Socket &operator=(Socket &&other) noexcept;

        void bind(const socket_address &sock_addr);
        void listen(int backlog = SOMAXCONN);
        [[nodiscard]] Socket accept();

        int send(std::span<const std::byte> data);
        int receive(std::span<std::byte> buffer);
    };
}