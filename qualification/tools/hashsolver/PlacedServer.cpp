#include "PlacedServer.h"





std::ostream& operator<<(std::ostream& is, const PlacedServer& a_server)
{
    if (!a_server.isPlaced())
    {
        is << "x";
        return is;
    }

    is << a_server.m_coord << " " << a_server.m_poolIndex;
    return is;
}

