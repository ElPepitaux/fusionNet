#pragma once

#include "server/Acceptor.hpp"
#include "server/EventLoop.hpp"

namespace fus::net {
    class NetServer {

        public:
            NetServer();
            ~NetServer();

            void start(fus::net::PORT port);
            void stop();

            bool isRunning() const;

            void onConnect(const ConnectCallback& callback);

            void onDisconnect(const DisconnectCallback& callback);

        private:
            std::shared_ptr<Acceptor> _acceptor;
            std::shared_ptr<ManageConnection> _manageConnection;
            std::unique_ptr<EventLoop> _eventLoop;

    };
}