#pragma once

#include "common/Platform.hpp"
#include "common/Types.hpp"
#include "common/Message.hpp"
#include "common/Logger.hpp"

namespace fus::common {
    class PacketDispatcher {
        public:
            PacketDispatcher() = default;
            ~PacketDispatcher() = default;

            template<typename PacketDataType>
            void registerHandler(std::function<void(std::shared_ptr<net::Connection>, const PacketDataType&)> handler)
            {
                HandlerFunction wrapper = [handler](std::shared_ptr<net::Connection> conn, const Message& message) {
                    PacketDataType data;
                    message.read(data);
                    handler(conn, data);
                };
                _handlers[PacketDataType::ID] = wrapper;
            }

            void dispatch(std::shared_ptr<net::Connection> conn, const Message& message);
        private:
            using HandlerFunction = std::function<void(std::shared_ptr<net::Connection>, const Message&)>;

            std::unordered_map<fus::net::PacketID, HandlerFunction> _handlers;
    };
} // namespace fus::common