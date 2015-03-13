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


size_t Solution::getRating() const
{
    size_t overallMin = std::numeric_limits<size_t>::max();
    for (size_t poolIndex = 0; poolIndex != getNmbPools(); ++poolIndex)
    {
        size_t poolCapacity = 0;
        for (size_t rowIndex = 0; rowIndex != getNmbRows(); ++rowIndex)
        {
            poolCapacity += m_assignedCapacity(rowIndex, poolIndex);
        }

        size_t poolMin = std::numeric_limits<size_t>::max();
        for (size_t rowIndex = 0; rowIndex != getNmbRows(); ++rowIndex)
        {
            size_t currentMin = poolCapacity - m_assignedCapacity(rowIndex, poolIndex);
            if (currentMin < poolMin)
                poolMin = currentMin;
        }

        if (poolMin < overallMin)
            overallMin = poolMin;
    }
    return overallMin;
}
