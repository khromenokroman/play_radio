#include <random>
#include "client.hpp"

radio::client::Client::Client(const std::string &ip_server, uint32_t port, std::filesystem::path const &path) : socket(
        std::make_unique<boost::asio::ip::tcp::socket>(io_context)) {

    std::ifstream list_radio_ref(path);
    if (!list_radio_ref.is_open()) {
        throw std::runtime_error(::fmt::format("Can't open file {}", path.string()));
    }

    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::system::error_code ec;

    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(ip_server, std::to_string(port), ec);
    if (ec) {
        throw std::runtime_error(::fmt::format("Failed to resolve {}:{}. Error: {}\n", ip_server, port, ec.message()));
    }

    boost::asio::connect(*socket, endpoints, ec);
    if (ec) {
        throw std::runtime_error(
                ::fmt::format("Failed to connect to {}:{}, Error: {}\n", ip_server, port, ec.message()));
    }

    ::fmt::print("Connected to {}:{} successful\n", ip_server, port);
}

void radio::client::Client::send_message() {
    get_random_line(LIST_RADIO);
    boost::system::error_code ec;

    if (full_command.empty()) {
        throw std::runtime_error("Message is empty!");
    }
    if (full_command.back() != '\n') {
        throw std::runtime_error(::fmt::format("In message {} last symbol is not '\\n'", full_command));
    }

    boost::asio::write(*socket, boost::asio::buffer(full_command), ec);
    if (ec) {
        throw std::runtime_error(::fmt::format("Failed to send message. Error: {}", ec.message()));
    }

    socket->shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    if (ec) {
        throw std::runtime_error(::fmt::format("Failed to shutdown socket. Error: {}", ec.message()));
    }
}

void radio::client::Client::get_random_line(std::string const &filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error(::fmt::format("Can't open file {}", LIST_RADIO));
    }

    std::vector<std::string> lines;
    lines.reserve(20);
    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    if (!lines.empty()) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(lines.begin(), lines.end(), g);

        full_command += lines[0];
        full_command += '\n';
    }else{
        throw std::runtime_error(::fmt::format("File {} is empty", LIST_RADIO));
    }
}
