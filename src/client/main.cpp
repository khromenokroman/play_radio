#include "client.hpp"

/**
 * @brief Helper function to check if the program is running as root.
 *
 * This function checks if the current user ID is 0, which corresponds to the root user.
 * If the program is running as root, an exception is thrown..
 * It is not generally recommended to run non-system-essential programs as root.
 * Doing so can lead to accidental system-wide changes.
 */
void check_if_not_root();


int main(int argc, char **argv) {

    if (argc < 4) {
        ::fmt::print(stderr, "Can't found ip server/port or file for list radio\n"
                             "Usage: {} <ip server> <port_number> <file list radio>\n", argv[0]);
        return 1;
    }

    std::string ip_server = argv[1];
    try {
        check_if_not_root();

        radio::client::Client client(argv[1], std::stoi(argv[2]));
        client.send_message("ls -l");
    } catch (std::exception const &ex) {
        ::fmt::print(stderr, "Exception occurred: {}\n", ex.what());
        return -1;
    }
    return 0;
}

void check_if_not_root() {
    if (getuid() == 0) {
        throw std::runtime_error("The program should not be run as root");
    }
}