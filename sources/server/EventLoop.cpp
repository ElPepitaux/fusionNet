#include "server/EventLoop.hpp"

fus::net::EventLoop::EventLoop(const std::shared_ptr<Acceptor>& acceptor, const std::shared_ptr<ManageConnection>& manageConnection)
 :  _acceptor(acceptor),
    _manageConnection(manageConnection)
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
    fus::logging::StandardLogger::info("Starting event loop...");
    this->_isRunning = true;
    this->_thread = std::thread(&EventLoop::run, this);
}

void fus::net::EventLoop::stop()
{
    if (!this->_isRunning)
        return;
    fus::logging::StandardLogger::info("Stopping event loop...");
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

void fus::net::EventLoop::run()
{
    while (this->_isRunning) {
        this->buildPollFds();
        int ret = poll(this->_pollFds.data(), this->_pollFds.size(), 1000);
        if (ret < 0) {
            fus::logging::StandardLogger::error("Poll error: " + std::string(strerror(errno)));
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
        this->_pollFds.push_back({connection->socket(), POLLIN, 0});
    }
}

void fus::net::EventLoop::handleNewConnections()
{
    if (this->_pollFds[0].revents & POLLIN) {
        auto newConnection = this->_acceptor->acceptClient();
        fus::logging::StandardLogger::info("New connection accepted");
        if (newConnection) {
            auto socket = newConnection->socket();
            this->_manageConnection->addConnection(std::move(newConnection));
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
                // Handle incoming data from the client
                // For example, you can read data and process it here
                // If the client disconnects, you can call the disconnect callback
            }
        }
    }
}