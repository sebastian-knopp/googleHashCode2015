#pragma once
#include "base/Vector2d.h"
#include "LineParser.h"
#include <iostream>
#include <sstream>
#include <iomanip>


enum class State
{
    Black,
    White
};


typedef base::Vector2d<State> Canvas;


std::ostream& operator<<(std::ostream& os, State a_state);


template<typename T>
std::ostream& operator<<(std::ostream& os, const base::Vector2d<T>& a_vector)
{
    for (size_t x = 0; x != a_vector.width(); ++x)
    {
        for (size_t y = 0; y != a_vector.height(); ++y)
        {
            os << std::right << std::setw(4) << a_vector(x, y);
        }
        os << "\n";
    }
    return os;
}


std::istream& operator>>(std::istream& is, State& a_state);


Canvas readFile(std::string filename);


