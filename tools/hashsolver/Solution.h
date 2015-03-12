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


std::ostream& operator<<(std::ostream& is, const Solution& a_solution);

