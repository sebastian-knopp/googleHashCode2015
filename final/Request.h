#pragma once
#include <istream>
#include <vector>


struct Coordinate
{
    size_t m_column;
    size_t m_row;
    size_t m_alt;
};


struct Vector
{
    size_t m_rowDiff;
    size_t m_columnDiff;
};


template<typename T>
struct Grid
{
    Grid(size_t a_nmbRows, size_t a_nmbColumns)
    : m_data(a_nmbRows * a_nmbColumns)
    , m_nmbColumns(a_nmbColumns)
    {
    }

    const T& operator() (size_t a_row, size_t a_column) const
    {
        return m_data[a_row * m_nmbColumns + a_column];
    }

    T& operator() (size_t a_row, size_t a_column)
    {
        return m_data[a_row * m_nmbColumns + a_column];
    }


private:

    std::vector<T> m_data;
    size_t m_nmbColumns;
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


