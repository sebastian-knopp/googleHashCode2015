#include "Server.h"




std::istream&operator>>(std::istream& is, Server& a_server)
{
    is >> a_server.m_size;
    is >> a_server.m_capacity;
    return is;
}




std::ostream&operator<<(std::ostream& is, const Server& a_server)
{
    is << "(size = " << a_server.m_size << ", capacity = " << a_server.m_capacity << ")";
    return is;
}



