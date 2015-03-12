#pragma once
#include <stdint.h>
#include <istream>
#include <vector>


class Server {

public:

    Server()
    {
    }

    int32_t m_size;
    int32_t m_capacity;

};

typedef std::vector<Server> Servers;


std::istream& operator>>(std::istream& is, Server& a_server);

std::ostream& operator<<(std::ostream& is, const Server& a_server);

