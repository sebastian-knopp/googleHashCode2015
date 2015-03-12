#pragma once

#include "PlacedServer.h"
#include <vector>


class Solution {

public:

    Solution()
    {
    }

public:

    PlacedServers m_servers;

};


//size_t getRating(const Request a_request, const Solution& a_solution);



std::ostream& operator<<(std::ostream& is, const Solution& a_solution);

