#include "Request.h"


std::istream& operator>>(std::istream& a_is, Coordinate& a_coord)
{
    a_is >> a_coord.m_lat;
    a_is >> a_coord.m_long;
    return a_is;
}


std::istream& operator>>(std::istream& a_is, Request& a_request)
{
    size_t nmbJunctions = 0;
    a_is >> nmbJunctions;
    a_is >> a_request.m_nmbStreets;
    a_is >> a_request.m_availableSecondsPerCar;
    a_is >> a_request.m_nmbCars;
    a_is >> a_request.m_initialJunctionIndex;

    a_request.m_junctions.reserve(nmbJunctions);
    for (size_t i = 0; i != nmbJunctions; ++i)
    {
        Coordinate junction;
        a_is >> junction;
        a_request.m_junctions.push_back(junction);
    }

    return a_is;
}


std::ostream& operator<<(std::ostream& a_os, const Request& a_request)
{
    a_os << " " << a_request.m_junctions.size();
    a_os << " " << a_request.m_nmbStreets;
    a_os << " " << a_request.m_availableSecondsPerCar;
    a_os << " " << a_request.m_nmbCars;
    a_os << " " << a_request.m_initialJunctionIndex;
    a_os << std::endl;
    return a_os;
}


