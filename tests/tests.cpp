#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>
#include "../src/client/client.hpp"
#include "../src/server/server.hpp"

TEST(Test, create_server) {
    std::unique_ptr<::radio::server::Server> server;
    std::thread th_server([&](){
        server =std::make_unique<::radio::server::Server>(1993);
    });
    std::this_thread::sleep_for(std::chrono::seconds(2));

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket(io_context);
    boost::system::error_code ec;
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1993), ec);

    ASSERT_FALSE(ec) << "Error occurred when trying to connect: " << ec.message();
    std::string message = "Test Message\n";
    boost::asio::write(socket, boost::asio::buffer(message));

    std::this_thread::sleep_for(std::chrono::seconds(1));

    server->stop();
    th_server.join();
}

TEST(Test, create_client) {
    std::unique_ptr<::radio::client::Client> client;
    ASSERT_THROW(client = std::make_unique<::radio::client::Client>("127.0.0.1", 1993),std::exception);
}

