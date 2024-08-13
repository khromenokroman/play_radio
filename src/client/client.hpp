#pragma once

#include <string>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <boost/asio.hpp>
#include <filesystem>
#include <fstream>

/* default values */
#define SERVER "192.168.25.32"
#define LIST_RADIO "/etc/radio/radio_list"
#define PORT 1993
#define BUFFER_RADIO_LIST 20

namespace radio::client {
    class Client {
    public:
        Client(std::string const &ip_server, uint32_t port, std::filesystem::path const &path);

        ~Client() = default;

        /**
         * @brief Method to send a message to the server.
         * First it gets a random line from the radio list file and stores it in full_command.
         * Then it writes the full_command to the socket to be sent to the server.
         * Lastly, it shuts down the socket sending.
         * If any operation fails, an error message will be displayed.
         * @throw runtime_error If there is a failure when performing the operations.
         * The error message will contain details provided by the corresponding boost::asio operation.
         */
        void send_message();

    private:
        /**
         * @brief Method to get a random line from a file.
         * The method reads lines from a given file and stores them into a string vector.
         * It then shuffles the vector and sets the full_command to the first shuffled line with a newline character appended.
         * If the file does not exist or is empty, a runtime error will be thrown.
         * @param filename The name of the file to be read.
         * @throw runtime_error If the file can't be opened or is empty.
         * The error message includes the filename.
         */
        void get_random_line(std::string const &filename);

        std::string full_command = "cvlc "; // 32
        boost::asio::io_context io_context; // 16
        std::unique_ptr<boost::asio::ip::tcp::socket> socket; // 8
    };
}

