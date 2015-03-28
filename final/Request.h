#pragma once
#include <istream>


class Request {

public:

    Request();


public:

    int m_test = 0;

};



std::istream& operator>>(std::istream& a_is, Request& a_request);

std::ostream& operator<<(std::ostream& a_os, const Request& a_request);


