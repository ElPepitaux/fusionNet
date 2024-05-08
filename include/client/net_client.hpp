/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** net_Client
*/

#ifndef NET_Client_HPP_
    #define NET_Client_HPP_

    #include <iostream>
    #include <array>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include <memory>
    #include "Error.hpp"
    #include "commonDefine.hpp"
    #include "Connection.hpp"

namespace fus {
    namespace net {

        class ClientError : public Error {
            public:
                ClientError(const std::string &message) : Error(message) {}
                ~ClientError() = default;
        };

        class Client {
            public:
                Client(const std::string &ip, const uint32_t port);
                ~Client();

                template<typename DataType>
                bool writeMessage(int id, const DataType &data)
                {
                    return this->m_connection->writeMessage(id, data);
                }

                bool sendMessage();
    
            protected:
            private:
                std::unique_ptr<Connection> m_connection;
        };

    } // namespace
}

#endif /* !NET_Client_HPP_ */
