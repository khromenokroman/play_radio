#pragma once

#include <string>
#include <fmt/format.h>
#include <boost/asio.hpp>
#include <thread>
#include <boost/process.hpp>


namespace radio::server {
    class Server final {
    public:
        explicit Server(int port);

        /**
         * @brief Asynchronous accept handler for the server.
         *
         * This function is called whenever a new connection is accepted. It
         * handles errors during acceptance and reads the next line of input
         * from the socket asynchronously. When new input is read,
         * the function handles it by running a provided command in a
         * separate child process.
         *
         * @param error Error code from acceptance of the connection.
         * @param socket The socket associated with the new connection.
         * @param buffer The buffer to read input into.
         */
        void accept_handler(const boost::system::error_code &error,
                            const std::shared_ptr<boost::asio::ip::tcp::socket> &socket,
                            boost::asio::streambuf &buffer);

        /**
         * @brief Stops the server by stopping the boost::asio::io_context.
         *
         * After calling stop on the io_context, the function puts the
         * current thread to sleep for 1 second to allow for all
         * handlers to finish executing.
         */
        void stop();

        /**
         * @brief Run the server.
         *
         * This function checks if the ASIO thread is joinable, i.e.,
         * it has an associated active thread. If it is, the function waits for
         * the thread to finish its execution.
         */
        void run();

        /**
         * @brief Start accepting new TCP connections.
         *
         * This function creates a new shared socket using the current IO context.
         * It then initiates an asynchronous call to accept a new connection.
         * When a new connection is accepted, the callback function handle_accept is called.
         */
        void start_accepting();

        ~Server() = default;

        /**
         * @brief Fetches the count of messages processed by the server. (maybe unused)
         *
         * This function returns the total number of messages that have been processed by the server.
         *
         * @return The count of messages processed.
         */
        [[nodiscard]] ssize_t get_count_message() const;

    private:
        boost::asio::streambuf m_buffer; // 96
        boost::process::child m_child_process; // 32
        boost::asio::io_context m_io_context{}; // 16
        std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor; // 8
        std::thread m_asio_thread; // 8
        std::unique_ptr<boost::asio::signal_set> m_signals; // 8
        ssize_t m_total_message = 0; // 8
    };
}

