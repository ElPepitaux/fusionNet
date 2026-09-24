#pragma once

#include "common/Platform.hpp"
#include "common/Types.hpp"
#include "common/Logger.hpp"

namespace fus::common {

    #pragma pack(push, 1)
    struct MessageHeader {
        ID id;
        uint32_t length;
    };
    #pragma pack(pop)
    class Message {
        public:
            Message() = default;
            ~Message() = default;

            ID& id();
            const ID& id() const;

            void setId(ID id);

            std::vector<Byte>& body();
            const std::vector<Byte>& body() const;

            void setBody(const std::vector<Byte>& body);

            void resetReadPos() const;

            template<typename T>
            void write(const T& data)
            {
                const Byte* rawData = reinterpret_cast<const Byte*>(&data);
                _body.insert(_body.end(), rawData, rawData + sizeof(T));
            }

            template<typename T>
            void read(T& data) const
            {
                if (_readPos + sizeof(T) > _body.size()) {
                    fus::logging::StandardLogger::error("Not enough data to read into type: " + std::string(typeid(T).name()));
                    return;
                }
                std::memcpy(&data, _body.data() + _readPos, sizeof(T));
                _readPos += sizeof(T);
            }



        private:
            ID _id = 0;
            std::vector<Byte> _body;
            mutable size_t _readPos = 0;
    };
} // namespace fus::common