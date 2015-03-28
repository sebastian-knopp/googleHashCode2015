#include "Request.h"


Request::Request()
{
}


std::istream& operator>>(std::istream& a_is, Request& /*a_request*/)
{
    //a_is >> a_request.m_test;
    return a_is;
}


std::ostream& operator<<(std::ostream& a_os, const Request& /*a_request*/)
{
    //a_os << a_request.m_test;
    return a_os;
}

