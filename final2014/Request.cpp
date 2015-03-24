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


