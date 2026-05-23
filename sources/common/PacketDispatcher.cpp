#include "common/PacketDispatcher.hpp"

void fus::common::PacketDispatcher::dispatch(std::shared_ptr<net::Connection> conn, const Message& message) {
    auto it = _handlers.find(message.id());
    if (it != _handlers.end()) {
        it->second(conn, message);
    } else {
        // Handle unknown packet ID if necessary
        fus::logging::StandardLogger::warning("Unknown packet ID: " + std::to_string(message.id()));
    }
}