#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/client/client.hpp"
#include "../src/server/server.hpp"

TEST(Test, create_server) {
    std::unique_ptr<::radio::server::Server> server;
    ASSERT_NO_THROW(server = std::make_unique<::radio::server::Server>(1993));
}

TEST(Test, create_client) {
    std::unique_ptr<::radio::client::Client> client;
    ASSERT_THROW(client = std::make_unique<::radio::client::Client>("127.0.0.1", 1993),std::exception);
}

