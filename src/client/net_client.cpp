/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** net_Client
*/

#include "net_client.hpp"

fus::net::Client::Client(const std::string &ip, uint32_t port)
{
    try {
        this->m_connection = std::make_unique<Connection>(ip, port);
    } catch (const std::exception &e) {
        std::string error = "[Client ERROR]: ";
        error += '\t' + e.what();
        throw ClientError(error);
    }
}


fus::net::Client::~Client()
{
    this->m_connection->onDisconnection();
}

bool fus::net::Client::sendMessage()
{
    return this->m_connection->sendMessage();
}
