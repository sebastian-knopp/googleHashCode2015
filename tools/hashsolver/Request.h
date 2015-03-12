#pragma once
#include "Server.h"
#include "Coordinate.h"


class Request {

public:

    Request()
    {
    }


public:

    int32_t m_nmbRows = 0;
    int32_t m_nmbColumns = 0;

    Coordinates m_unavailableSlots;

    Servers m_servers;

    //! Nmb pools to be created
    int32_t m_nmbPools = 0;

};


std::istream& operator>>(std::istream& is, Request& a_request);


Request readFile(std::string filename);


