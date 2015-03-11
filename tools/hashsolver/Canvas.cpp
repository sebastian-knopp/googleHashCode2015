#include "Canvas.h"
#include <fstream>


std::ostream&operator<<(std::ostream& os, State a_state)
{
    if (a_state == State::Black)
        os << "#";
    else
        os << ".";

    return os;
}


std::istream&operator>>(std::istream& is, State& a_state)
{
    char c;
    is >> c;
    if (c == '#')
        a_state = State::Black;
    else
        a_state = State::White;

    return is;
}


Canvas readFile(std::string filename)
{
    std::ifstream ifs(filename.c_str());
    if (!ifs)
        std::cout << "cannot open file" << std::endl;

    LineParser parser(ifs);

    std::stringstream s(parser.getLine());
    size_t nmbRows;
    s >> nmbRows;
    size_t nmbColumns;
    s >> nmbColumns;

    std::cout << "nmbRows: " << nmbRows << "\n";
    std::cout << "nmbColumns: " << nmbColumns << "\n";

    Canvas result(nmbRows, nmbColumns, State::White);

    for (size_t x = 0; x != result.width(); ++x)
    {
        std::stringstream s(parser.getLine());
        for (size_t y = 0; y != result.height(); ++y)
        {
            s >> result(x, y);
        }
    }
    return result;
}

