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
        fus::net::Server server(1);
        fus::net::Message msg;
        std::string str;
        while (true) {
            msg =  server.handleGetMessage();
            std::cout << "NEW" << std::endl;
            std::cout << msg << std::endl;
            msg >> str;
            std::cout << str << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
