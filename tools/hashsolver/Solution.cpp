#include "Solution.h"
#include <iostream>


std::ostream& operator<<(std::ostream& is, const Solution& a_solution)
{
    for (const PlacedServer& s : a_solution.m_servers)
    {
        is << s;
    }
    return is;
}


