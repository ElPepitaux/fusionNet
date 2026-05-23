#pragma once

#include <vector>
#include <cstdint>
#include <cstring>

struct example_packet_data {
    static constexpr uint32_t ID = 1;

    int value;

    static std::vector<uint8_t> serialize(const example_packet_data& data) {
        std::vector<uint8_t> buffer(sizeof(data.value));
        std::memcpy(buffer.data(), &data.value, sizeof(data.value));
        return buffer;
    }

    static example_packet_data deserialize(const std::vector<uint8_t>& buffer) {
        example_packet_data data;
        if (buffer.size() >= sizeof(data.value)) {
            std::memcpy(&data.value, buffer.data(), sizeof(data.value));
        }
        return data;
    }
};