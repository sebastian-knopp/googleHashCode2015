#pragma once

#include "Coordinate.h"
#include <limits>


class PlacedServer {

public:

    PlacedServer()
    {
    }

    inline bool isPlaced() const
    {
        return m_poolIndex != std::numeric_limits<size_t>::max();
    }


public:

    Coordinate m_coord;
    size_t m_poolIndex = std::numeric_limits<size_t>::max();

};


typedef std::vector<PlacedServer> PlacedServers;


std::ostream& operator<<(std::ostream& is, const PlacedServer& a_server);
