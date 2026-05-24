#include "server/EventLoop.hpp"

fus::net::EventLoop::EventLoop(const std::shared_ptr<Acceptor>& acceptor,
        const std::shared_ptr<ManageConnection>& manageConnection,
        const std::shared_ptr<fus::common::PacketDispatcher>& packetDispatcher)
 :  _acceptor(acceptor),
    _manageConnection(manageConnection),
    _packetDispatcher(packetDispatcher)
{
}

fus::net::EventLoop::~EventLoop()
{
    this->stop();
}

void fus::net::EventLoop::start()
{
    if (this->_isRunning)
        return;
    fus::logging::StandardLogger::info("[Server] Starting event loop...");
    this->_isRunning = true;
    this->_thread = std::thread(&EventLoop::run, this);
}

void fus::net::EventLoop::stop()
{
    if (!this->_isRunning)
        return;
    fus::logging::StandardLogger::info("[Server] Stopping event loop...");
    this->_isRunning = false;
    if (this->_thread.joinable())
        this->_thread.join();
}

bool fus::net::EventLoop::isRunning() const
{
    return this->_isRunning;
}

void fus::net::EventLoop::onConnect(const ConnectCallback& callback)
{
    this->_connectCallback = callback;
}

void fus::net::EventLoop::onDisconnect(const DisconnectCallback& callback)
{
    this->_disconnectCallback = callback;
}

void fus::net::EventLoop::onMessage(const MessageCallback& callback)
{
    this->_messageCallback = callback;
}

void fus::net::EventLoop::run()
{
    while (this->_isRunning) {
        this->buildPollFds();
        int ret = poll(this->_pollFds.data(), this->_pollFds.size(), -1);
        if (ret < 0) {
            fus::logging::StandardLogger::error("[Server] Poll error: " + std::string(strerror(errno)));
            continue;
        }
        if (ret == 0)
            continue;
        this->handleNewConnections();
        this->handleClientEvents();
    }
}

void fus::net::EventLoop::buildPollFds()
{
    this->_pollFds.clear();
    this->_pollFds.push_back({this->_acceptor->listenSocket(), POLLIN, 0});
    for (const auto& connection : this->_manageConnection->getConnections()) {
        if (connection->isConnected())
            this->_pollFds.push_back({connection->socket(), POLLIN, 0});
        else
            _manageConnection->removeConnection(connection->socket());
    }
}

void fus::net::EventLoop::handleNewConnections()
{
    if (this->_pollFds[0].revents & POLLIN) {
        auto newConnection = this->_acceptor->acceptClient();
        fus::logging::StandardLogger::info("[Server] New connection accepted with socket: " + std::to_string(newConnection->socket()));
        if (newConnection) {
            auto socket = newConnection->socket();
            this->_manageConnection->addConnection(newConnection);
            if (this->_connectCallback)
                this->_connectCallback(this->_manageConnection->getConnection(socket));
        }
    }
}

void fus::net::EventLoop::handleClientEvents()
{
    for (size_t i = 1; i < this->_pollFds.size(); ++i) {
        if (this->_pollFds[i].revents & POLLIN) {
            auto connection = this->_manageConnection->getConnection(this->_pollFds[i].fd);
            if (connection) {
                try {
                    auto messages = connection->receive();
                    for (auto& msg : messages) {
                        if (this->_messageCallback)
                            this->_messageCallback(connection, msg);
                        this->_packetDispatcher->dispatch(connection, msg);
                    }
                } catch (const fus::exception::ClientDisconnected &e) {
                    if (_disconnectCallback)
                        _disconnectCallback(connection);

                    _manageConnection->removeConnection(connection->socket());

                    connection->disconnect();
                }
            }
        }
    }
}
