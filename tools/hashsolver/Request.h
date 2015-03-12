#pragma once
#include "Server.h"


class Request {

public:

    Request()
    {
    }

public:

    Servers m_servers;

};


std::istream& operator>>(std::istream& is, Request& a_request);

