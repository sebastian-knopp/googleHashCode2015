#pragma once
#include <istream>
#include <vector>


struct Coordinate
{
    inline bool operator== (const Coordinate& a_other) const
    {
        return m_column == a_other.m_column &&
               m_row == a_other.m_row &&
               m_alt == a_other.m_alt;
    }

    int m_column;
    int m_row;
    int m_alt;
};


struct Vector
{
    int m_rowDiff;
    int m_columnDiff;
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

    int m_nmbRows = 0;
    int m_nmbColumns = 0;
    int m_nmbAltitudes = 0;

    int m_nmbTargetCells = 0;
    int m_coverageRadius = 0;
    int m_nmbBallons = 0;
    int m_nmbTurns = 0;

    Coordinate m_startCell = { 0, 0, 0 };

    std::vector<Coordinate> m_targetCells;

    std::vector<Grid<Vector>> m_windVectors;

};



std::istream& operator>>(std::istream& a_is, Request& a_request);

std::ostream& operator<<(std::ostream& a_os, const Request& a_request);


