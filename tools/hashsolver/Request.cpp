#include "Request.h"
#include <sstream>
#include <iostream>
#include <fstream>


std::istream& operator>>(std::istream& is, Request& a_request)
{

    //std::stringstream s(parser.getLine());

    // R​(1 ≤ R ≤ 1000) ​denotes the number of rows in the data center,
    // S​(1 ≤ S ≤ 1000) denotes the number of slots in each row of the data center,
    // U​(0 ≤ U ≤ R × S) denotes the number of unavailable slots,
    // P​(1 ≤ P ≤ 1000) denotes the number of pools to be created,
    // M​(1 ≤ M ≤ R × S) denotes the number of servers to be allocated;

    size_t nmbUnavailable = 0;
    size_t nmbServers = 0;

    is >> a_request.m_nmbRows;
    std::cout << "rows: " << a_request.m_nmbRows << std::endl;
    is >> a_request.m_nmbColumns;
    is >> nmbUnavailable;
    is >> a_request.m_nmbPools;
    is >> nmbServers;

    a_request.m_unavailableSlots.reserve(nmbUnavailable);
    for (size_t i = 0; i != nmbUnavailable; ++i)
    {
        Coordinate c;
        is >> c;
        a_request.m_unavailableSlots.push_back(c);
    }

    a_request.m_servers.reserve(nmbServers);
    for (size_t i = 0; i != nmbServers; ++i)
    {
        Server s;
        is >> s;
        a_request.m_servers.push_back(s);
    }

    return is;
}




Request readFile(std::string filename)
{
    std::ifstream ifs(filename.c_str());
    if (!ifs)
        std::cout << "cannot open file" << std::endl;

    Request r;
    ifs >> r;
    return r;
}
