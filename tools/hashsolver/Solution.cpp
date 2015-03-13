#include "Solution.h"
#include "Request.h"
#include <iostream>


Solution::Solution(const Request& a_request)
: m_request(&a_request)
, m_servers(a_request.m_servers.size())
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
    std::mt19937 rndGenerator;
    std::uniform_int_distribution<> intDistribution(0, std::numeric_limits<int>::max());
    std::uniform_real_distribution<> realDistribution(0, 1.0);

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


bool Solution::isPlacable(const Coordinate& a_coord, size_t a_serverIndex) const
{
    for (size_t i = 0; i < m_request->m_servers[a_serverIndex].m_size; ++i)
    {
        if (m_isAssigned(a_coord.m_row, a_coord.m_slot + i) == 1)
            return false;
    }
    return true;
}


void Solution::placeServer(const Coordinate& a_coord, size_t a_serverIndex, size_t a_poolIndex)
{
    for (size_t i = 0; i < m_request->m_servers[a_serverIndex].m_size; ++i)
    {
        if (m_isAssigned(a_coord.m_row, a_coord.m_slot + i) == 1)
            throw "error";
        m_isAssigned(a_coord.m_row, a_coord.m_slot + i) = 1;
    }
    m_servers[a_serverIndex].m_coord = a_coord;
    m_servers[a_serverIndex].m_poolIndex = a_poolIndex;
    m_assignedCapacity(a_coord.m_row, a_poolIndex) += m_request->m_servers[a_serverIndex].m_capacity;
}


void Solution::removeServer(size_t a_serverIndex)
{
    const PlacedServer& server = m_servers[a_serverIndex];
    m_assignedCapacity(server.m_coord.m_row, server.m_poolIndex) -= m_request->m_servers[a_serverIndex].m_capacity;

    for (size_t i = 0; i < m_request->m_servers[a_serverIndex].m_size; ++i)
    {
        m_isAssigned(server.m_coord.m_row, server.m_coord.m_slot + i) = 0;
    }
    m_servers[a_serverIndex] = PlacedServer();
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

/*
bool Solution::canSwapServers(size_t a_server1, size_t a_server2)
{
    return false;
}
*/

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



Solution simulatedAnnealing(std::mt19937& rndGenerator,
                            const Solution& a_solution, double initialTemperature, double coolingFactor, size_t runs, bool print)
{
    std::uniform_int_distribution<> intDistribution(0, std::numeric_limits<int>::max());
    std::uniform_real_distribution<> realDistribution(0, 1.0);

    Solution s = a_solution;
    Solution bestSolution = a_solution;
    size_t bestRating = bestSolution.getRating();


    std::vector<size_t> placedServerIndices;
    for (size_t i = 0; i != a_solution.m_servers.size(); ++i)
    {
        if (s.m_servers[i].isPlaced())
            placedServerIndices.push_back(i);
    }

    double previousRating = static_cast<double>(bestRating);
    double currentTemperature = initialTemperature;
    for (size_t i = 0; i != runs; ++i)
    {
        size_t randomIndex1 = placedServerIndices[intDistribution(rndGenerator) % placedServerIndices.size()];

        PlacedServer old = s.m_servers[randomIndex1];
        size_t oldPoolIndex = s.m_servers[randomIndex1].m_poolIndex;
        size_t newPoolIndex = intDistribution(rndGenerator) % s.getNmbPools();

        s.removeServer(randomIndex1);
        s.placeServer(old.m_coord, randomIndex1, newPoolIndex);

        double currentRating = static_cast<double>(s.getRating());

        const double diff = previousRating - currentRating;
        const double p = realDistribution(rndGenerator);
        const double e = std::exp(-diff / currentTemperature);

        if (p > e)
        {
            s.m_servers[randomIndex1].m_poolIndex = oldPoolIndex;
            s.m_assignedCapacity(s.m_servers[randomIndex1].m_coord.m_row, oldPoolIndex) += s.m_request->m_servers[randomIndex1].m_capacity;
            s.m_assignedCapacity(s.m_servers[randomIndex1].m_coord.m_row, newPoolIndex) -= s.m_request->m_servers[randomIndex1].m_capacity;
        }
        else
        {
            previousRating = currentRating;
        }

        if (previousRating > bestRating)
        {
            bestSolution = s;
            bestRating = previousRating;
            if (print)
            {
                std::cout << "new best rating : "<< bestRating << std::endl;
                std::cout << "i: "<< i << std::endl;
            }
        }

        currentTemperature *= coolingFactor;
    }
    return bestSolution;
}
