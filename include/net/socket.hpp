#include <iostream>    // for std::cout
#include <cerrno>      // for errno
#include <cstring>     // for strerror
#include <unistd.h>    // for close
#include <arpa/inet.h> // for inet_pton
#include <span>
#include "net/net_except.hpp"

namespace net
{
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
        std::string ip;
        uint16_t port;

        sockaddr_in to_sockaddr_in() const;
    };

    struct sock_config
    {
        int sock_fd{-1};
        socket_address s_adr{};
        Domain domain{Domain::IPv4};
        Type typ{Type::Sstream};
        Protocol proto{Protocol::Default};
    };

    class Socket
    {
    private:
        sock_config cfg;

        Socket(int fd, Domain d, Type t, Protocol p);

    public:
        ~Socket();

        Socket(Domain d, Type t, Protocol p);

        Socket(const Socket &) = delete;
        Socket &operator=(const Socket &) = delete;

        Socket(Socket &&other) noexcept;
        Socket &operator=(Socket &&other) noexcept;

        void bind(const std::string &address, uint16_t port);
        void listen(int backlog = SOMAXCONN);
        [[nodiscard]] Socket accept();

        int send(std::span<const std::byte> data);
        int receive(std::span<std::byte> buffer);
    };
}