/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** Connection
*/

#ifndef CONNECTION_HPP_
    #define CONNECTION_HPP_

    #include <iostream>
    #include <array>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include "Error.hpp"
    #include "commonDefine.hpp"
    #include "Message.hpp"
    #include <queue>

namespace fus {
    namespace net {

        class ConnectionError : public Error {
            public:
                ConnectionError(const std::string &message) : Error(message) {}
        };

        class Connection {
            public:
                Connection(Connection&& connection);
                Connection(_fd socket, struct sockaddr_in addr);
                Connection(const std::string &ip, uint32_t port);
                ~Connection();

                void onDisconnection();

                template<typename DataType>
                bool writeMessage(int id, const DataType &data)
                {
                    Message msg;
                    msg.idSet(id);
                    msg << data;
                    m_messagesIn.push(msg);
                    return true;
                }

                bool sendMessage();

                Message readMessage();

                _fd getSocket() const { return m_socket; }

            protected:
                bool writeHeader(const message_header &header);
                bool writeBody(const std::vector<uint8_t> &body);

                void _initSocket();
                void _connectSocket(const std::string &ip, const uint32_t port);

                _fd m_socket;
                struct sockaddr_in m_addr;
                std::string m_ip;
                uint32_t m_port;

                std::queue<Message> m_messagesIn;
                std::queue<Message> m_messagesOut;
            private:
        };
    }
}

#endif /* !CONNECTION_HPP_ */
