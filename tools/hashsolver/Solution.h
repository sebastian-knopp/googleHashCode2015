#pragma once

#include "PlacedServer.h"
#include "base/Vector2d.h"


class Solution {

public:

    Solution(size_t a_nmbRows, size_t a_nmbPools, size_t a_nmbSlots)
    : m_assignedCapacity(a_nmbRows, a_nmbPools, 0)
    , m_isAssigned(a_nmbRows, a_nmbSlots, 0)
    {
    }

    size_t getRating() const;

    inline size_t getNmbRows() const
    {
        return m_assignedCapacity.width();
    }

    inline size_t getNmbPools() const
    {
        return m_assignedCapacity.height();
    }

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

