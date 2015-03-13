#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <random>
//#include <boost/random/uniform_int_distribution.hpp>
//#include <boost/random/uniform_real_distribution.hpp>
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


    std::mt19937 rndGenerator;

    Solution bestSolution(r);
    size_t bestRating = bestSolution.getRating();

    for (size_t i = 0; i != 200; ++i)
    {
        Solution s(r);

        std::vector<size_t> serverPermutation;
        serverPermutation.reserve(r.m_servers.size());
        for (size_t i = 0; i != r.m_servers.size(); ++i)
        {
            serverPermutation.push_back(i);
        }
        std::shuffle(begin(serverPermutation), end(serverPermutation), rndGenerator);

        std::stable_sort(begin(serverPermutation), end(serverPermutation), [&r] (size_t lhs, size_t rhs){
           return r.m_servers[lhs].getRatio() > r.m_servers[rhs].getRatio();
        });

        std::vector<size_t> rowPermutation;
        for (size_t i = 0; i != r.m_nmbRows; ++i)
        {
            rowPermutation.push_back(i);
        }
        std::shuffle(begin(rowPermutation), end(rowPermutation), rndGenerator);

        auto placeServer = [&] (size_t serverIndex) {
            const Server& server = r.m_servers[serverIndex];
            Coordinate currentCoord;
            size_t rowIndex = 0;
            while (rowIndex < r.m_nmbRows)
            {
                currentCoord.m_row = rowPermutation[rowIndex];
                currentCoord.m_slot = 0;
                while (currentCoord.m_slot < r.m_nmbSlots - server.m_size)
                {
                    if (s.isPlacable(currentCoord, serverIndex))
                    {
                        s.placeServer(currentCoord, serverIndex);
                        size_t currentPool = s.getPoolWithMinCapacity(currentCoord.m_row);
                        s.m_servers[serverIndex].m_coord.m_row = currentCoord.m_row;
                        s.m_servers[serverIndex].m_coord.m_slot = currentCoord.m_slot;
                        s.m_servers[serverIndex].m_poolIndex = currentPool;
                        s.m_assignedCapacity(currentCoord.m_row, currentPool) += server.m_capacity;
                        return;
                    }
                    ++currentCoord.m_slot;
                }
                ++rowIndex;
            }
        };

        for (size_t serverIndex : serverPermutation)
        {
            placeServer(serverIndex);
        }

        s = simulatedAnnealing(rndGenerator, s, 20.0, 0.9, 100, false);

        size_t rating = s.getRating();
        if (rating > bestRating)
        {
            bestSolution = s;
            bestRating = rating;
            std::cout << "[initial] new best rating : "<< bestRating << std::endl;
        }

    }

    Solution s = bestSolution;

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

    //s.m_servers

    bestSolution = simulatedAnnealing(rndGenerator, bestSolution, 50.0, 0.9999, 1000000, true);
    //for ();

    std::ofstream ofs("D:\\googleHash\\out.txt");
    ofs << s;
}




