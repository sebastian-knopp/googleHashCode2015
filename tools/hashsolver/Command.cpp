#include "Command.h"


std::ostream&operator<<(std::ostream& os, const Command& a_command)
{
    if (a_command.m_type == Command::Type::PaintSQ)
        os << "PAINTSQ " << a_command.m_x << " " << a_command.m_y << " " << a_command.m_s;
    else
        os << "ERASECELL " << a_command.m_x << " " << a_command.m_y;

    return os;
}


void verify(const base::Vector2d<State>& a_vector, Commands& a_commands)
{
    base::Vector2d<State> matrix(a_vector.width(), a_vector.height(), State::White);
    for (const Command& c : a_commands)
    {
        c.execute(matrix);
    }

    if (matrix != a_vector)
        std::cout << "*** Not verified! ***" << std::endl;
    else
        std::cout << "verified.\n" ;
}


Command::Command(Command::Type a_type, size_t a_x, size_t a_y, size_t a_s)
    : m_type(a_type)
    , m_x(a_x)
    , m_y(a_y)
    , m_s(a_s)
{
}


void Command::execute(base::Vector2d<State>& a_vector) const
{
    if (m_type == Type::EraseCell)
    {
        a_vector(m_x, m_y) = State::White;
        return;
    }

    for (size_t x = m_x - m_s; x <= m_x + m_s; ++x)
    {
        for (size_t y = m_y - m_s; y <= m_y + m_s; ++y)
        {
            a_vector(x, y) = State::Black;
        }
    }
}
