#pragma once

#include "common/Platform.hpp"
#include "common/Types.hpp"
#include "common/Logger.hpp"
#include "common/Connection.hpp"

namespace fus::net {
    class Acceptor {
        public:
            Acceptor();
            ~Acceptor();

            void initialize(fus::net::PORT port);

            std::shared_ptr<fus::net::Connection> acceptClient();

            fus::net::_fd listenSocket() const;
        private:

            void _socket();

            void _bind(fus::net::PORT port);

            void _listen();

            _fd _listenSocket = fus::net::NO_FD;
            struct sockaddr_in _addrServer;
    };
}