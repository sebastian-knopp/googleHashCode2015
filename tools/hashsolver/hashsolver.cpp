#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include "Request.h"
#include "Solution.h"


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
       return r.m_servers[lhs].getRatio() < r.m_servers[rhs].getRatio();
    });

    for (size_t serverIndex : serverPermutation)
    {
        std::cout << r.m_servers[serverIndex] << " r=" << r.m_servers[serverIndex].getRatio() << std::endl;
    }

    Solution s;

    std::ofstream ofs("out.txt");

    s.m_servers.resize(r.m_servers.size());
    ofs << s;
}




