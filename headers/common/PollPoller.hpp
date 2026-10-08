#pragma once

#include "common/IPoller.hpp"

namespace fus::net {

    class PollPoller : public IPoller {
        public:
            void add(fus::net::_fd fd, uint32_t events) override;

            void modify(fus::net::_fd fd, uint32_t events) override;

            void remove(fus::net::_fd fd) override;

            int wait(std::vector<Event>& out, int timeoutMs) override;

        private:
            std::unordered_map<fus::net::_fd, uint32_t> _registrations;
    };

} // namespace fus::net
