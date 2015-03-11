#pragma once

#include "LineParser.h"
#include "base/Exception.h"
#include <map>
#include <vector>
#include <sstream>


class CSVParser {

public:

    //! Can be used to indicate a parse error.
    struct Exception : public base::Exception
    {
        Exception(std::string a_what);
    };

    CSVParser(std::istream& a_istream, char m_separator);

    //! Returns if there is still a line remaining to be read.
    operator bool() const;

    //! Reads header information and check if the expected columns are present.
    void readHeader();

    //! Reads the next line.
    void getLine();

    //! Returns the number of the columns of the line that was previously read.
    size_t getNmbColumns() const;

    //! Returns the value for the given key of the current line.
    std::string get(const std::string& a_key) const;

    //! Returns the value in the given column of the current line.
    std::string get(size_t a_columnIndex) const;

    template<typename T>
    T get(const std::string& a_key) const
    {
        std::stringstream s(get(a_key));
        T value;
        s >> value;
        return value;
    }

    template<typename T>
    T get(const size_t a_columnIndex) const
    {
        std::stringstream s(get(a_columnIndex));
        T value;
        s >> value;
        return value;
    }


private:

    LineParser m_lineParser;

    const char m_separator;

    std::vector<std::string> m_currentLine = {};

    //! Maps keys to columns
    std::map<std::string, size_t> m_keyToColumn;

};

