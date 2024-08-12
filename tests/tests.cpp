#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>
#include "../src/client/client.hpp"
#include "../src/server/server.hpp"

#define PORT 1993
#define SERVER "127.0.0.1"

TEST(Server, create_sigint) {
    auto server = std::make_shared<radio::server::Server>(PORT);

    std::promise<void> started;
    auto started_fut = started.get_future();

    std::thread th1([&, server]() {
        started.set_value(); // Поток запущен и готов к приему сигналов
        ASSERT_NO_THROW(server->run());
    });

    // Ожидаем, пока поток полностью не будет готов принять сигналы
    started_fut.get();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    int ret = pthread_kill(th1.native_handle(), SIGINT);
    if (ret != 0) {
        ::fmt::print("Err: {}\n", strerror(errno));
    }
    th1.join();
}

TEST(Server, create_sigterm) {
    auto server = std::make_shared<radio::server::Server>(PORT);

    std::promise<void> started;
    auto started_fut = started.get_future();

    std::thread th1([&, server]() {
        started.set_value(); // Поток запущен и готов к приему сигналов
        ASSERT_NO_THROW(server->run());
    });

    // Ожидаем, пока поток полностью не будет готов принять сигналы
    started_fut.get();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    int ret = pthread_kill(th1.native_handle(), SIGTERM);
    if (ret != 0) {
        ::fmt::print("Err: {}\n", strerror(errno));
    }
    th1.join();
}

TEST(Server, connect) {
    auto server = std::make_shared<radio::server::Server>(PORT);

    std::promise<void> started;
    auto started_fut = started.get_future();

    std::thread th1([&, server]() {
        started.set_value(); // Поток запущен и готов к приему сигналов
        ASSERT_NO_THROW(server->run());
    });

    // Ожидаем, пока поток полностью не будет готов принять сигналы
    started_fut.get();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::system::error_code ec;

    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(SERVER, std::to_string(PORT), ec);
    if (ec) {
        throw std::runtime_error(::fmt::format("Failed to resolve {}:{}. Error: {}\n", SERVER, PORT, ec.message()));
    }
    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints, ec);
    if (ec) {
        throw std::runtime_error(
                ::fmt::format("Failed to connect to {}:{}, Error: {}\n", SERVER, PORT, ec.message()));
    }

    ASSERT_FALSE(ec) << "Error occurred when trying to connect: " << ec.message();
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    if (ec) {
        throw std::runtime_error(::fmt::format("Failed to shutdown socket. Error: {}", ec.message()));
    }

    int ret = pthread_kill(th1.native_handle(), SIGTERM);
    if (ret != 0) {
        ::fmt::print("Err: {}\n", strerror(errno));
    }
    th1.join();
}



/*class Server : public ::testing::Test {
protected:
    std::unique_ptr<::radio::server::Server> server;
    std::thread serverThread;

    void SetUp() override {
        serverThread = std::thread([&]() {
            server = std::make_unique<::radio::server::Server>(PORT);
            server->run();
        });
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

//    void TearDown() override {
//        if(server) {
//            server->stop();
//        }
//        if(serverThread.joinable()) {
//            serverThread.join();
//        }
//    }
};*/


//TEST_F(Server, connect_client) {

//    radio::client::Client client("127.0.0.1", 1993);
//    client.send_message("Hello server! My name is Roman. ^_^");

//    boost::asio::io_context io_context;
//    boost::asio::ip::tcp::resolver resolver(io_context);
//    boost::system::error_code ec;
//
//    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(SERVER, std::to_string(PORT), ec);
//    if (ec) {
//        throw std::runtime_error(::fmt::format("Failed to resolve {}:{}. Error: {}\n", SERVER, PORT, ec.message()));
//    }
//    boost::asio::ip::tcp::socket socket(io_context);
//    boost::asio::connect(socket, endpoints, ec);
//    if (ec) {
//        throw std::runtime_error(
//                ::fmt::format("Failed to connect to {}:{}, Error: {}\n", SERVER, PORT, ec.message()));
//    }
//
//    ASSERT_FALSE(ec) << "Error occurred when trying to connect: " << ec.message();
//    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
//    if (ec) {
//        throw std::runtime_error(::fmt::format("Failed to shutdown socket. Error: {}", ec.message()));
//    }
//}

/*TEST_F(Server, recive_message) {
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket(io_context);
    boost::system::error_code ec;
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(SERVER), PORT), ec);

    ASSERT_FALSE(ec) << "Error occurred when trying to connect: " << ec.message();
//    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::string message = "Test Message\n";
    boost::asio::write(socket, boost::asio::buffer(message));
//    std::this_thread::sleep_for(std::chrono::seconds(2));
    message = "Test Message";
    boost::asio::write(socket, boost::asio::buffer(message));
    ::fmt::print("total message: {}\n", server->m_total_message);
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    if (ec) {
        throw std::runtime_error(::fmt::format("Failed to shutdown socket. Error: {}", ec.message()));
    }
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//
//    server->stop();
//    th_server.join();
//    std::this_thread::sleep_for(std::chrono::seconds(10));
//    PORT++;
}*/

//TEST(Client, create_client) {
//    std::unique_ptr<::radio::client::Client> client;
//    ASSERT_THROW(client = std::make_unique<::radio::client::Client>(SERVER, PORT), std::exception);
//}

