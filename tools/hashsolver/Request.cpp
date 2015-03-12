#include "Request.h"
#include "LineParser.h"
#include <sstream>


std::istream& operator>>(std::istream& is, Request& /*a_request*/)
{
    LineParser parser(is);

    std::stringstream s(parser.getLine());

    // R​(1 ≤ R ≤ 1000) ​denotes the number of rows in the data center,
    // S​(1 ≤ S ≤ 1000) denotes the number of slots in each row of the data center,
    // U​(0 ≤ U ≤ R × S) denotes the number of unavailable slots,
    // P​(1 ≤ P ≤ 1000) denotes the number of pools to be created,
    // M​(1 ≤ M ≤ R × S) denotes the number of servers to be allocated;

    size_t nmbRows = 0;
    size_t nmbColumns = 0;
    size_t nmbUnavailable = 0;
    size_t nmbPools = 0;
    size_t nmbServers = 0;

    is >> nmbRows;
    is >> nmbColumns;
    is >> nmbUnavailable;
    is >> nmbPools;
    is >> nmbServers;

    return is;
}


