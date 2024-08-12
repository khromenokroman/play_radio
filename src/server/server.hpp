#pragma once

#include <string>
#include <fmt/format.h>
#include <boost/asio.hpp>


namespace radio::server {
    class Server {
    public:
        [[noreturn]] explicit Server(int port);

        ~Server() = default;
        boost::asio::io_context io_context;
    };
}

