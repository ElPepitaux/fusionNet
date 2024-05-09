/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** main
*/

#include <iostream>
#include <array>
#include "net_server.hpp"
#include "Message.hpp"

int main(int ac, char **av)
{
    try {
        if (ac != 2) {
            std::cerr << "Usage: " << av[0] << " <slaves>" << std::endl;
            return 1;
        }
        int nbr = std::stoi(av[1]);
        fus::net::Server server(nbr);
        fus::net::Message msg;
        std::string str;
        server.sendMessageAllClient(0, "Hello");
        while (true) {
            msg = server.handleGetMessage();
            std::cout << msg << std::endl;
            msg >> str;
            std::cout << str << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
