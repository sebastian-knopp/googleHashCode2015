#pragma once
#include <istream>
#include <vector>
#include <cmath>


struct Coordinate
{
    double m_lat;
    double m_long;

    //! Returns the length when interpreting this as a vector
    double getLength() const;

};

double getAngle(const Coordinate& a_first, const Coordinate& a_second);
double getDistance(const Coordinate& a_first, const Coordinate& a_second);
constexpr double pi() { return std::atan(1)*4; }


typedef std::vector<Coordinate> Coordinates;

std::ostream& operator<<(std::ostream& a_os, const Coordinate& a_coord);
std::istream& operator>>(std::istream& a_is, Coordinate& a_coord);


struct Street
{
    size_t m_junction1Index = 0;
    size_t m_junction2Index = 0;
    bool m_isOneway = false;
    int m_cost = 0;
    int m_length = 0;
};

typedef std::vector<Street> Streets;

std::istream& operator>>(std::istream& a_is, Street& a_street);

class Request {

public:

    Request()
    {
    }

    void computeAdjacentStreetIndices();


public:

    // data read from file
    size_t m_availableSecondsPerCar = 0; // denotes the virtual time in seconds allowed for the car itineraries
    size_t m_nmbCars = 0; // denotes the number of cars in the fleet
    size_t m_initialJunctionIndex = 0; // denotes the junction at which all the cars are located initially
    Coordinates m_junctions;
    Streets m_streets;

    // preprocessed data
    std::vector<std::vector<size_t>> m_adjacentStreetIndices;

};

std::istream& operator>>(std::istream& a_is, Request& a_request);

std::ostream& operator<<(std::ostream& a_is, const Request& a_request);



