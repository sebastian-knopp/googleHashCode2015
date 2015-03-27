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
    a_request.m_pizza.resize(nmbColumns, std::vector<Taste>(nmbRows, Taste::Ham));

    for (size_t r = 0; r < nmbRows; ++r)
    {
        for (size_t c = 0; c < nmbColumns; ++c)
        {
            char taste;
            a_is >> taste;
            if (taste=='H')
                a_request.m_pizza[c][r] = Taste::Ham;
            else
                a_request.m_pizza[c][r] = Taste::Other;
        }
    }
    return a_is;
}


std::ostream& operator<<(std::ostream& a_is, const Request& a_request)
{
    a_is << a_request.m_pizza.size();
    return a_is;
}

