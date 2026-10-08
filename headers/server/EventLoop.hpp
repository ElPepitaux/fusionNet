#pragma once

#include "common/Logger.hpp"
#include "common/IPoller.hpp"
#include "server/ManageConnection.hpp"
#include "server/Acceptor.hpp"
#include "common/PacketDispatcher.hpp"
#include "common/ThreadPool.hpp"

namespace fus::net {
    class EventLoop {
        public:
            EventLoop(const std::shared_ptr<Acceptor>& acceptor,
                    const std::shared_ptr<ManageConnection>& manageConnection,
                    const std::shared_ptr<fus::common::PacketDispatcher>& packetDispatcher);
            ~EventLoop();

            void start();

            void stop();

            bool isRunning() const;

            void onConnect(const ConnectCallback& callback);

            void onDisconnect(const DisconnectCallback& callback);

            void onMessage(const MessageCallback& callback);

        private:
            void run();

            void handleNewConnections(const std::vector<Event>& events);

            void handleClientEvents(const std::vector<Event>& events);

            std::shared_ptr<Acceptor> _acceptor;
            std::shared_ptr<ManageConnection> _manageConnection;
            std::shared_ptr<fus::common::PacketDispatcher> _packetDispatcher;

            bool _isRunning                                         = false;

            ConnectCallback _connectCallback                        = nullptr;
            DisconnectCallback _disconnectCallback                  = nullptr;
            MessageCallback _messageCallback                        = nullptr;

            std::unique_ptr<IPoller> _poller;
            std::unique_ptr<fus::common::ThreadPool> _threadPool;

            std::thread _thread;

    };
}