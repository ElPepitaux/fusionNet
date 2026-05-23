#pragma once

#include "common/Logger.hpp"
#include "server/ManageConnection.hpp"
#include "server/Acceptor.hpp"

namespace fus::net {
    class EventLoop {
        public:
            EventLoop(const std::shared_ptr<Acceptor>& acceptor, const std::shared_ptr<ManageConnection>& manageConnection);
            ~EventLoop();

            void start();

            void stop();

            bool isRunning() const;

            void onConnect(const ConnectCallback& callback);

            void onDisconnect(const DisconnectCallback& callback);

        private:
            void run();

            void buildPollFds();

            void handleNewConnections();

            void handleClientEvents();

            std::shared_ptr<Acceptor> _acceptor;
            std::shared_ptr<ManageConnection> _manageConnection;

            bool _isRunning                                         = false;

            ConnectCallback _connectCallback                        = nullptr;
            DisconnectCallback _disconnectCallback                  = nullptr;

            std::vector<struct pollfd> _pollFds;

            std::thread _thread;

    };
}