#include "server.hpp"

radio::server::Server::Server(int port) : m_acceptor(
        std::make_unique<boost::asio::ip::tcp::acceptor>(m_io_context, boost::asio::ip::tcp::endpoint(
                boost::asio::ip::tcp::v4(), port))), m_signals(
        std::make_unique<boost::asio::signal_set>(m_io_context, SIGINT, SIGTERM)) {

    ::fmt::print("Server started in port {}\n", port);


    m_signals->async_wait(
            [this](boost::system::error_code ec, int signal_number) {
                if (!ec) {
                    switch (signal_number) {
                        case SIGINT:
                            ::fmt::print("Signal SIGINT {} -> stop()\n", signal_number);
                            break;
                        case SIGTERM:
                            ::fmt::print("Signal SIGTERM {} -> stop()\n", signal_number);
                            break;
                        default:
                            break;
                    }
                    stop();
                } else {
                    ::fmt::print(stderr, "Err: get signal SIGINT {}", ec.message());
                }

            });

    m_asio_thread = std::thread([this]() {
        m_io_context.run();
    });
    if (pthread_setname_np(m_asio_thread.native_handle(), "aio_context") != 0) {
        ::fmt::print(stderr, "Can not set name for thread aio context: {}", strerror(errno));
    }

    start_accepting();

}

void radio::server::Server::start_accepting() {
    std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(m_io_context);
    auto callback = [this, socket](boost::system::error_code const &error) {
        accept_handler(error, socket, m_buffer);
    };
    m_acceptor->async_accept(*socket, callback);
}

void radio::server::Server::run() {
    if (m_asio_thread.joinable()) {
        m_asio_thread.join();
    }
}

void radio::server::Server::stop() {
    m_io_context.stop();
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void radio::server::Server::accept_handler(const boost::system::error_code &error,
                                           const std::shared_ptr<boost::asio::ip::tcp::socket> &socket,
                                           boost::asio::streambuf &buffer) {
    ::fmt::print("find new connection, socket is {}\n", socket->native_handle());
    if (!error) {
        auto callback = [this, socket, &buffer](boost::system::error_code const &error, std::size_t bytes_transferred) {
            if (!error) {
                std::istream is(&buffer);
                std::string message;
                std::getline(is, message);
                ::fmt::print("Received from client: {}\n", message);
                if (m_child_process && m_child_process.running()) {
                    m_child_process.terminate();
                    m_child_process.wait();
                }
                auto future = std::async(std::launch::async, [this, message]() {
                    m_child_process = boost::process::child(message, boost::process::std_out > boost::process::null);
                    m_child_process.detach();
                });
                m_total_message++;
            } else {
                if (error == boost::asio::error::eof) {
                    ::fmt::print(stderr, "Connection closed by client\n");
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

ssize_t radio::server::Server::get_count_message() const {
    return m_total_message;
}