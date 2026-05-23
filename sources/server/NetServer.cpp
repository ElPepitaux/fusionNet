#include "server/NetServer.hpp"

fus::net::NetServer::NetServer()
{
    this->_manageConnection = std::make_unique<ManageConnection>();
    this->_acceptor = std::make_shared<Acceptor>();
    this->_eventLoop = std::make_unique<EventLoop>(this->_acceptor, this->_manageConnection);
}

fus::net::NetServer::~NetServer()
{
    this->stop();
}

void fus::net::NetServer::start(fus::net::PORT port)
{
    if (this->isRunning())
        return;
    this->_acceptor->initialize(port);
    this->_eventLoop->start();
}

void fus::net::NetServer::stop()
{
    if (!this->isRunning())
        return;
    this->_eventLoop->stop();
}

bool fus::net::NetServer::isRunning() const
{
    return this->_eventLoop->isRunning();
}

void fus::net::NetServer::onConnect(const ConnectCallback& callback)
{
    this->_eventLoop->onConnect(callback);
}

void fus::net::NetServer::onDisconnect(const DisconnectCallback& callback)
{
    this->_eventLoop->onDisconnect(callback);
}