#include <client/NetClient.hpp>
#include "common/Datatype.hpp"

int main() {
    fus::net::NetClient client;

    client.onPacket<example_packet_data>([](std::shared_ptr<fus::net::Connection> conn, const example_packet_data& data) {
        fus::logging::StandardLogger::info("[Client] Received packet with value: " + std::to_string(data.value));
    });

    client.connect("127.0.0.1", 8080);

    client.send<example_packet_data>(example_packet_data{42});

    return 0;
}