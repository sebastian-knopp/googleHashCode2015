#include "Request.h"
#include <iostream>


double Coordinate::getLength() const
{
    const double squaredSum = m_lat * m_lat + m_long * m_long;
    if (squaredSum <= 0l)
        return 0l;

    return std::sqrt(squaredSum);
}


double getDistance(const Coordinate& a_first, const Coordinate& a_second)
{
    const Coordinate diff = { a_first.m_lat - a_second.m_lat,
                              a_first.m_long - a_second.m_long };
    return diff.getLength();
}


double getAngle(const Coordinate& a_origin, const Coordinate& a_coord)
{
    return std::atan2(a_origin.m_lat - a_coord.m_lat, a_origin.m_long - a_coord.m_long);
}


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


void Request::computeAdjacentStreetIndices()
{
    m_adjacentStreetIndices.resize(m_junctions.size());

    for (size_t i = 0; i != m_streets.size(); ++i)
    {
        const Street& str = m_streets[i];
        m_adjacentStreetIndices[str.m_junction1Index].push_back(i);

        if (!str.m_isOneway)
            m_adjacentStreetIndices[str.m_junction2Index].push_back(i);
    }
}


Coordinate Request::getOrigin() const
{
    return m_junctions[m_initialJunctionIndex];
}


std::ostream& operator<<(std::ostream& a_os, const Coordinate& a_coord)
{
    a_os << "(" << a_coord.m_lat << ", " << a_coord.m_long << ")";
    return a_os;
}


size_t Street::getOppositeJunction(const size_t a_junctionIndex) const
{
    if (m_junction1Index == a_junctionIndex)
        return m_junction2Index;

    if (m_junction2Index == a_junctionIndex)
        return m_junction1Index;

    std::cout << "getOppositeJunction failed (" << a_junctionIndex << ")" << std::endl;
    std::cout << "J 1 " << m_junction1Index << std::endl;
    std::cout << "J 2 " << m_junction2Index << std::endl;
    exit(1);
}
