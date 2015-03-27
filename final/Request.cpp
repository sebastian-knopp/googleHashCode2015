#include "Request.h"


Request::Request()
{
}


std::istream& operator>>(std::istream& a_is, Request& a_request)
{
    size_t nmbRows = 0;
    size_t nmbColumns = 0;
    a_is >> nmbRows;
    a_is >> nmbColumns;
    a_is >> a_request.minNmbHam;
    a_is >> a_request.maxNmbCells;

    a_request.m_pizza.resize(nmbColumns, std::vector<Taste>(nmbRows, Taste::Ham));

    for (size_t r = 0; r < nmbRows; ++r)
    {
        for (size_t c = 0; c < nmbColumns; ++c)
        {
            char taste;
            a_is >> taste;
            if (taste == 'H')
                a_request.m_pizza[c][r] = Taste::Ham;
            else
                a_request.m_pizza[c][r] = Taste::Other;
        }
    }
    return a_is;
}


std::ostream& operator<<(std::ostream& a_os, const Request& a_request)
{
    for (size_t r = 0; r < a_request.getNmbRows(); ++r)
    {
        for (size_t c = 0; c < a_request.getNmbColumns(); ++c)
        {
            a_os << a_request.m_pizza[c][r];
        }
        a_os << "\n";
    }
    a_os << std::flush;
    return a_os;
}


std::ostream& operator<<(std::ostream& a_os, const Taste& a_taste)
{
    if (a_taste == Taste::Ham)
        a_os << 'H';
    else
        a_os << 'T';

    return a_os;
}
