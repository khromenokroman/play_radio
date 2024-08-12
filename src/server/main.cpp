#include "server.hpp"

int main([[maybe_unused]]int arc, [[maybe_unused]]char **argv) {
    radio::server::Server server(1993);
    return 0;
}
