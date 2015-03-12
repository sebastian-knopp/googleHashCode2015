#include "Server.h"




std::istream&operator>>(std::istream& is, Server& a_server)
{
    is >> a_server.m_capacity;
    is >> a_server.m_size;
    return is;
}


