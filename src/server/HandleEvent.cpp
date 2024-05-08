/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** HandleEvent
*/

#include "HandleEvent.hpp"

fus::net::HandleEvent::HandleEvent(uint32_t nbrSlaves) : m_nbrSlaves(nbrSlaves)
{
    this->_initSocket();
    this->_bindSocket();
    this->_listenSocket();
    this->_acceptSlaves();
}


fus::net::HandleEvent::~HandleEvent()
{
}

void fus::net::HandleEvent::_initSocket()
{
    this->m_socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (this->m_socketServer == -1)
        throw HandleEventError("[Server ERROR]: socket creation failed");
    this->m_addrServer.sin_family = AF_INET;
    this->m_addrServer.sin_port = htons(PORT);
    this->m_addrServer.sin_addr.s_addr = INADDR_ANY;
    std::cout << "[Server INFO] Socket created" << std::endl;
    std::cout << "[Server INFO] Port: " << PORT << std::endl;
}

void fus::net::HandleEvent::_bindSocket()
{
    if (bind(this->m_socketServer, (struct sockaddr *)&this->m_addrServer, sizeof(this->m_addrServer)) < 0)
        throw HandleEventError("Error: bind failed");
    std::cout << "[Server INFO] Socket binded" << std::endl;
}

void fus::net::HandleEvent::_listenSocket()
{
    if (listen(this->m_socketServer, this->m_nbrSlaves) < 0)
        throw HandleEventError("Error: listen failed");
    std::cout << "[Server INFO] Socket listening" << std::endl;
}

void fus::net::HandleEvent::_acceptSlaves()
{
    socklen_t addrlen = 0;
    struct sockaddr_in addr;
    _fd newSocket = 0;

    for (uint32_t i = 0; i < this->m_nbrSlaves; i++) {
        addrlen = sizeof(addr);
        newSocket = accept(this->m_socketServer, (struct sockaddr *)&addr, &addrlen);
        if (newSocket < 0)
            throw HandleEventError("Error: accept failed");
        std::cout << "[Server INFO] Slave connected (" << i + 1 << "/" << this->m_nbrSlaves << ")"  << std::endl;
        this->m_connections.push_back(std::move(Connection(newSocket, addr)));
    }
}

int fus::net::HandleEvent::_initFdSet()
{
    int max = this->m_socketServer;
    FD_ZERO(&this->m_readfds);
    FD_ZERO(&this->m_writefds);
    FD_ZERO(&this->m_errorfds);
    FD_SET(this->m_socketServer, &this->m_readfds);
    FD_SET(this->m_socketServer, &this->m_errorfds);
    for (auto &connection : this->m_connections) {
        if (connection.getSocket() > max)
            max = connection.getSocket();
        FD_SET(connection.getSocket(), &this->m_readfds);
        FD_SET(connection.getSocket(), &this->m_errorfds);
    }
    return max;
}

fus::net::Message fus::net::HandleEvent::handleGetMessage()
{
    int maxFd = this->_initFdSet();
    if (select(maxFd + 1, &this->m_readfds, NULL, NULL, NULL) < 0) {
        std::string error = "Error: select failed";
        error += "\t" + std::string(strerror(errno));
        throw HandleEventError(error);
    }
    for (auto &connection : this->m_connections) {
        if (FD_ISSET(connection.getSocket(), &this->m_readfds)) {
            return connection.readMessage();
        }
    }
    return Message();
}
