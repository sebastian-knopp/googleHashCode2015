#pragma once

#include "PlacedServer.h"
#include "base/Vector2d.h"

class Request;


class Solution {

public:

    explicit Solution(const Request& a_request);

    size_t getRating() const;

    inline size_t getNmbRows() const
    {
        return m_assignedCapacity.width();
    }

    inline size_t getNmbPools() const
    {
        return m_assignedCapacity.height();
    }

    bool isAvailable(const Coordinate& a_coord, size_t size) const;

    void placeServer(const Coordinate& a_coord, size_t size);

    size_t getPoolCapacity(size_t a_pool) const;

    size_t getPoolWithMinCapacity(size_t a_row) const;


public:

    PlacedServers m_servers;

    //! capacity of a pool in a row
    base::Vector2d<size_t> m_assignedCapacity;

    base::Vector2d<size_t> m_isAssigned;


};


//size_t getRating(const Request a_request, const Solution& a_solution);



std::ostream& operator<<(std::ostream& is, const Solution& a_solution);

