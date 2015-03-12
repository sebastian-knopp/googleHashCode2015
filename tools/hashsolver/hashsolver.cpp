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

    size_t currentPool = 0;
    auto placeServer = [&] (size_t serverIndex) {
        const Server& server = r.m_servers[serverIndex];
        size_t currentRow = 0;
        while (currentRow < r.m_nmbRows)
        {
            size_t currentSlot = 0;
            while (currentSlot < r.m_nmbSlots)
            {
                if (isAvailable(currentRow, currentSlot, server.m_size))
                {
                    place(currentRow, currentSlot, server.m_size);
                    s.m_servers[serverIndex].m_coord.m_row = currentRow;
                    s.m_servers[serverIndex].m_coord.m_slot = currentSlot;
                    s.m_servers[serverIndex].m_poolIndex = currentPool;
                    ++currentPool;
                    currentPool %= r.m_nmbPools;
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

    //for ();

    std::ofstream ofs("out.txt");
    ofs << s;
}




