#pragma once

#include "common/Platform.hpp"
#include "common/Types.hpp"
#include "common/Connection.hpp"
#include "common/PacketDispatcher.hpp"


namespace fus::net {
    class ManageConnection {
        public:
            ManageConnection();
            ~ManageConnection();

            void addConnection(std::shared_ptr<Connection> connection);

            void removeConnection(const Connection& connection);
            void removeConnection(fus::net::_fd socket);

            std::vector<std::shared_ptr<Connection>> getConnections();

            std::shared_ptr<Connection> getConnection(fus::net::_fd socket);

        private:
            std::mutex _mutex;
            std::unordered_map<fus::net::_fd, std::shared_ptr<Connection>> _connections;
    };
}