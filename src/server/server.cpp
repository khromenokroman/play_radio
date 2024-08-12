#include "server.hpp"

radio::server::Server::Server(int port) : m_acceptor(
        std::make_unique<boost::asio::ip::tcp::acceptor>(m_io_context, boost::asio::ip::tcp::endpoint(
                boost::asio::ip::tcp::v4(), port))) {

    ::fmt::print("Server started in port {}\n", port);

    m_asio_thread = std::thread([&]() {
        m_io_context.run();
    });
    if (pthread_setname_np(m_asio_thread.native_handle(), "aio_context") != 0) {
        ::fmt::print(stderr, "Can not set name for thread aio context: {}", strerror(errno));
    }

    start_accepting();

}

radio::server::Server::~Server() {
    if (m_asio_thread.joinable()) {
        m_asio_thread.join();
    }
}

void radio::server::Server::start_accepting() {
    std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(m_io_context);
//    boost::asio::streambuf buffer;
    auto callback = [&](boost::system::error_code const &error) {
        accept_handler(error, socket, m_buffer);
    };
    m_acceptor->async_accept(*socket, callback);
}

void radio::server::Server::stop() {
    m_io_context.stop();
}

void radio::server::Server::accept_handler(const boost::system::error_code &error,
                                           const std::shared_ptr<boost::asio::ip::tcp::socket>& socket,
                                           boost::asio::streambuf &buffer) {
    if (!error) {
        auto callback = [this, socket, &buffer](boost::system::error_code const &error, std::size_t bytes_transferred) {
            if (!error) {
                std::istream is(&buffer);
                std::string message;
                std::getline(is, message);
                ::fmt::print("Received from client: {}\n", message);
            } else {
                if (error == boost::asio::error::eof) {
                    ::fmt::print("Connection closed by client\n");
                } else {
                    ::fmt::print(stderr, "Error occurred while receiving message: {}\n", error.message());
                }
            }
            buffer.consume(bytes_transferred);
            start_accepting();
        };
        boost::asio::async_read_until(*socket, buffer, "\n", callback);
    } else {
        ::fmt::print(stderr, "Error occurred while accepting a new connection: {}\n", error.message());
    }
}