/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** HandleEvent
*/

#ifndef HANDLEEVENT_HPP_

    #define HANDLEEVENT_HPP_

    #include "Message.hpp"
    #include <sys/select.h>
    #include <iostream>
    #include <array>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <deque>
    #include <string.h>
    #include "Error.hpp"
    #include <memory>
    #include "commonDefine.hpp"
    #include "HandleEvent.hpp"
    #include "Connection.hpp"
    #include <algorithm>

namespace fus {
    namespace net {
        class HandleEventError : public Error {
            public:
                HandleEventError(const std::string &message) : Error(message) {}
                ~HandleEventError() = default;
        };

        class HandleEvent {
            public:
                HandleEvent(uint32_t nbrSlaves);
                ~HandleEvent();

                Message handleGetMessage();

                template<typename DataType>
                bool sendMessageToClient(fus::net::Connection &connection, int id, const DataType &data)
                {
                    try {
                        connection.writeMessage(id, data);
                        connection.sendMessage();
                    } catch (const ConnectionLost &e) {
                        std::cerr << e.what() << std::endl;
                        connection.onDisconnection();
                        auto it = std::find(this->m_connections.begin(), this->m_connections.end(), connection);
                        this->m_connections.erase(it);
                        return false;
                    }
                    return true;
                }

                template<typename DataType>
                bool sendMessageAllClient(int id, const DataType &data)
                {
                    for (auto &connection : this->m_connections) {
                        if (!sendMessageToClient(connection, id, data))
                            return false;
                    }
                    return true;
                }

            protected:
                int _initFdSet();
                void _initSocket();
                void _bindSocket();
                void _listenSocket();
                void _acceptSlaves();


                fd_set m_readfds;
                fd_set m_writefds;
                fd_set m_errorfds;
                _fd m_socketServer;
                struct sockaddr_in m_addrServer;
                std::deque<Connection> m_connections;
                const uint32_t m_nbrSlaves;
            private:
        };
    }
}

#endif /* !HANDLEEVENT_HPP_ */
