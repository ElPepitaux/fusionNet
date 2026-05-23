#include "server/NetServer.hpp"
#include <iostream>
#include "common/Datatype.hpp"

int main()
{
    fus::net::NetServer server;

    server.onConnect([](std::shared_ptr<fus::net::Connection> connection) {
        std::cout << "[Server INFO] New connection: " << connection->socket() << std::endl;
    });
    server.onDisconnect([](std::shared_ptr<fus::net::Connection> connection)  {
        std::cout << "[Server INFO] Connection closed: " << connection->socket() << std::endl;
    });
    server.onMessage([](std::shared_ptr<fus::net::Connection> connection, fus::common::Message& message) {
        std::cout << "[Server INFO] Received message from " << connection->socket() << ": " << message.id() << std::endl;
    });
    server.onPacket<example_packet_data>([](std::shared_ptr<fus::net::Connection> connection, const example_packet_data& data) {
        std::cout << "[Server INFO] Received example_packet_data with value: " << data.value << " from connection: " << connection->socket() << std::endl;
    });
    server.start(8080);

    while (server.isRunning()) {
        // Server is running, you can add additional logic here if needed
    }

    server.stop();
    return 0;
}