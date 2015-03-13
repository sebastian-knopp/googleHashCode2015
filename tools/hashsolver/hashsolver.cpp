#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include "Request.h"
#include "Solution.h"
#include "base/Vector2d.h"


int main (int , char *[])
{
    std::cout << "Google Hash 2015" << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "Team A211: Abdel Bitar and Sebastian Knopp" << std::endl;


    Request r = readFile("dc.in");
/*
    std::cout << "read file" << std::endl;
    std::cout << "c = " << r.m_nmbColumns << std::endl;
    std::cout << "r = " << r.m_nmbRows << std::endl;
    std::cout << "p = " << r.m_nmbPools << std::endl;
    std::cout << "s = " << r.m_servers.size() << std::endl;

    for (const Server& s : r.m_servers)
    {
        std::cout << s << std::endl;
    }

    for (const Coordinate& c : r.m_unavailableSlots)
    {
        std::cout << c << std::endl;
    }

    std::cout << "u = " << r.m_unavailableSlots.size() << std::endl;
*/

    std::vector<size_t> serverPermutation;
    serverPermutation.reserve(r.m_servers.size());
    for (size_t i = 0; i != r.m_servers.size(); ++i)
    {
        serverPermutation.push_back(i);
    }

    std::sort(begin(serverPermutation), end(serverPermutation), [&r] (size_t lhs, size_t rhs){
       return r.m_servers[lhs].getRatio() > r.m_servers[rhs].getRatio();
    });

    Solution s(r.m_nmbRows, r.m_nmbPools);
    s.m_servers.resize(r.m_servers.size());

/*
    for (size_t serverIndex : serverPermutation)
    {
        std::cout << r.m_servers[serverIndex] << " r=" << r.m_servers[serverIndex].getRatio() << std::endl;
    }
*/

    // capacity of a pool in a row

    auto getPoolCapacity = [&] (size_t a_pool)
    {
        size_t nmbRows = s.m_assignedCapacity.width();
        size_t c = 0;
        for (size_t i = 0; i < nmbRows; ++i)
        {
            c += s.m_assignedCapacity(i, a_pool);
        }
        return c;
    };

    auto getPoolWithMinCapacity = [&] (size_t a_row)
    {
        size_t bestPool = 0;
        size_t min = std::numeric_limits<size_t>::max();
        size_t nmbPools = s.m_assignedCapacity.height();
        for (size_t i = 0; i < nmbPools; ++i)
        {
            size_t poolCap = getPoolCapacity(i) + s.m_assignedCapacity(a_row, i);
            if (poolCap < min)
            {
                min = poolCap;
                bestPool = i;
            }
        }
        return bestPool;
    };

    base::Vector2d<size_t> isAssigned(r.m_nmbRows, r.m_nmbSlots, 0);
    for (const Coordinate& c : r.m_unavailableSlots)
    {
        isAssigned(c.m_row, c.m_slot) = 1;
    }

    auto isAvailable = [&] (const Coordinate& a_coord, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (isAssigned(a_coord.m_row, a_coord.m_slot + i) == 1)
                return false;
        }
        return true;
    };

    auto place = [&] (size_t row, size_t slot, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (isAssigned(row, slot + i) == 1)
                throw "error";
            isAssigned(row, slot + i) = 1;
        }
    };

    auto placeServer = [&] (size_t serverIndex) {
        const Server& server = r.m_servers[serverIndex];
        Coordinate currentCoord;
        while (currentCoord.m_row < r.m_nmbRows)
        {
            currentCoord.m_slot = 0;
            while (currentCoord.m_slot < r.m_nmbSlots - server.m_size)
            {
                if (isAvailable(currentCoord, server.m_size))
                {
                    place(currentCoord.m_row, currentCoord.m_slot, server.m_size);
                    size_t currentPool = getPoolWithMinCapacity(currentCoord.m_row);
                    s.m_servers[serverIndex].m_coord.m_row = currentCoord.m_row;
                    s.m_servers[serverIndex].m_coord.m_slot = currentCoord.m_slot;
                    s.m_servers[serverIndex].m_poolIndex = currentPool;
                    s.m_assignedCapacity(currentCoord.m_row, currentPool) += server.m_capacity;
                    return;
                }
                ++currentCoord.m_slot;
            }
            ++currentCoord.m_row;
        }
    };

    for (size_t serverIndex : serverPermutation)
    {
        placeServer(serverIndex);
    }

    boost::mt19937 rndGenerator;
    boost::random::uniform_int_distribution<> intDistribution(0, std::numeric_limits<int>::max());
    boost::random::uniform_real_distribution<> realDistribution(0, 1.0);

    std::vector<size_t> placedServerIndices;
    for (size_t i = 0; i != r.m_servers.size(); ++i)
    {
        if (s.m_servers[i].isPlaced())
            placedServerIndices.push_back(i);
    }

    Solution bestSolution = s;
    size_t bestRating = s.getRating();
    double previousRating = static_cast<double>(bestRating);
    std::cout << "initial rating : " << bestRating << std::endl;

/*
    std::vector<size_t> trueServerSize(r.m_servers.size(), 0);
    for (size_t i = 0; i != r.m_servers.size(); ++i)
    {
        trueServerSize[i] = r.m_servers[i].m_size;
        size_t slotIndex = s.m_servers[i].m_coord.m_slot + r.m_servers[i].m_size;
        while (slotIndex < r.m_nmbSlots)
        {
            if (isAssigned(s.m_servers[i].m_coord.m_row, slotIndex))
            {
                trueServerSize[i] = slotIndex - s.m_servers[i].m_coord.m_slot;
                break;
            }

            ++slotIndex;
        }
        trueServerSize[i] = r.m_nmbSlots - s.m_servers[i].m_coord.m_slot;
    }

    auto canSwap = [&] (size_t server1, size_t server2) {
        return r.m_servers[server1].m_size <= trueServerSize[server2] &&
               r.m_servers[server2].m_size <= trueServerSize[server1];
    };
*/
    double currentTemperature = 50.0;
    for (size_t i = 0; i != 1000000; ++i)
    {
        size_t randomIndex1 = placedServerIndices[intDistribution(rndGenerator) % placedServerIndices.size()];
        {
            size_t oldPoolIndex = s.m_servers[randomIndex1].m_poolIndex;
            size_t newPoolIndex = intDistribution(rndGenerator) % r.m_nmbPools;

            s.m_servers[randomIndex1].m_poolIndex = newPoolIndex;
            s.m_assignedCapacity(s.m_servers[randomIndex1].m_coord.m_row, oldPoolIndex) -= r.m_servers[randomIndex1].m_capacity;
            s.m_assignedCapacity(s.m_servers[randomIndex1].m_coord.m_row, newPoolIndex) += r.m_servers[randomIndex1].m_capacity;

            double currentRating = static_cast<double>(s.getRating());

            const double diff = previousRating - currentRating;
            const double p = realDistribution(rndGenerator);
            const double e = std::exp(-diff / currentTemperature);

            if (p > e)
            {
                s.m_servers[randomIndex1].m_poolIndex = oldPoolIndex;
                s.m_assignedCapacity(s.m_servers[randomIndex1].m_coord.m_row, oldPoolIndex) += r.m_servers[randomIndex1].m_capacity;
                s.m_assignedCapacity(s.m_servers[randomIndex1].m_coord.m_row, newPoolIndex) -= r.m_servers[randomIndex1].m_capacity;
            }
            else
            {
                previousRating = currentRating;
            }
        }

        if (previousRating > bestRating)
        {
            bestSolution = s;
            bestRating = previousRating;
            std::cout << "new best rating : "<< bestRating << std::endl;
            std::cout << "i: "<< i << std::endl;
        }

        currentTemperature *= 0.9;
    }

    //s.m_servers


    //for ();

    std::ofstream ofs("D:\\googleHash\\out.txt");
    ofs << s;
}




