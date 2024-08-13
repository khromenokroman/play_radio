#pragma once

#include <string>
#include <fmt/format.h>
#include <boost/asio.hpp>
#include <thread>


namespace radio::server {
    class Server {
    public:
        explicit Server(int port);

        void accept_handler(const boost::system::error_code &error,
                            const std::shared_ptr<boost::asio::ip::tcp::socket> &socket,
                            boost::asio::streambuf &buffer);

        void stop();

        void run();

        void start_accepting();

        ~Server() = default;

        [[nodiscard]]ssize_t get_count_message() const;

    private:
        boost::asio::streambuf m_buffer; // 96
        boost::asio::io_context m_io_context{}; // 16
        std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor; // 8
        std::thread m_asio_thread; // 8
        std::unique_ptr<boost::asio::signal_set> m_signals; // 8
        ssize_t m_total_message = 0; // 8
    };
}

