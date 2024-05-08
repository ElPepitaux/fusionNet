/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** main
*/

#include <iostream>
#include "net_client.hpp"

int main(int ac, char **av)
{
    try {
        fus::net::Client slave("127.0.0.1", 8080);
        if (!slave.writeMessage(1, "Hello World")) {
            std::cerr << "Error: writeMessage failed" << std::endl;
            return 84;
        }
        if (!slave.sendMessage()) {
            std::cerr << "Error: sendMessage failed" << std::endl;
            return 84;
        }
        while (true)
        {
            /* code */
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
