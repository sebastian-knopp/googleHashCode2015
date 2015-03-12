#pragma once
#include <stdint.h>
#include <vector>
#include <istream>


class Coordinate {

public:

    Coordinate()
    {
    }

public:

    size_t m_row = 0;
    size_t m_column = 0;
};

typedef std::vector<Coordinate> Coordinates;


std::istream& operator>>(std::istream& is, Coordinate& a_coordinate);

std::ostream& operator<<(std::ostream& is, const Coordinate& a_server);

