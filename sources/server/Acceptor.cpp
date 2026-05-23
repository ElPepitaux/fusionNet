#include "server/Acceptor.hpp"

fus::net::Acceptor::Acceptor()
{
}

fus::net::Acceptor::~Acceptor()
{
    if (this->_listenSocket != fus::net::NO_FD)
        close(this->_listenSocket);
}

void fus::net::Acceptor::initialize(fus::net::PORT port)
{
    this->_socket();
    this->_bind(port);
    this->_listen();
}

std::shared_ptr<fus::net::Connection> fus::net::Acceptor::acceptClient()
{
    socklen_t addrlen = sizeof(this->_addrServer);
    struct sockaddr_in addrClient;
    _fd newSocket = accept(this->_listenSocket, (struct sockaddr *)&addrClient, &addrlen);
    if (newSocket < 0) {
        fus::logging::StandardLogger::error("Error: accept failed");
        return nullptr;
    }
    fus::logging::StandardLogger::info("Client connected");
    return std::make_shared<fus::net::Connection>(newSocket, addrClient);
}

fus::net::_fd fus::net::Acceptor::listenSocket() const
{
    return this->_listenSocket;
}

void fus::net::Acceptor::_socket()
{
    this->_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_listenSocket == fus::net::NO_FD) {
        fus::logging::StandardLogger::error("[Server]: socket creation failed");
        return;
    }
    int opt = 1;
    if (setsockopt(this->_listenSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        fus::logging::StandardLogger::error("Error: setsockopt failed");
        return;
    }
}

void fus::net::Acceptor::_bind(fus::net::PORT port)
{
    this->_addrServer.sin_family = AF_INET;
    this->_addrServer.sin_port = htons(port);
    this->_addrServer.sin_addr.s_addr = INADDR_ANY;

    if (bind(this->_listenSocket, (struct sockaddr *)&this->_addrServer, sizeof(this->_addrServer)) < 0) {
        fus::logging::StandardLogger::error("Error: bind failed");
        return;
    }
    fus::logging::StandardLogger::info("[Server] Socket binded on port " + std::to_string(port));
}

void fus::net::Acceptor::_listen()
{
    if (listen(this->_listenSocket, SOMAXCONN) < 0) {
        fus::logging::StandardLogger::error("Error: listen failed");
        return;
    }
    fus::logging::StandardLogger::info("[Server] Socket listening");
}