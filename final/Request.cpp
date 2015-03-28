#include "Request.h"


Request::Request()
{
}


std::istream& operator>>(std::istream& a_is, Request& a_request)
{
    a_is >> a_request.m_nmbRows;
    a_is >> a_request.m_nmbColumns;
    a_is >> a_request.m_nmbAltitudes;

    a_is >> a_request.m_nmbTargetCells;
    a_is >> a_request.m_coverageRadius;
    a_is >> a_request.m_nmbBallons;
    a_is >> a_request.m_nmbTurns;

    a_is >> a_request.m_startCell.m_row;
    a_is >> a_request.m_startCell.m_column;

    a_request.m_targetCells.reserve(a_request.m_nmbTargetCells);
    for (size_t t = 0; t != a_request.m_nmbTargetCells; ++t)
    {
        Coordinate coord = {0, 0, 0};
        a_is >> coord.m_row;
        a_is >> coord.m_column;
        a_request.m_targetCells.push_back(coord);
    }

    a_request.m_windVectors.resize(a_request.m_nmbAltitudes,
                                   Grid<Vector>(a_request.m_nmbRows, a_request.m_nmbColumns));

    for (size_t a = 0; a != a_request.m_nmbAltitudes; ++a)
    {
        for (size_t r = 0; r != a_request.m_nmbRows; ++r)
        {
            for (size_t c = 0; c != a_request.m_nmbColumns; ++c)
            {
                a_is >> a_request.m_windVectors[a](r, c).m_rowDiff;
                a_is >> a_request.m_windVectors[a](r, c).m_columnDiff;
                ;
            }
        }
    }

    return a_is;
}


std::ostream& operator<<(std::ostream& a_os, const Request& /*a_request*/)
{
    //a_os << a_request.m_test;
    return a_os;
}

