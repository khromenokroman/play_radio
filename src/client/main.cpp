#include "client.hpp"

int main([[maybe_unused]]int arc, [[maybe_unused]]char **argv) {
    radio::client::Client client("127.0.0.1", 1993);
    client.send_message("Hello server! My name is Roman. ^_^");
    return 0;
}