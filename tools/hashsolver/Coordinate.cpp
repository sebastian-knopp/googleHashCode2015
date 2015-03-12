#include "Coordinate.h"





std::istream&operator>>(std::istream& is, Coordinate& a_coordinate)
{
    is >> a_coordinate.m_row;
    is >> a_coordinate.m_column;
    return is;
}



std::ostream&operator<<(std::ostream& is, const Coordinate& a_server)
{
    is << "(row = " << a_server.m_row << ", column = " << a_server.m_column << ")";
    return is;
}
