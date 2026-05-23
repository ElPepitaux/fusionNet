#include "client/NetClient.hpp"

fus::net::NetClient::NetClient()
{
    _dispatcher = std::make_unique<fus::common::PacketDispatcher>();
}

fus::net::NetClient::~NetClient()
{
    disconnect();
}

void fus::net::NetClient::connect(const std::string& host, uint16_t port)
{
    if (isConnected()) {
        disconnect();
    }

    if (!this->_makeConnection(host, port)) {
        return;
    }
    _running = true;
    _pollingThread = std::thread(&NetClient::run, this);
}

void fus::net::NetClient::disconnect()
{
    if (isConnected()) {
        _running = false;
        if (_pollingThread.joinable()) {
            _pollingThread.join();
        }
        _connection.reset();
    }
}

bool fus::net::NetClient::isConnected() const
{
    return _connection && _connection->isConnected();
}

bool fus::net::NetClient::_makeConnection(const std::string& host, uint16_t port)
{
    fus::net::_fd fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        fus::logging::StandardLogger::error("[Client] Failed to create socket");
        return false;
    }
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);

    if (::connect(fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        fus::logging::StandardLogger::error("[Client] Failed to connect to server");
        close(fd);
        return false;
    }
    fus::logging::StandardLogger::info("[Client] Connected to server " + host + ":" + std::to_string(port));
    _connection = std::make_shared<Connection>(fd, serverAddr);
    return true;
}

void fus::net::NetClient::run()
{
    struct pollfd pollFd;
    pollFd.fd = _connection->socket();
    pollFd.events = POLLIN;
    while (_running) {
        int ret = poll(&pollFd, 1, 1000);
        if (ret < 0) {
            fus::logging::StandardLogger::error("[Client] Poll error: " + std::string(strerror(errno)));
            break;
        }
        if (ret == 0)
            continue;

        if (pollFd.revents & POLLIN) {
            auto messages = _connection->receive();
            for (auto& msg : messages) {
                _dispatcher->dispatch(_connection, msg);
            }
        }
    }
}
