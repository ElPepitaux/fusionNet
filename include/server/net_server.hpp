/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** net_Server
*/

#ifndef NET_Server_HPP_
    #define NET_Server_HPP_

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

namespace fus {
    namespace net {

        class ServerError : public Error {
            public:
                ServerError(const std::string &message) : Error(message) {}
                ~ServerError() = default;
        };

        class Server {
            public:
                Server(uint32_t nbrSlaves);
                ~Server();

                Message handleGetMessage();

            protected:
            private:
                std::unique_ptr<HandleEvent> m_handleEvent;
        };
    }
}

#endif /* !NET_Server_HPP_ */
