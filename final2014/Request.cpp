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
    size_t nmbStreets = 0;
    a_is >> nmbStreets;
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

    a_request.m_streets.reserve(nmbStreets);
    for (size_t i = 0; i != nmbStreets; ++i)
    {
        Street str;
        a_is >> str;
        a_request.m_streets.push_back(str);
    }

    return a_is;
}


std::ostream& operator<<(std::ostream& a_os, const Request& a_request)
{
    a_os << " " << a_request.m_junctions.size();
    a_os << " " << a_request.m_streets.size();
    a_os << " " << a_request.m_availableSecondsPerCar;
    a_os << " " << a_request.m_nmbCars;
    a_os << " " << a_request.m_initialJunctionIndex;
    a_os << std::endl;
    return a_os;
}


std::istream& operator>>(std::istream& a_is, Street& a_street)
{
    a_is >> a_street.m_junction1Index ;
    a_is >> a_street.m_junction2Index ;

    int x = 0;
    a_is >> x;
    a_street.m_isOneway = (x == 1);

    a_is >> a_street.m_cost ;
    a_is >> a_street.m_length;
    return a_is;
}


