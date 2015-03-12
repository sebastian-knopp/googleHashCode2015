#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
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

    std::cout << "rating : " << getRating();
    //for ();

    std::ofstream ofs("D:\\googleHash\\out.txt");
    ofs << s;
}




