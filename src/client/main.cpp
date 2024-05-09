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
        if (ac != 3) {
            std::cerr << "Usage: " << av[0] << " <ip> <port>" << std::endl;
            return 1;
        }
        int port = std::stoi(av[2]);
        fus::net::Client slave(av[1], port);
        while (true) {

        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
