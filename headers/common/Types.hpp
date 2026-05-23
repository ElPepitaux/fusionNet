#pragma once

#include "common/Platform.hpp"

namespace fus::net {
    using _fd = int;
    using PORT = uint16_t;
    const _fd NO_FD = -1;

    class Connection;

    using ConnectCallback = std::function<void(std::shared_ptr<Connection>)>;

    using DisconnectCallback = std::function<void(std::shared_ptr<Connection>)>;
}