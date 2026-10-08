#pragma once

#include "common/Platform.hpp"
#include "common/Types.hpp"

namespace fus::net {

    enum EventType : uint32_t {
        Readable = 1u << 0,
        Writable = 1u << 1,
        Error    = 1u << 2,
        Closed   = 1u << 3,
    };

    struct Event {
        fus::net::_fd fd;
        uint32_t events;
    };

    class IPoller {
        public:
            virtual ~IPoller() = default;

            virtual void add(fus::net::_fd fd, uint32_t events) = 0;

            virtual void modify(fus::net::_fd fd, uint32_t events) = 0;

            virtual void remove(fus::net::_fd fd) = 0;

            virtual int wait(std::vector<Event>& out, int timeoutMs) = 0;
    };

} // namespace fus::net
