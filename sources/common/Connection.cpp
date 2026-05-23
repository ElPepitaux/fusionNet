#include "common/Connection.hpp"

fus::net::Connection::Connection(fus::net::_fd socket, const struct sockaddr_in &addr) : _socket(socket), _addr(addr)
{
}

fus::net::Connection::~Connection()
{
    if (this->_connected)
        this->disconnect();
}

fus::net::Connection::Connection(Connection&& other) noexcept : _socket(other._socket), _addr(other._addr), _connected(other._connected)
{
    other._socket = fus::net::NO_FD;
    other._connected = false;
}

fus::net::Connection& fus::net::Connection::operator=(Connection&& other) noexcept
{
    if (this != &other) {
        if (this->_connected)
            this->disconnect();

        this->_socket = other._socket;
        this->_addr = other._addr;
        this->_connected = other._connected;

        other._socket = fus::net::NO_FD;
        other._connected = false;
    }
    return *this;
}

fus::net::_fd fus::net::Connection::socket() const
{
    return this->_socket;
}

bool fus::net::Connection::isConnected() const
{
    return this->_connected;
}

void fus::net::Connection::disconnect()
{
    if (this->_connected) {
        close(this->_socket);
        this->_socket = fus::net::NO_FD;
        this->_connected = false;
    }
}

bool fus::net::Connection::pollRead() const
{
    return true; // TODO: implement poll read
}

const struct sockaddr_in& fus::net::Connection::address() const
{
    return this->_addr;
}