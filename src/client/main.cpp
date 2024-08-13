#include <iostream>
#include <algorithm>
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

/**
 * @brief Function to select the server IP address.
 * If no valid IP-address is entered, the default IP-address is returned.
 * @return The IP address string.
*/
std::string server_selection();

/**
 * @brief Function to select the port for the server.
 * If no valid port is entered, the default port is returned.
 * @return The port number.
*/
uint32_t port_selection();

/**
 * @brief Trim leading and trailing whitespace from a string in C++.
 * @param s The string to be trimmed.
 * @return The trimmed string.
*/
std::string trim(std::string const &s);

/**
 * @brief Function to validate an IP address
 * @param ip The string containing the IP address.
 * @return True if IP is valid, false otherwise
*/
bool is_valid_ip(std::string ip);

int main() {
    try {
        std::string ip_srv = server_selection();
        uint32_t port = port_selection();

        /*::fmt::print("Server: {}\nPort: {}\n", ip_srv, port);*/
        check_if_not_root();
        radio::client::Client client(ip_srv, port, radio::client::Client::LIST_RADIO);
        client.send_message();
    } catch (std::exception const &ex) {
        ::fmt::print(stderr, "Exception occurred: {}\n", ex.what());
        return -1;
    }
    return 0;
}

std::string server_selection() {
    std::string ip_server{};
    ::fmt::print("\033[2J\033[H");
    do {
        ::fmt::print("Введите адрес сервера [{}]: ", radio::client::Client::SERVER);
        std::getline(std::cin, ip_server);
        ip_server = trim(ip_server);
        if (!is_valid_ip(ip_server) && !ip_server.empty()) {
            return radio::client::Client::SERVER;
        }
    } while (!is_valid_ip(ip_server) && !ip_server.empty());
    if (ip_server.empty()) {
        return radio::client::Client::SERVER;
    }
    return ip_server;
}

uint32_t port_selection() {
    std::string port_str;
    uint32_t port = 0;
    ::fmt::print("\033[2J\033[H");
    ::fmt::print("Введите порт сервера [{}]: ",radio::client::Client::PORT);
    std::getline(std::cin, port_str);
    port_str = trim(port_str);
    if (port_str.empty()) {
        return radio::client::Client::PORT;
    }
    try {
        port = std::stoul(port_str);
    } catch (std::invalid_argument &e) {
        return radio::client::Client::PORT;
    } catch (std::out_of_range &e) {
        return radio::client::Client::PORT;
    }
    return port;
}

void check_if_not_root() {
    if (getuid() == 0) {
        throw std::runtime_error("The program should not be run as root");
    }
}

std::string trim(const std::string &s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }

    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

bool is_valid_ip(std::string ip) {
    int parts_count = 0;
    std::string part;
    ip.push_back('.');
    for (char c: ip) {
        if (c == '.') {
            if (part.empty() || stoi(part) > 255 || stoi(part) < 0) {
                return false;
            }
            parts_count++;
            part.clear();
        } else if (!isdigit(c)) {
            return false;
        } else {
            part.push_back(c);
        }
    }
    return parts_count == 4;
}