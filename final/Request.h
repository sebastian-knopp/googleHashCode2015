#pragma once
#include <istream>
#include <vector>


struct Coordinate
{
    size_t m_x;
    size_t m_y;
    size_t m_alt;
};


struct Vector
{
    size_t m_x;
    size_t m_y;
};


template<typename T>
struct Grid
{
    Grid(size_t a_width, size_t a_height)
    : m_data(a_width * a_height)
    , m_width(a_width)
    {
    }

    const T& operator() (size_t a_x, size_t a_y) const
    {
        return m_data[a_x + m_width * a_y];
    }

    T& operator() (size_t a_x, size_t a_y)
    {
        return m_data[a_x + m_width * a_y];
    }

private:
    std::vector<T> m_data;
    size_t m_width;
};


class Request {

public:

    Request();


public:

    size_t m_nmbRows = 0;
    size_t m_nmbColumns = 0;
    size_t m_nmbAltitudes = 0;

    size_t m_nmbTargetCells = 0;
    size_t m_coverageRadius = 0;
    size_t m_nmbBallons = 0;
    size_t m_nmbTurns = 0;

    Coordinate m_startCell = { 0, 0, 0 };

    std::vector<Coordinate> m_targetCells;

    std::vector<Grid<Vector>> m_windVectors;

};



std::istream& operator>>(std::istream& a_is, Request& a_request);

std::ostream& operator<<(std::ostream& a_os, const Request& a_request);


