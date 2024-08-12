#include "client.hpp"

radio::client::Client::Client(const std::string &ip_server, int port) : socket(std::make_unique<boost::asio::ip::tcp::socket>(io_context)) {
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::system::error_code ec;

    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(ip_server, std::to_string(port), ec);
    if (ec) {
        ::fmt::print("Failed to resolve {}:{}. Error: {}\n", ip_server, port, ec.message());
        return;
    }

    boost::asio::connect(*socket, endpoints, ec);
    if (ec) {
        ::fmt::print("Failed to connect to {}:{}, Error: {}\n", ip_server, port, ec.message());
        return;
    }

    ::fmt::print("Connected to {}:{}\n", ip_server, port);

    std::string message = "Hello server!\n";
    boost::asio::write(*socket, boost::asio::buffer(message), ec);
    if (ec) {
        ::fmt::print("Failed to send message. Error: {}\n", ec.message());
        return;
    }

    socket->shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    if (ec) {
        ::fmt::print("Failed to shutdown socket. Error: {}\n", ec.message());
        return;
    }
}
