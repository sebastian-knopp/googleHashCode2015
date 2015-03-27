#pragma once
#include <istream>
#include <vector>

enum class Taste
{
    Ham,
    Other
};


typedef std::vector<std::vector<Taste>> Pizza;


class Request {

public:

    Request();


public:

    Pizza m_pizza;

    size_t minNmbHam = 0;

    size_t maxNmbCells = 0;

};



std::istream& operator>>(std::istream& a_is, Request& a_request);

std::ostream& operator<<(std::ostream& a_is, const Request& a_request);


