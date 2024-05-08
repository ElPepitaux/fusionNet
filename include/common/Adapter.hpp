/*
** EPITECH PROJECT, 2024
** $
** File description:
** Adapter
*/

#ifndef ADAPTER_HPP_
    #define ADAPTER_HPP_

    #include <iostream>
    #include <array>
    #include "Error.hpp"
    #include <ostream>
    #include <vector>
    #include <cstring>
    #include <string>
    #include <cstdint>
    #include <type_traits>

namespace fus {
    namespace mes {
        template <typename T>
        struct adapter;

        template <>
        struct adapter<std::string> {
            void bodyGet(std::vector<uint8_t> &body, std::string &str)
            {
                str = std::string(reinterpret_cast<char *>(body.data()), body.size());
                body.clear();
            }
        };

        template<typename T>
        requires std::is_integral_v<T>
        struct adapter<T> {
            void bodyGet(std::vector<uint8_t> &body, T data)
            {
                size_t i = body.size() - sizeof(T);
                std::memcpy(&data, body.data() + i, sizeof(T));
                body.resize(i);
            }
        };

        template<typename F>
        requires std::is_floating_point_v<F>
        struct adapter<F> {
            void bodyGet(std::vector<uint8_t> &body, F data)
            {
                size_t i = body.size() - sizeof(F);
                std::memcpy(&data, body.data() + i, sizeof(F));
                body.resize(i);
            }
        };

        template<typename T>
        requires std::is_standard_layout_v<T>
        struct adapter<T> {
            void bodyGet(std::vector<uint8_t> &body, T &data)
            {
                size_t i = body.size() - sizeof(T);
                std::memcpy(&data, body.data() + i, sizeof(T));
                body.resize(i);
            }
        };
    }
} // namespace fu

#endif /* !ADAPTER_HPP_ */
