#pragma once
#include <stdint.h>
#include <istream>
#include <vector>


class Server {

public:

    Server()
    {
    }

    double getRatio() const
    {
        return static_cast<double>(m_capacity) / static_cast<double>(m_size);
    }


public:

    size_t m_size;
    size_t m_capacity;

};

typedef std::vector<Server> Servers;


std::istream& operator>>(std::istream& is, Server& a_server);

std::ostream& operator<<(std::ostream& is, const Server& a_server);

