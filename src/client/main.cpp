#include "client.hpp"

int main([[maybe_unused]]int arc, [[maybe_unused]]char **argv) {
    radio::client::Client client("127.0.0.1", 1993);
    return 0;
}