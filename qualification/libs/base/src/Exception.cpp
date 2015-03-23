#include "Exception.h"
#include "stringtools.h"
#include <iostream>


namespace base {


Exception::Exception(const char* const a_message)
: std::runtime_error(a_message)
{
}


Exception::Exception(const std::string& a_message)
: std::runtime_error(a_message)
{
}


void Exception::logException(const std::exception& a_exception)
{
    std::cerr << "Exception thrown: " << a_exception.what() << "\n";
}


} // namespace base

