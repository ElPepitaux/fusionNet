 /*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** Message
*/

#ifndef MESSAGE_HPP_
    #define MESSAGE_HPP_

    #include <iostream>
    #include <array>
    #include "Error.hpp"
    #include <ostream>
    #include <vector>
    #include <cstring>
    #include <cstdint>
    #include "Adapter.hpp"

namespace fus {
    namespace net {

        class MessageError : public Error {
            public:
                MessageError(const std::string &message) : Error(message) {}
                ~MessageError() = default;
        };


        struct message_header
        {
            int id{};
            uint32_t size = 0;
        };

        class Message {
            public:
                Message() = default;
                virtual ~Message() = default;

                uint32_t size() const
                {
                    return m_body.size();
                }

                int id() const
                {
                    return this->m_header.id;
                }

                void idSet(int id)
                {
                    this->m_header.id = id;
                }

                template <typename DataType>
                friend Message& operator<<(Message& msg, const DataType &data)
                {
                    fus::mes::adapter<DataType> adapter;
                    if (!(std::is_standard_layout<DataType>::value))
                        throw MessageError("Data is not standard layout");
                    adapter.bodySet(msg.m_body, data);
                    msg.m_header.size = msg.size();
                    return msg;
                }

                template <typename DataType>
                friend Message& operator>>(Message& msg, DataType &data)
                {
                    fus::mes::adapter<DataType> adapter;
                    if (!(std::is_standard_layout<DataType>::value))
                        throw MessageError("Data is not standard layout");
                    adapter.bodyGet(msg.m_body, data);
                    msg.m_header.size = msg.size();
                    return msg;
                }

                message_header headerGet() const
                {
                    return this->m_header;
                }

                void headerSet(const message_header &header)
                {
                    this->m_header = header;
                }

                void bodySet(const std::vector<uint8_t> &body)
                {
                    this->m_body = body;
                }

                std::vector<uint8_t> bodyGet() const
                {
                    return this->m_body;
                }

                friend std::ostream& operator<<(std::ostream &os, const fus::net::Message &msg)
                {
                    os << "ID: " << static_cast<int>(msg.id()) << " Size: " << msg.size();
                    return os;
                }
            protected:
                message_header m_header{};
                std::vector<uint8_t> m_body;
            private:
        };
    }
}

std::ostream& operator<<(std::ostream &os, const fus::net::message_header &header);

#endif /* !MESSAGE_HPP_ */
