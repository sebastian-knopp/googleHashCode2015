#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <random>
#include <future>
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



    auto startHeuristic = [&r] (int a_runIndex) {
        std::cout << "start " << a_runIndex << std::endl;

        std::mt19937 rndGenerator(a_runIndex);
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
                while (currentCoord.m_slot <= r.m_nmbSlots - server.m_size)
                {
                    if (s.isPlacable(currentCoord, serverIndex))
                    {
                        s.placeServer(currentCoord, serverIndex, s.getPoolWithMinCapacity(currentCoord.m_row));
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

        //        return simulatedAnnealing(rndGenerator, s, 5.0, 0.99, 1000, false);

        const Solution result = simulatedAnnealing(rndGenerator, s, 20.0, 0.999995, 10000000, false);
        std::cout << "end " << a_runIndex << " ("<< result.getRating() << ")" << std::endl;
        return result;
    };


    std::vector<std::future<Solution>> futures;
    for (size_t i = 0; i != 8; ++i)
    {
        futures.emplace_back(std::async(std::launch::async, startHeuristic, i));
    }

    Solution bestSolution(r);
    size_t bestRating = bestSolution.getRating();
    for (auto& f : futures)
    {
        Solution s = f.get();
        size_t rating = s.getRating();
        if (rating > bestRating)
        {
            bestSolution = s;
            bestRating = rating;
            std::cout << "new best rating : "<< bestRating << std::endl;
        }
    }

    std::cout << "final rating : " << bestSolution.getRating() << std::endl;
//    std::mt19937 rndGenerator;

//    s = simulatedAnnealing(rndGenerator, s, 5.0, 0.999995, 50000000, true);
    /*


    std::cout << "best rating : " << bestSolution.getRating() << std::endl;
*/
    //s.printSolution();


    std::ofstream ofs("D:\\googleHash\\out.txt");
    ofs << bestSolution;
}




