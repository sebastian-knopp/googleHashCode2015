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
/*
    for (size_t serverIndex : serverPermutation)
    {
        std::cout << r.m_servers[serverIndex] << " r=" << r.m_servers[serverIndex].getRatio() << std::endl;
    }
*/

    // capacity of a pool in a row
    base::Vector2d<size_t> assignedCapacity(r.m_nmbRows, r.m_nmbPools, 0);

    auto getPoolCapacity = [&] (size_t a_pool)
    {
        size_t nmbRows = assignedCapacity.width();
        size_t c = 0;
        for (size_t i = 0; i < nmbRows; ++i)
        {
            c += assignedCapacity(i, a_pool);
        }
        return c;
    };

    auto getPoolWithMinCapacity = [&] (size_t a_row)
    {
        size_t bestPool = 0;
        size_t min = std::numeric_limits<size_t>::max();
        size_t nmbPools = assignedCapacity.height();
        for (size_t i = 0; i < nmbPools; ++i)
        {
            size_t poolCap = getPoolCapacity(i) + assignedCapacity(a_row, i);
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

    auto isAvailable = [&] (size_t row, size_t slot, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (isAssigned(row, slot + i) == 1)
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

    Solution s;
    s.m_servers.resize(r.m_servers.size());

    auto placeServer = [&] (size_t serverIndex) {
        const Server& server = r.m_servers[serverIndex];
        size_t currentRow = 0;
        while (currentRow < r.m_nmbRows)
        {
            size_t currentSlot = 0;
            while (currentSlot < r.m_nmbSlots - server.m_size)
            {
                if (isAvailable(currentRow, currentSlot, server.m_size))
                {
                    place(currentRow, currentSlot, server.m_size);
                    size_t currentPool = getPoolWithMinCapacity(currentRow);
                    s.m_servers[serverIndex].m_coord.m_row = currentRow;
                    s.m_servers[serverIndex].m_coord.m_slot = currentSlot;
                    s.m_servers[serverIndex].m_poolIndex = currentPool;
                    assignedCapacity(currentRow, currentPool) += server.m_capacity;
                    return;
                }
                ++currentSlot;
            }
            ++currentRow;
        }
    };

    for (size_t serverIndex : serverPermutation)
    {
        placeServer(serverIndex);
    }

    auto getRating = [&] ()
    {
        size_t overallMin = std::numeric_limits<size_t>::max();
        for (size_t poolIndex = 0; poolIndex != r.m_nmbPools; ++poolIndex)
        {
            size_t poolCapacity = 0;
            for (size_t rowIndex = 0; rowIndex != r.m_nmbRows; ++rowIndex)
            {
                poolCapacity += assignedCapacity(rowIndex, poolIndex);
            }

            size_t poolMin = std::numeric_limits<size_t>::max();
            for (size_t rowIndex = 0; rowIndex != r.m_nmbRows; ++rowIndex)
            {
                size_t currentMin = poolCapacity - assignedCapacity(rowIndex, poolIndex);
                if (currentMin < poolMin)
                    poolMin = currentMin;
            }

            if (poolMin < overallMin)
                overallMin = poolMin;
        }
        return overallMin;
    };

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
    size_t bestRating = getRating();
    double previousRating = static_cast<double>(bestRating);
    std::cout << "initial rating : " << bestRating << std::endl;


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
/*
    auto canSwap = [&] (size_t server1, size_t server2) {
        return r.m_servers[server1].m_size <= trueServerSize[server2] &&
               r.m_servers[server2].m_size <= trueServerSize[server1];
    };
*/
    double currentTemperature = 25.0;
    for (size_t i = 0; i != 1000000; ++i)
    {
        size_t randomIndex = placedServerIndices[intDistribution(rndGenerator) % placedServerIndices.size()];

        size_t oldPoolIndex = s.m_servers[randomIndex].m_poolIndex;
        size_t newPoolIndex = intDistribution(rndGenerator) % r.m_nmbPools;

        size_t moveType = intDistribution(rndGenerator) % 2;
        if (moveType == 0)
        {
            s.m_servers[randomIndex].m_poolIndex = newPoolIndex;
            assignedCapacity(s.m_servers[randomIndex].m_coord.m_row, oldPoolIndex) -= r.m_servers[randomIndex].m_capacity;
            assignedCapacity(s.m_servers[randomIndex].m_coord.m_row, newPoolIndex) += r.m_servers[randomIndex].m_capacity;

            double currentRating = static_cast<double>(getRating());

            const double diff = previousRating - currentRating;
            const double p = realDistribution(rndGenerator);
            const double e = std::exp(-diff / currentTemperature);

            if (p > e)
            {
                s.m_servers[randomIndex].m_poolIndex = oldPoolIndex;
                assignedCapacity(s.m_servers[randomIndex].m_coord.m_row, oldPoolIndex) += r.m_servers[randomIndex].m_capacity;
                assignedCapacity(s.m_servers[randomIndex].m_coord.m_row, newPoolIndex) -= r.m_servers[randomIndex].m_capacity;
            }
            else
            {
                previousRating = currentRating;
            }
        }
        else
        {
            s.m_servers[randomIndex].m_poolIndex = newPoolIndex;
            assignedCapacity(s.m_servers[randomIndex].m_coord.m_row, oldPoolIndex) -= r.m_servers[randomIndex].m_capacity;
            assignedCapacity(s.m_servers[randomIndex].m_coord.m_row, newPoolIndex) += r.m_servers[randomIndex].m_capacity;

            double currentRating = static_cast<double>(getRating());

            const double diff = previousRating - currentRating;
            const double p = realDistribution(rndGenerator);
            const double e = std::exp(-diff / currentTemperature);

            if (p > e)
            {
                s.m_servers[randomIndex].m_poolIndex = oldPoolIndex;
                assignedCapacity(s.m_servers[randomIndex].m_coord.m_row, oldPoolIndex) += r.m_servers[randomIndex].m_capacity;
                assignedCapacity(s.m_servers[randomIndex].m_coord.m_row, newPoolIndex) -= r.m_servers[randomIndex].m_capacity;
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
            std::cout << "new best rating : " << bestRating << std::endl;
        }

        currentTemperature *= 0.999;
    }

    //s.m_servers


    //for ();

    std::ofstream ofs("D:\\googleHash\\out.txt");
    ofs << s;
}




