#include "client.hpp"

radio::client::Client::Client(const std::string &ip_server, int port) : socket(
        std::make_unique<boost::asio::ip::tcp::socket>(io_context)) {
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::system::error_code ec;

    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(ip_server, std::to_string(port), ec);
    if (ec) {
        throw std::runtime_error(::fmt::format("Failed to resolve {}:{}. Error: {}\n", ip_server, port, ec.message()));
    }

    boost::asio::connect(*socket, endpoints, ec);
    if (ec) {
        throw std::runtime_error(
                ::fmt::format("Failed to connect to {}:{}, Error: {}\n", ip_server, port, ec.message()));
    }

    ::fmt::print("Connected to {}:{} successful\n", ip_server, port);
}

void radio::client::Client::send_message(std::string_view const &message) const {
    boost::system::error_code ec;
    if (message.empty()) {
        throw std::runtime_error("Message is empty!");
    }
    std::string correct_message;
    if (message[message.size()] != '\n') {
        (correct_message += message) += '\n';
    } else {
        correct_message = message;
    }

    boost::asio::write(*socket, boost::asio::buffer(correct_message), ec);
    if (ec) {
        throw std::runtime_error(::fmt::format("Failed to send message. Error: {}", ec.message()));
    }

    socket->shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    if (ec) {
        throw std::runtime_error(::fmt::format("Failed to shutdown socket. Error: {}", ec.message()));
    }
}
