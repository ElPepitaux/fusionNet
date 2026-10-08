#include "server/EventLoop.hpp"
#include "common/PollPoller.hpp"

fus::net::EventLoop::EventLoop(const std::shared_ptr<Acceptor>& acceptor,
        const std::shared_ptr<ManageConnection>& manageConnection,
        const std::shared_ptr<fus::common::PacketDispatcher>& packetDispatcher)
 :  _acceptor(acceptor),
    _manageConnection(manageConnection),
    _packetDispatcher(packetDispatcher)
{
    this->_poller = std::make_unique<PollPoller>();
    this->_threadPool = std::make_unique<fus::common::ThreadPool>(4);
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
    this->_poller->add(this->_acceptor->listenSocket(), Readable);
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
        std::vector<Event> events;
        int ret = this->_poller->wait(events, -1);
        if (ret < 0) {
            fus::logging::StandardLogger::error("[Server] Poll error: " + std::string(strerror(errno)));
            continue;
        }
        if (ret == 0)
            continue;
        this->handleNewConnections(events);
        this->handleClientEvents(events);
    }
}

void fus::net::EventLoop::handleNewConnections(const std::vector<Event>& events)
{
    for (const auto& event : events) {
        if (event.fd != this->_acceptor->listenSocket())
            continue;
        if (!(event.events & Readable))
            continue;

        auto newConnection = this->_acceptor->acceptClient();
        if (newConnection) {
            auto socket = newConnection->socket();
            this->_manageConnection->addConnection(newConnection);
            this->_poller->add(socket, Readable);
            fus::logging::StandardLogger::info("[Server] New connection accepted with socket: " + std::to_string(socket));
            if (this->_connectCallback)
                this->_connectCallback(this->_manageConnection->getConnection(socket));
        }
    }
}

void fus::net::EventLoop::handleClientEvents(const std::vector<Event>& events)
{
    for (const auto& event : events) {
        if (event.fd == this->_acceptor->listenSocket())
            continue;
        if (!(event.events & (Readable | Closed)))
            continue;

        auto connection = this->_manageConnection->getConnection(event.fd);
        if (!connection)
            continue;

        try {
            auto messages = connection->receive();
            for (auto& msg : messages) {
                if (this->_messageCallback)
                    this->_messageCallback(connection, msg);
                this->_threadPool->submit([this, connection, msg]() {
                    this->_packetDispatcher->dispatch(connection, msg);
                });
            }
        } catch (const fus::exception::ClientDisconnected &e) {
            if (_disconnectCallback)
                _disconnectCallback(connection);

            this->_poller->remove(connection->socket());
            _manageConnection->removeConnection(connection->socket());

            connection->disconnect();
        }
    }
}
