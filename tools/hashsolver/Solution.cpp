#include "Solution.h"
#include "Request.h"
#include <iostream>


Solution::Solution(const Request& a_request)
: m_servers(a_request.m_servers.size())
, m_assignedCapacity(a_request.m_nmbRows, a_request.m_nmbPools, 0)
, m_isAssigned(a_request.m_nmbRows, a_request.m_nmbSlots, 0)
{
    for (const Coordinate& c : a_request.m_unavailableSlots)
    {
        m_isAssigned(c.m_row, c.m_slot) = 1;
    }
}


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


bool Solution::isAvailable(const Coordinate& a_coord, size_t size) const
{
    for (size_t i = 0; i < size; ++i)
    {
        if (m_isAssigned(a_coord.m_row, a_coord.m_slot + i) == 1)
            return false;
    }
    return true;
}


void Solution::placeServer(const Coordinate& a_coord, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (m_isAssigned(a_coord.m_row, a_coord.m_slot + i) == 1)
            throw "error";
        m_isAssigned(a_coord.m_row, a_coord.m_slot + i) = 1;
    }
}


size_t Solution::getPoolCapacity(size_t a_pool) const
{
    size_t nmbRows = m_assignedCapacity.width();
    size_t c = 0;
    for (size_t i = 0; i < nmbRows; ++i)
    {
        c += m_assignedCapacity(i, a_pool);
    }
    return c;
}


size_t Solution::getPoolWithMinCapacity(size_t a_row) const
{
    size_t bestPool = 0;
    size_t min = std::numeric_limits<size_t>::max();
    size_t nmbPools = m_assignedCapacity.height();
    for (size_t i = 0; i < nmbPools; ++i)
    {
        size_t poolCap = getPoolCapacity(i) + m_assignedCapacity(a_row, i);
        if (poolCap < min)
        {
            min = poolCap;
            bestPool = i;
        }
    }
    return bestPool;
}


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

