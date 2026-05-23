#pragma once

#include "common/Platform.hpp"

namespace fus::common {
    using Byte = uint8_t;
    using ID = uint16_t;
    class Message;
} // namespace fus::common
namespace fus::net {
    using _fd = int;
    using PORT = uint16_t;
    const _fd NO_FD = -1;
    using PacketID = uint16_t;

    class Connection;

    using ConnectCallback = std::function<void(std::shared_ptr<Connection>)>;

    using DisconnectCallback = std::function<void(std::shared_ptr<Connection>)>;

    using MessageCallback = std::function<void(std::shared_ptr<Connection>, fus::common::Message&)>;
}

