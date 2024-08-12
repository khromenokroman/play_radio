#pragma once

#include <string>
#include <fmt/format.h>
#include <boost/asio.hpp>
#include <thread>


namespace radio::server {
    class Server {
    public:
        explicit Server(int port);

        void
        accept_handler(const boost::system::error_code &error,
                       const std::shared_ptr<boost::asio::ip::tcp::socket> &socket,
                       boost::asio::streambuf &buffer);

        void stop();

        void run();

        void start_accepting();

        ~Server();

        ssize_t m_total_message = 0;
    private:

        boost::asio::streambuf m_buffer;
        boost::asio::io_context m_io_context{}; // 16
        std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor; // 8
        std::thread m_asio_thread; // 8
        boost::asio::signal_set m_signals;
    };
}

