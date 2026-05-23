#include "server/NetServer.hpp"
#include <iostream>

int main()
{
    fus::net::NetServer server;

    server.onConnect([](std::shared_ptr<fus::net::Connection> connection) {
        std::cout << "[Server INFO] New connection: " << connection->socket() << std::endl;
    });
    server.onDisconnect([](std::shared_ptr<fus::net::Connection> connection)  {
        std::cout << "[Server INFO] Connection closed: " << connection->socket() << std::endl;
    });
    server.start(8080);

    while (server.isRunning()) {
        // Server is running, you can add additional logic here if needed
    }

    server.stop();
    return 0;
}