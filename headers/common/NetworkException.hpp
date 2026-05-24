#pragma once

#include "common/Platform.hpp"
#include "common/Logger.hpp"

namespace fus::exception {
    class Network : public std::runtime_error {
        public:
            explicit Network(const std::string& message = "")
                : std::runtime_error(message) {}
    };

    class Connection : public Network {
        public:
            explicit Connection(const std::string& message = "")
                : Network(message) {}
    };

    class ClientDisconnected : public Connection {
        public:
            explicit ClientDisconnected(const std::string& message = "")
                : Connection(message) {}
    };

    class Send : public ClientDisconnected {
        public:
            explicit Send(const std::string& message = "")
                : ClientDisconnected(message) {}
    };
}