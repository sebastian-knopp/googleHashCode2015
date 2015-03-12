#include "Solution.h"
#include "Request.h"
#include <iostream>


std::ostream& operator<<(std::ostream& is, const Solution& a_solution)
{

    for (size_t i = 0; i != a_solution.m_servers.size(); ++i)
    {
        is << a_solution.m_servers[i];
        if (i + 1 != a_solution.m_servers.size())
            is << "\n";
    }
    return is;
}


/*

size_t getRating(const Request a_request, const Solution& a_solution)
{

    base::Vector2d<size_t> assignedCapacity(a_request.m_nmbRows, a_request.m_nmbPools, 0);

    for (size_t i = 0 ; i != a_solution.m_servers.size(); ++i)
    {
        assignedCapacity(a_solution.m_servers[i])
    }

    std::vector<size_t>;
}
*/
