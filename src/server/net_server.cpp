/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** Server
*/

#include "net_server.hpp"

fus::net::Server::Server(uint32_t nbrSlaver)
{
    this->m_handleEvent = std::make_unique<HandleEvent>(nbrSlaver);
}

fus::net::Server::~Server()
{
}

fus::net::Message fus::net::Server::handleGetMessage()
{
    return this->m_handleEvent->handleGetMessage();
}
