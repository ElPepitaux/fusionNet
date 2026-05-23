#include "server/ManageConnection.hpp"

fus::net::ManageConnection::ManageConnection()
{
}

fus::net::ManageConnection::~ManageConnection()
{
}

void fus::net::ManageConnection::addConnection(std::shared_ptr<fus::net::Connection> connection)
{
    std::lock_guard<std::mutex> lock(_mutex);

    this->_connections.emplace(connection->socket(), std::move(connection));
}

void fus::net::ManageConnection::removeConnection(const fus::net::Connection& connection)
{
    std::lock_guard<std::mutex> lock(_mutex);

    this->_connections.erase(connection.socket());
}

void fus::net::ManageConnection::removeConnection(fus::net::_fd socket)
{
    std::lock_guard<std::mutex> lock(_mutex);

    this->_connections.erase(socket);
}

std::vector<std::shared_ptr<fus::net::Connection>> fus::net::ManageConnection::getConnections()
{
    std::lock_guard<std::mutex> lock(_mutex);

    std::vector<std::shared_ptr<fus::net::Connection>> connections;
    for (const auto& pair : this->_connections) {
        connections.push_back(pair.second);
    }
    return connections;
}

std::shared_ptr<fus::net::Connection> fus::net::ManageConnection::getConnection(fus::net::_fd socket)
{
    std::lock_guard<std::mutex> lock(_mutex);

    auto it = this->_connections.find(socket);
    if (it != this->_connections.end()) {
        return it->second;
    }
    return nullptr;
}