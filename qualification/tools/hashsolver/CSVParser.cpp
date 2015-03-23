#include "CSVParser.h"
#include "base/algorithm.h"
#include <sstream>


CSVParser::Exception::Exception(std::string a_what)
: base::Exception(a_what)
{
}


CSVParser::CSVParser(std::istream& a_istream,
                     char a_separator)
: m_lineParser(a_istream)
, m_separator(a_separator)
{
}


CSVParser::operator bool() const
{
    return m_lineParser;
}


void CSVParser::readHeader()
{
    getLine();
    for (size_t i = 0; i != m_currentLine.size(); ++i)
    {
        m_keyToColumn[m_currentLine[i]] = i;
    }
}


void CSVParser::getLine()
{
    m_currentLine.clear();
    std::stringstream s(m_lineParser.getLine());
    std::string item;
    while (std::getline(s, item, m_separator)) {
        m_currentLine.push_back(item);
    }
}


size_t CSVParser::getNmbColumns() const
{
    return m_currentLine.size();
}


std::string CSVParser::get(const std::string& a_key) const
{
    auto it = m_keyToColumn.find(a_key);
    if (it == m_keyToColumn.end())
    {
        std::stringstream s;
        s << "Unknown key " << a_key;
        base::Exception::raise<Exception>(s.str());
    }

    if (it->second >= m_currentLine.size())
        return "";

    return m_currentLine[it->second];
}


std::string CSVParser::get(size_t a_columnIndex) const
{
    if (a_columnIndex >= m_currentLine.size())
    {
        std::stringstream s;
        s << "Column " << a_columnIndex
          << " cannot be accessed because there are only "
          << m_currentLine.size();
        base::Exception::raise<Exception>(s.str());
    }

    return m_currentLine[a_columnIndex];
}

