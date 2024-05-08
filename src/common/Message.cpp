/*
** EPITECH PROJECT, 2024
** clusterCuda
** File description:
** Message
*/

#include "Message.hpp"

std::ostream& operator<<(std::ostream &os, const fus::net::message_header &header)
{
    os << "ID: " << header.id << " Size: " << header.size;
    return os;
}

