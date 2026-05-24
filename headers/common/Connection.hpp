#pragma once

#include "common/Platform.hpp"
#include "common/Types.hpp"
#include "common/Message.hpp"
#include "common/NetworkException.hpp"

namespace fus::net {
    class Connection {
        public:
            Connection(fus::net::_fd socket, const struct sockaddr_in &addr);
            ~Connection();

            Connection(const Connection&) = delete;
            Connection& operator=(const Connection&) = delete;

            fus::net::_fd socket() const;

            bool isConnected() const;

            void disconnect();

            bool pollRead() const;

            std::vector<fus::common::Message> receive();

            void sendMessage(const fus::common::Message& msg);

            const struct sockaddr_in& address() const;

        private:

            void _receiveData();

            fus::common::MessageHeader _parseMessageHeader();

            fus::net::_fd _socket;
            struct sockaddr_in _addr;
            bool _connected = true;
            std::vector<uint8_t> _recvBuffer;
    };
}