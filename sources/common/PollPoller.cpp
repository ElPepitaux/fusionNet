#include "common/PollPoller.hpp"

void fus::net::PollPoller::add(fus::net::_fd fd, uint32_t events)
{
    this->_registrations[fd] |= events;
}

void fus::net::PollPoller::modify(fus::net::_fd fd, uint32_t events)
{
    this->_registrations[fd] = events;
}

void fus::net::PollPoller::remove(fus::net::_fd fd)
{
    this->_registrations.erase(fd);
}

int fus::net::PollPoller::wait(std::vector<Event>& out, int timeoutMs)
{
    std::vector<struct pollfd> pollFds;
    pollFds.reserve(this->_registrations.size());

    for (const auto& [fd, events] : this->_registrations) {
        struct pollfd pfd = {};
        pfd.fd = fd;

        if (events & Readable)
            pfd.events |= POLLIN;
        if (events & Writable)
            pfd.events |= POLLOUT;

        pollFds.push_back(pfd);
    }

    int ret = poll(pollFds.data(), pollFds.size(), timeoutMs);
    if (ret <= 0)
        return ret;

    out.clear();
    out.reserve(static_cast<size_t>(ret));

    for (const auto& pfd : pollFds) {
        if (pfd.revents == 0)
            continue;

        Event event = {};
        event.fd = pfd.fd;

        if (pfd.revents & POLLIN)
            event.events |= Readable;
        if (pfd.revents & POLLOUT)
            event.events |= Writable;
        if (pfd.revents & (POLLERR | POLLNVAL))
            event.events |= Error;
        if (pfd.revents & POLLHUP)
            event.events |= Closed;

        out.push_back(event);
    }

    return static_cast<int>(out.size());
}
