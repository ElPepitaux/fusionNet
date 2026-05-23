#pragma once

#include "server/Acceptor.hpp"
#include "server/EventLoop.hpp"
#include "common/PacketDispatcher.hpp"

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

            void onMessage(const MessageCallback& callback);

            template<typename PacketDataType>
            void onPacket(std::function<void(std::shared_ptr<Connection>, const PacketDataType&)> handler)
            {
                fus::logging::StandardLogger::info("[Server] Registering packet handler for type: " + std::string(typeid(PacketDataType).name()));
                this->_packetDispatcher->registerHandler<PacketDataType>(handler);
            }

        private:
            std::shared_ptr<Acceptor> _acceptor;
            std::shared_ptr<ManageConnection> _manageConnection;
            std::unique_ptr<EventLoop> _eventLoop;
            std::shared_ptr<fus::common::PacketDispatcher> _packetDispatcher;
    };
}