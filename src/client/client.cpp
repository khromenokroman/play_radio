#include "client.hpp"

radio::client::Client::Client(const std::string &ip_server, int port) {
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(ip_server, std::to_string(port));
    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);
    ::fmt::print("Connect to {}:{}", ip_server, port);

    std::string message = "Hello, Server! I am Client!\n";
    boost::asio::write(socket, boost::asio::buffer(message));
}
