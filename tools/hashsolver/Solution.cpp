#include "Solution.h"
#include <iostream>


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


