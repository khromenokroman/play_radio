#include <iostream>
#include <algorithm>
#include "client.hpp"

#define SERVER "192.168.0.98"
#define LIST_RADIO "/etc/radio/radio_list"
#define PORT 1993

/**
 * @brief Helper function to check if the program is running as root.
 *
 * This function checks if the current user ID is 0, which corresponds to the root user.
 * If the program is running as root, an exception is thrown..
 * It is not generally recommended to run non-system-essential programs as root.
 * Doing so can lead to accidental system-wide changes.
 */
void check_if_not_root();

std::string server_selection();

uint32_t port_selection();

std::string trim(std::string const &s);

bool isValidIP(std::string ip);

int main() {
    try {
        std::string ip_srv = server_selection();
        uint32_t port = port_selection();

        ::fmt::print("Server: {}\nPort: {}", ip_srv, port);
        check_if_not_root();
        radio::client::Client client(ip_srv, port, LIST_RADIO);
//        client.send_message("cvlc http://kpradio.hostingradio.ru:8000/kaliningrad.radiokp128.mp3");
//        client.send_message("ls -l");
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
        ::fmt::print("Введите адрес сервера [192.168.0.98]: ");
        std::getline(std::cin, ip_server);
        ip_server = trim(ip_server);
        if (!isValidIP(ip_server) && !ip_server.empty()) {
            return SERVER;
        }
    } while (!isValidIP(ip_server) && !ip_server.empty());
    if (ip_server.empty()) {
        return SERVER;
    }
    return ip_server;
}

uint32_t port_selection() {
    std::string port_str;
    uint32_t port = 0;
    ::fmt::print("\033[2J\033[H");
    ::fmt::print("Введите порт сервера [1993]: ");
    std::getline(std::cin, port_str);
    port_str = trim(port_str);
    if (port_str.empty()) {
        return PORT;
    }
    try {
        port = std::stoul(port_str);
    } catch (std::invalid_argument &e) {
        return PORT;
    } catch (std::out_of_range &e) {
        return PORT;
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

bool isValidIP(std::string ip) {
    int partsCount = 0;
    std::string part;
    ip.push_back('.');
    for (char c: ip) {
        if (c == '.') {
            if (part.empty() || stoi(part) > 255 || stoi(part) < 0) {
                return false;
            }
            partsCount++;
            part.clear();
        } else if (!isdigit(c)) {
            return false;
        } else {
            part.push_back(c);
        }
    }
    return partsCount == 4;
}