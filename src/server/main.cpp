#include "server.hpp"

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
    if (argc < 2) {
        ::fmt::print(stderr, "Can't found port server\nUsage: {} <port_number>\n", argv[0]);
        return 1;
    }

    check_if_not_root();

    int port = std::stoi(argv[1]);
    radio::server::Server server(port);
    server.run();
    return 0;
}

void check_if_not_root() {
    if (getuid() == 0) {
        throw std::runtime_error("The program should not be run as root");
    }
}