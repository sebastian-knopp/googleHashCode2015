#pragma once
#include <istream>

class Request {

public:

    Request()
    {
    }


public:

    size_t m_nmbJunctions = 0; // denotes the number of junctions in the city
    size_t m_nmbStreets = 0; // denotes the number of streets in the city
    size_t m_availableSecondsPerCar = 0; // denotes the virtual time in seconds allowed for the car itineraries
    size_t m_nmbCars = 0; // denotes the number of cars in the fleet
    size_t m_initialJunctionIndex = 0; // denotes the junction at which all the cars are located initially

};


std::istream& operator>>(std::istream& a_is, Request& a_request);

std::ostream& operator<<(std::ostream& a_is, const Request& a_request);

