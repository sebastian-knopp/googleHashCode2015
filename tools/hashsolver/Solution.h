#pragma once

#include "PlacedServer.h"
#include "base/Vector2d.h"


class Solution {

public:

    Solution(size_t a_nmbRows, size_t a_nmbPools)
    : m_assignedCapacity(a_nmbRows, a_nmbPools, 0)
    {
    }


public:

    PlacedServers m_servers;

    base::Vector2d<size_t> m_assignedCapacity;

};


//size_t getRating(const Request a_request, const Solution& a_solution);



std::ostream& operator<<(std::ostream& is, const Solution& a_solution);

