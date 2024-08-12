#pragma once

#include <string>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <boost/asio.hpp>


namespace radio::client {
    class Client {
    public:
        Client(std::string const &ip_server, int port);

        ~Client() = default;
        boost::asio::io_context io_context;
    };
}

