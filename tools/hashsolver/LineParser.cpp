#include "LineParser.h"

#include "base/stringtools.h"
#include <sstream>


LineParser::Exception::Exception(size_t a_line, std::string a_what)
: base::Exception(base::stringtools::concat("Error in line ", a_line, ": ", a_what))
{
}


LineParser::LineParser(std::istream& a_istream)
: m_istream(a_istream)
{
    getLine();
}


LineParser::operator bool() const
{
    return static_cast<bool>(m_istream);
}


void LineParser::expectLine(std::string a_expectedLine)
{
    std::string line = getLine();

    if (line != a_expectedLine)
    {
        std::stringstream s;
        s << "Expected: " << a_expectedLine << std::endl;
        throwException(s.str());
    }
}


std::string LineParser::getLine()
{
    std::string line = m_currentLine;
    getline(m_istream, m_currentLine);
    ++m_currentLineNumber;
    return line;
}


void LineParser::throwException(const std::string& a_message) const
{
    base::Exception::raise<Exception>(m_currentLineNumber - 1, a_message);

}

