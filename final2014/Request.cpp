#include "Request.h"



std::istream& operator>>(std::istream& a_is, Request& a_request)
{
    a_is >> a_request.m_nmbJunctions;
    a_is >> a_request.m_nmbStreets;
    a_is >> a_request.m_availableSecondsPerCar;
    a_is >> a_request.m_nmbCars;
    a_is >> a_request.m_initialJunctionIndex;
    return a_is;
}


std::ostream& operator<<(std::ostream& a_os, const Request& a_request)
{
    a_os << " " << a_request.m_nmbJunctions;
    a_os << " " << a_request.m_nmbStreets;
    a_os << " " << a_request.m_availableSecondsPerCar;
    a_os << " " << a_request.m_nmbCars;
    a_os << " " << a_request.m_initialJunctionIndex;
    a_os << std::endl;
    return a_os;
}

