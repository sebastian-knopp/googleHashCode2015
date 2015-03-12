#pragma once
#include "Server.h"
#include "Coordinate.h"


class Request {

public:

    Request()
    {
    }


public:

    size_t m_nmbRows = 0;
    size_t m_nmbSlots = 0;

    Coordinates m_unavailableSlots;

    Servers m_servers;

    //! Nmb pools to be created
    size_t m_nmbPools = 0;

};


std::istream& operator>>(std::istream& is, Request& a_request);


Request readFile(std::string filename);


