#pragma once

#include "base/Exception.h"
#include <istream>


/**
 * @brief Facilitates parsing of a stream
 *        that contains text line by line.
 */
class LineParser {

public:

    //! Can be used to indicate a parse error.
    struct Exception : public base::Exception
    {
        Exception(size_t a_line, std::string a_what);
    };


public:

    //! Creates a parser object from a stream.
    explicit LineParser(std::istream& a_istream);

    //! Returns if there is still a line remaining to be read.
    operator bool() const;

    /**
     * @brief Reads a line that is expected to have a certain content.
     * @throw Exception if read something else
     */
    void expectLine(std::string a_expectedLine);

    //! Reads and returns the current line
    std::string getLine();

    //! Throws an exception to indicate an error at the current line.
    void throwException(const std::string& a_message) const;


private:

    //! The stream to be parsed.
    std::istream& m_istream;

    //! The current line that was read.
    std::string m_currentLine = "";

    //! The current line number.
    size_t m_currentLineNumber = 0;

};

