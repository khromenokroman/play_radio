#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>
#include "../src/client/client.hpp"
#include "../src/server/server.hpp"

#define PORT_TMP 1993
#define SERVER_TMP "127.0.0.1"

TEST(Server, create_sigint) {
    auto server = std::make_shared<radio::server::Server>(PORT_TMP);

    std::promise<void> started;
    auto started_fut = started.get_future();

    std::thread th1([&, server]() {
        started.set_value(); // Поток запущен и готов к приему сигналов
        ASSERT_NO_THROW(server->run());
    });

    // Ожидаем, пока поток полностью не будет готов принять сигналы
    started_fut.get();

    int ret = pthread_kill(th1.native_handle(), SIGINT);
    ASSERT_EQ(ret, 0) << ::fmt::format("Err: {}\n", strerror(errno));
    th1.join();
}

TEST(Server, create_sigterm) {
    auto server = std::make_shared<radio::server::Server>(PORT_TMP);

    std::promise<void> started;
    auto started_fut = started.get_future();

    std::thread th1([&, server]() {
        started.set_value(); // Поток запущен и готов к приему сигналов
        ASSERT_NO_THROW(server->run());
    });

    // Ожидаем, пока поток полностью не будет готов принять сигналы
    started_fut.get();

    int ret = pthread_kill(th1.native_handle(), SIGTERM);
    ASSERT_EQ(ret, 0) << ::fmt::format("Err: {}\n", strerror(errno));
    th1.join();
}

TEST(Server, connect) {
    auto server = std::make_shared<radio::server::Server>(PORT_TMP);

    std::promise<void> started;
    auto started_fut = started.get_future();

    std::thread th1([&, server]() {
        started.set_value(); // Поток запущен и готов к приему сигналов
        ASSERT_NO_THROW(server->run());
    });

    // Ожидаем, пока поток полностью не будет готов принять сигналы
    started_fut.get();

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::system::error_code ec;

    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(SERVER_TMP, std::to_string(PORT_TMP), ec);
    ASSERT_FALSE(ec) << ::fmt::format("Failed to resolve {}:{} - Error: {}\n", SERVER_TMP, PORT_TMP, ec.message());
    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints, ec);
    ASSERT_FALSE(ec) << ::fmt::format("Failed to connect to {}:{}, Error: {}\n", SERVER_TMP, PORT_TMP, ec.message());

    ASSERT_FALSE(ec) << "Error occurred when trying to connect: " << ec.message();
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    ASSERT_FALSE(ec) << "Failed to shutdown socket. Error: " << ec.message();

    int ret = pthread_kill(th1.native_handle(), SIGTERM);
    ASSERT_EQ(ret, 0) << ::fmt::format("Err: {}\n", strerror(errno));
    th1.join();
}

TEST(Server, recive_message) {
    auto server = std::make_shared<radio::server::Server>(PORT_TMP);

    std::promise<void> started;
    auto started_fut = started.get_future();

    std::thread th1([&, server]() {
        started.set_value(); // Поток запущен и готов к приему сигналов
        ASSERT_NO_THROW(server->run());
    });

    // Ожидаем, пока поток полностью не будет готов принять сигналы
    started_fut.get();

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::system::error_code ec;

    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(SERVER_TMP, std::to_string(PORT_TMP), ec);
    ASSERT_FALSE(ec) << ::fmt::format("Failed to resolve {}:{} - Error: {}\n", SERVER_TMP, PORT_TMP, ec.message());
    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints, ec);
    ASSERT_FALSE(ec) << ::fmt::format("Failed to connect to {}:{}, Error: {}\n", SERVER_TMP, PORT_TMP, ec.message());

    std::string message1 = "ls -lh\n";
    boost::asio::write(socket, boost::asio::buffer(message1));
    std::this_thread::sleep_for(std::chrono::seconds(4));
    ::fmt::print("get_count_message: {}\n", server->get_count_message());

    ASSERT_EQ(1,server->get_count_message());

    ASSERT_FALSE(ec) << "Error occurred when trying to connect: " << ec.message();
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    ASSERT_FALSE(ec) << "Failed to shutdown socket. Error: " << ec.message();

    int ret = pthread_kill(th1.native_handle(), SIGTERM);
    ASSERT_EQ(ret, 0) << ::fmt::format("Err: {}\n", strerror(errno));
    th1.join();
}