#pragma once

#include "common/PacketDispatcher.hpp"
#include "common/Connection.hpp"
#include "common/Logger.hpp"

namespace fus::net {
    class NetClient {
        public:
            NetClient();
            ~NetClient();

            NetClient(const NetClient&) = delete;
            NetClient& operator=(const NetClient&) = delete;

            void connect(const std::string& host, uint16_t port);

            void disconnect();

            bool isConnected() const;

            template<typename PacketDataType>
            void send(const PacketDataType& data)
            {
                if (!isConnected()) {
                    fus::logging::StandardLogger::warning("[Client] Cannot send data, not connected");
                    return;
                }
                fus::common::Message msg;
                msg.setId(PacketDataType::ID);
                msg.setBody(PacketDataType::serialize(data));
                _connection->sendMessage(msg);
            }

            template<typename PacketDataType>
            void onPacket(std::function<void(std::shared_ptr<Connection>, const PacketDataType&)> handler)
            {
                this->_dispatcher->registerHandler<PacketDataType>(handler);
            }

        private:

            bool _makeConnection(const std::string& host, uint16_t port);

            std::shared_ptr<Connection> _connection;
            std::unique_ptr<fus::common::PacketDispatcher> _dispatcher;

            std::thread _pollingThread;

            std::atomic<bool> _running = false;

            void run();
    };
}