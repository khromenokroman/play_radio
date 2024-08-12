#include "server.hpp"
#include <thread>

int main([[maybe_unused]]int arc, [[maybe_unused]]char **argv) {
    radio::server::Server server(1993);
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    server.stop();
    return 0;
}
