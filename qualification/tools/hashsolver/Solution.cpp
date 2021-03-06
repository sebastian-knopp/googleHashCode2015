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


void Solution::printBalance() const
{
    for (size_t poolIndex = 0; poolIndex != getNmbPools(); ++poolIndex)
    {
        std::cout << "pool " << poolIndex;
        size_t poolCapacity = 0;
        for (size_t rowIndex = 0; rowIndex != getNmbRows(); ++rowIndex)
        {
            poolCapacity += m_assignedCapacity(rowIndex, poolIndex);
        }

        std::cout << " c= " << poolCapacity;
        size_t poolMin = std::numeric_limits<size_t>::max();
        for (size_t rowIndex = 0; rowIndex != getNmbRows(); ++rowIndex)
        {
            size_t currentMin = poolCapacity - m_assignedCapacity(rowIndex, poolIndex);
            if (currentMin < poolMin)
                poolMin = currentMin;
        }

        std::cout << " min= " << poolMin << std::endl;
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

size_t Solution::getCriticalServer() const
{
    size_t criticalRow = 0;
    size_t criticalPool = 0;

    size_t overallMin = std::numeric_limits<size_t>::max();
    for (size_t poolIndex = 0; poolIndex != getNmbPools(); ++poolIndex)
    {
        size_t poolCapacity = 0;
        for (size_t rowIndex = 0; rowIndex != getNmbRows(); ++rowIndex)
        {
            poolCapacity += m_assignedCapacity(rowIndex, poolIndex);
        }

        size_t criticalRowCandidate = 0;
        size_t poolMin = std::numeric_limits<size_t>::max();
        for (size_t rowIndex = 0; rowIndex != getNmbRows(); ++rowIndex)
        {
            size_t currentMin = poolCapacity - m_assignedCapacity(rowIndex, poolIndex);
            if (currentMin < poolMin)
            {
                poolMin = currentMin;
                criticalRowCandidate = rowIndex;
            }
        }

        if (poolMin < overallMin)
        {
            overallMin = poolMin;
            criticalRow = criticalRowCandidate;
            criticalPool = poolIndex;
        }
    }

    size_t smallestSize = std::numeric_limits<size_t>::max();
    size_t csIndex = 0;

    for (size_t s = 0; s != m_servers.size(); ++s)
    {
        const PlacedServer& ps = m_servers[s];
        const Server& rs = m_request->m_servers[s];
        if (ps.m_coord.m_row != criticalRow)
            continue;
        if (ps.m_poolIndex != criticalPool)
            continue;

        if (rs.m_size < smallestSize)
        {
            smallestSize = rs.m_size;
            csIndex = s;
        }
    }
    return csIndex;
}


size_t Solution::getCapacityRange() const
{
    size_t min = std::numeric_limits<size_t>::max();
    size_t max = 0;
    for (size_t poolIndex = 0; poolIndex != getNmbPools(); ++poolIndex)
    {
        size_t poolCapacity = 0;
        for (size_t rowIndex = 0; rowIndex != getNmbRows(); ++rowIndex)
        {
            poolCapacity += m_assignedCapacity(rowIndex, poolIndex);
        }

        if (poolCapacity < min)
            min = poolCapacity;

        if (poolCapacity > max)
            max = poolCapacity;
    }
    return min;
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
    PlacedServer& server = m_servers[a_serverIndex];
    if (server.m_poolIndex != std::numeric_limits<size_t>::max())
    {
        std::cerr << "Server already placed." << std::endl;
        throw "huh";
    }

    if (a_poolIndex == std::numeric_limits<size_t>::max())
        return;

    server.m_coord = a_coord;
    for (size_t i = 0; i < m_request->m_servers[a_serverIndex].m_size; ++i)
    {
        if (m_isAssigned(server.m_coord.m_row, server.m_coord.m_slot + i) == 1)
        {
            std::cerr << "invalid placement" << std::endl;
            throw "error";
        }
        m_isAssigned(server.m_coord.m_row, server.m_coord.m_slot + i) = 1;
    }
    server.m_poolIndex = a_poolIndex;
    m_assignedCapacity(server.m_coord.m_row, a_poolIndex) += m_request->m_servers[a_serverIndex].m_capacity;
}


void Solution::alignLeft(size_t a_serverIndex)
{
    PlacedServer old = m_servers[a_serverIndex];
    removeServer(a_serverIndex);

    Coordinate coord = old.m_coord;

    size_t slot = coord.m_slot;
    while (slot > 0 && m_isAssigned(coord.m_row, slot - 1) == 0)
    {
        --slot;
        std::cout << "left" << std::endl;
    }
    coord.m_slot = slot;

    placeServer(coord, a_serverIndex, old.m_poolIndex);
}


void Solution::removeServer(size_t a_serverIndex)
{
    const PlacedServer& server = m_servers[a_serverIndex];

    if (server.m_poolIndex == std::numeric_limits<size_t>::max())
        return;

    m_assignedCapacity(server.m_coord.m_row, server.m_poolIndex) -= m_request->m_servers[a_serverIndex].m_capacity;

    for (size_t i = 0; i < m_request->m_servers[a_serverIndex].m_size; ++i)
    {
        if (m_isAssigned(server.m_coord.m_row, server.m_coord.m_slot + i) == 0)
        {
            std::cerr << "expected unassigned slot" << std::endl;
            throw "error";
        }
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


bool Solution::canSwapServers(const size_t a_server1, const size_t a_server2)
{
    auto canMoveFromTo = [&] (const size_t a_from, const size_t a_to)
    {
        PlacedServer to = m_servers[a_to];
        removeServer(a_to);
        const bool result = isPlacable(to.m_coord, a_from);
        placeServer(to.m_coord, a_to, to.m_poolIndex);
        return result;
    };
    return canMoveFromTo(a_server1, a_server2) && canMoveFromTo(a_server2, a_server1);
}


void Solution::swapServers(size_t a_server1, size_t a_server2)
{
    PlacedServer placed1 = m_servers[a_server1];
    PlacedServer placed2 = m_servers[a_server2];
    removeServer(a_server1);
    removeServer(a_server2);
    placeServer(placed1.m_coord, a_server1, placed2.m_poolIndex);
    placeServer(placed2.m_coord, a_server2, placed1.m_poolIndex);
}


void Solution::printServerInfo(size_t serverIndex)
{
    std::cout << "Server " << serverIndex
              << " size = " << m_request->m_servers[serverIndex].m_size
              << " coords = " << m_servers[serverIndex].m_coord
              << " pool = " << m_servers[serverIndex].m_poolIndex
              << std::endl;
}


void Solution::printSolution() const
{
    base::Vector2d<size_t> draw = m_isAssigned;

    for (size_t s = 0; s != m_servers.size(); ++s)
    {
        for (size_t i = 0; i < m_request->m_servers[s].m_size; ++i)
        {
            draw(m_servers[s].m_coord.m_row, m_servers[s].m_coord.m_slot + i) = 2 + m_servers[s].m_poolIndex;
        }
    }

    for (size_t row = 0; row != getNmbRows(); ++row)
    {
        for (size_t slot = 0; slot != m_request->m_nmbSlots; ++slot)
        {
            if (draw(row, slot) == 0)
                std::cout << "O";
            else if (draw(row, slot) == 1)
                std::cout << "X";
            else if (draw(row, slot) > 1)
                std::cout << static_cast<char>('a' + (draw(row, slot)%26));
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
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

/*
class Move
{
    Move(size_t );

public:

};
*/

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
        size_t moveType = intDistribution(rndGenerator) % 6;
        if (moveType < 4)
        {
            size_t randomIndex1 = placedServerIndices[intDistribution(rndGenerator) % placedServerIndices.size()];
            if (moveType == 0)
                randomIndex1 =  s.getCriticalServer();

            PlacedServer old = s.m_servers[randomIndex1];
            size_t oldPoolIndex = s.m_servers[randomIndex1].m_poolIndex;
            size_t newPoolIndex = intDistribution(rndGenerator) % s.getNmbPools();

            s.removeServer(randomIndex1);
            s.placeServer(old.m_coord, randomIndex1, newPoolIndex);

            double currentRating = static_cast<double>(s.getRating()) * 10 + static_cast<double>(s.getCapacityRange());

            const double diff = previousRating - currentRating;
            const double p = realDistribution(rndGenerator);
            const double e = std::exp(-diff / currentTemperature);

            if (p > e)
            {
                s.removeServer(randomIndex1);
                s.placeServer(old.m_coord, randomIndex1, oldPoolIndex);
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
        }
        else
        {
            size_t randomIndex1 = intDistribution(rndGenerator) % s.m_servers.size();

            if (moveType % 2 == 0)
                randomIndex1 =  s.getCriticalServer();

            s.getCriticalServer();
            size_t randomIndex2 = intDistribution(rndGenerator) % s.m_servers.size();

            if (randomIndex1 == randomIndex2)
            {
                randomIndex2 = (randomIndex2 + 1) % s.m_servers.size();
            }

            if (!s.canSwapServers(randomIndex1, randomIndex2))
                continue;

            s.swapServers(randomIndex1, randomIndex2);
//            s.alignLeft(randomIndex1);
//            s.alignLeft(randomIndex2);
/*
            if (!s.canSwapServers(randomIndex2, randomIndex1))
            {
                s.printServerInfo(randomIndex1);
                s.printServerInfo(randomIndex2);
                std::cout << "huh?" << std::endl;
                throw "hu";
            }
*/
            double currentRating = static_cast<double>(s.getRating()) * 10 + static_cast<double>(s.getCapacityRange());

            const double diff = previousRating - currentRating;
            const double p = realDistribution(rndGenerator);
            const double e = std::exp(-diff / currentTemperature);

            if (p > e)
            {
                s.swapServers(randomIndex2, randomIndex1);
            }
            else
            {
                //std::cout << "swap move ok" << std::endl;
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
        }
        currentTemperature *= coolingFactor;
    }
    return bestSolution;
}
