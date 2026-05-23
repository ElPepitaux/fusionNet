#pragma once

#include "common/Platform.hpp"
#include "common/Types.hpp"

namespace fus::net {
    class Connection {
        public:
            Connection(fus::net::_fd socket, const struct sockaddr_in &addr);
            ~Connection();

            Connection(const Connection&) = delete;
            Connection& operator=(const Connection&) = delete;

            Connection(Connection&&) noexcept;
            Connection& operator=(Connection&&) noexcept;

            fus::net::_fd socket() const;

            bool isConnected() const;

            void disconnect();

            bool pollRead() const;

            // Message receive();

            // void send(const Message& msg);

            const struct sockaddr_in& address() const;

        private:
            fus::net::_fd _socket;
            struct sockaddr_in _addr;
            bool _connected = true;
    };
}