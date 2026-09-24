#pragma once

#include "common/Platform.hpp"
#include "common/Types.hpp"
#include "common/Message.hpp"
#include "common/Logger.hpp"

#include <type_traits>

namespace fus::common {

    template<typename T>
    concept PacketData =
    requires(const T& data, const std::vector<fus::common::Byte>& bytes)
    {
        requires std::same_as<decltype(T::ID), const uint16_t>;

        { T::serialize(data) } -> std::same_as<std::vector<fus::common::Byte>>;
        { T::deserialize(bytes) } -> std::same_as<T>;
    };

    class PacketDispatcher {
        public:
            PacketDispatcher() = default;
            ~PacketDispatcher() = default;

            template<PacketData PacketDataType>
            void registerHandler(std::function<void(std::shared_ptr<net::Connection>, const PacketDataType&)> handler)
            {
                HandlerFunction wrapper = [handler](std::shared_ptr<net::Connection> conn, const Message& message) {
                    PacketDataType data;
                    message.resetReadPos();
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