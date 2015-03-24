#pragma once
#include <istream>
#include <vector>


struct Coordinate
{
    double m_lat;
    double m_long;
};

typedef std::vector<Coordinate> Coordinates;

std::istream& operator>>(std::istream& a_is, Coordinate& a_coord);


class Request {

public:

    Request()
    {
    }


public:

    size_t m_nmbStreets = 0; // denotes the number of streets in the city
    size_t m_availableSecondsPerCar = 0; // denotes the virtual time in seconds allowed for the car itineraries
    size_t m_nmbCars = 0; // denotes the number of cars in the fleet
    size_t m_initialJunctionIndex = 0; // denotes the junction at which all the cars are located initially

    Coordinates m_junctions;

};

std::istream& operator>>(std::istream& a_is, Request& a_request);

std::ostream& operator<<(std::ostream& a_is, const Request& a_request);

