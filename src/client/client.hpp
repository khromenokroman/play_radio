#pragma once

#include <string>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <boost/asio.hpp>
#include <filesystem>
#include <fstream>


namespace radio::client {
    class Client {
    public:
        Client(std::string const &ip_server, uint32_t port, std::filesystem::path const& path);

        ~Client() = default;

        void send_message(std::string_view const &message) const;

        boost::asio::io_context io_context;
        std::unique_ptr<boost::asio::ip::tcp::socket> socket;
    };
}

