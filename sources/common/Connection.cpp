#include "common/Connection.hpp"

fus::net::Connection::Connection(fus::net::_fd socket, const struct sockaddr_in &addr) : _socket(socket), _addr(addr)
{
}

fus::net::Connection::~Connection()
{
    if (this->_connected)
        this->disconnect();
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
    if (!this->_connected)
        return false;

    struct pollfd pfd;
    pfd.fd = this->_socket;
    pfd.events = POLLIN;

    int ret = poll(&pfd, 1, 0);
    if (ret < 0) {
        fus::logging::StandardLogger::error("[Connection] Poll error: " + std::string(strerror(errno)));
        return false;
    }
    return (pfd.revents & POLLIN) != 0;
}

std::vector<fus::common::Message> fus::net::Connection::receive()
{
    std::vector<fus::common::Message> messages;
    this->_receiveData();
    while (true) {
        auto header = this->_parseMessageHeader();
        if (header.length == 0 || this->_recvBuffer.size() < header.length)
            break; // Not enough data for a complete message

        fus::common::Message msg;
        msg.setId(header.id);
        msg.body().assign(this->_recvBuffer.begin(), this->_recvBuffer.begin() + header.length);
        messages.push_back(std::move(msg));

        this->_recvBuffer.erase(this->_recvBuffer.begin(), this->_recvBuffer.begin() + header.length);
    }
    return messages;
}

void fus::net::Connection::sendMessage(const fus::common::Message& msg)
{
    if (!this->_connected)
        return;

    fus::common::MessageHeader header;
    header.id = msg.id();
    header.length = msg.body().size();

    std::vector<uint8_t> buffer(sizeof(header) + msg.body().size());
    std::memcpy(buffer.data(), &header, sizeof(header));
    std::memcpy(buffer.data() + sizeof(header), msg.body().data(), msg.body().size());

    ssize_t bytesSent = send(this->_socket, buffer.data(), buffer.size(), 0);
    if (bytesSent < 0) {
        fus::logging::StandardLogger::error("[Connection] Send error: " + std::string(strerror(errno)));
        this->disconnect();
    }
}

const struct sockaddr_in& fus::net::Connection::address() const
{
    return this->_addr;
}

void fus::net::Connection::_receiveData()
{
    do {
        uint8_t buffer[1024] = {0};
        ssize_t bytesRead = recv(this->_socket, buffer, sizeof(buffer), 0);
        if (bytesRead < 0) {
            if (errno != EWOULDBLOCK && errno != EAGAIN) {
                fus::logging::StandardLogger::error("[Connection] Receive error: " + std::string(strerror(errno)));
                this->disconnect();
            }
            break;
        } else {
            this->_recvBuffer.insert(this->_recvBuffer.end(), buffer, buffer + bytesRead);
        }
    } while (true);
}

fus::common::MessageHeader fus::net::Connection::_parseMessageHeader()
{
    if (this->_recvBuffer.size() < sizeof(fus::common::MessageHeader))
        return fus::common::MessageHeader{}; // Not enough data for header

    fus::common::MessageHeader header;
    std::memcpy(&header, this->_recvBuffer.data(), sizeof(header));
    this->_recvBuffer.erase(this->_recvBuffer.begin(), this->_recvBuffer.begin() + sizeof(header));
    return header;
}