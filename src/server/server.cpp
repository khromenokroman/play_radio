#include "server.hpp"

[[noreturn]] radio::server::Server::Server(int port) {
    boost::asio::ip::tcp::acceptor acceptor(io_context,
                                            boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));

    ::fmt::print("Start server to port {}\n", port);
    for (;;) {
        boost::asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);

        boost::asio::streambuf buf;
        boost::asio::read_until(socket, buf, "\n");

        std::string message = boost::asio::buffer_cast<const char *>(buf.data());
        ::fmt::print("Received from client: {}\n", message);
    }
}
