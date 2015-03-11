#include "Canvas.h"
#include "Command.h"
#include "GreedySolver.h"
#include <iostream>
#include <fstream>
#include <limits>


Commands solveTrivially(const base::Vector2d<State>& a_vector)
{
    Commands result;
    for (size_t x = 0; x != a_vector.width(); ++x)
    {
        for (size_t y = 0; y !=a_vector.height(); ++y)
        {
            if (a_vector(x, y) == State::Black)
                result.emplace_back(Command::Type::PaintSQ, x, y, 0);
        }
    }
    return result;
}


int main (int , char *[])
{
    std::cout << "Hello World!" << std::endl;

    base::Vector2d<State> v = readFile("doodle.txt");

    Commands result = solveGreedily(v);
    //Commands result = solveTrivially(v);

    for (const auto& r : result)
        std::cout << r << std::endl;
    std::cout << "nmbOfCommands " << result.size() << "\n";

    verify(v, result);
}


