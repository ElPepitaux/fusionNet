/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** Connection
*/

#include "Connection.hpp"

fus::net::Connection::Connection(Connection&& other)
{
    this->m_socket = other.m_socket;
    this->m_addr = other.m_addr;
    this->m_ip = other.m_ip;
    other.m_socket = -1;
}

fus::net::Connection::Connection(_fd socket, struct sockaddr_in addr)
{
    this->m_socket = socket;
    this->m_addr = addr;
}

fus::net::Connection::Connection(const std::string &ip, uint32_t port)
{
    this->_initSocket();
    this->_connectSocket(ip, port);
    this->m_ip = ip;
    this->m_port = port;
}

fus::net::Connection::~Connection()
{
}

void fus::net::Connection::onDisconnection()
{
    close(this->m_socket);
}

void fus::net::Connection::_initSocket()
{
    this->m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->m_socket == -1)
        throw ConnectionError("[Client ERROR]: socket creation failed");
    std::cout << "[Client INFO] Socket created" << std::endl;
}

void fus::net::Connection::_connectSocket(const std::string &ip, uint32_t port)
{
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (connect(this->m_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        throw ConnectionError("[Client ERROR]: connection failed");
    std::cout << "[Client INFO] Connected to master" << std::endl;
    if (getsockname(this->m_socket, (struct sockaddr *)&this->m_addr, (socklen_t *)&this->m_addr) == -1)
        throw ConnectionError("[Client ERROR]: getsockname failed");
}

bool fus::net::Connection::sendMessage()
{
    if (this->m_messagesIn.empty())
        return false;
    Message msg = this->m_messagesIn.front();
    this->m_messagesIn.pop();
    if (!this->writeHeader(msg.headerGet()))
        return false;
    if (!this->writeBody(msg.bodyGet()))
        return false;
    return true;
}

bool fus::net::Connection::writeHeader(const message_header &header)
{
    std::cout << header << std::endl;
    if (write(this->m_socket, &header, sizeof(header)) == -1)
        throw ConnectionError("[Client ERROR]: send failed");
    return true;
}

bool fus::net::Connection::writeBody(const std::vector<uint8_t> &body)
{
    if (write(this->m_socket, body.data(), body.size()) == -1)
        throw ConnectionError("[Client ERROR]: send failed");
    return true;
}

fus::net::Message fus::net::Connection::readMessage()
{
    Message msg;
    message_header header;
    int ret = recv(this->m_socket, &header, sizeof(header), 0);
    if (ret == -1)
        throw ConnectionError("[Client ERROR]: recv failed");
    if (ret == 0)
        throw ConnectionLost("[Client ERROR]: connection closed");
    msg.headerSet(header);
    std::vector<uint8_t> body(header.size);
    if (recv(this->m_socket, body.data(), header.size, 0) == -1)
        throw ConnectionError("[Client ERROR]: recv failed");
    msg.bodySet(body);
    return msg;
}

bool fus::net::Connection::operator==(const Connection &connection)
{
    return this->m_socket == connection.m_socket;
}

bool fus::net::Connection::operator!=(const Connection &connection) const
{
    return this->m_socket != connection.m_socket;
}

fus::net::Connection &fus::net::Connection::operator=(const Connection &connection)
{
    this->m_socket = connection.m_socket;
    this->m_addr = connection.m_addr;
    this->m_ip = connection.m_ip;
    return *this;
}
