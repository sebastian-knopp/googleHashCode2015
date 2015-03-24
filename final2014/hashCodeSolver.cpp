#include "Request.h"
#include <fstream>
#include <iostream>


int main()
{
    std::cout << "Google Hash Code Challenge 2015- Team A211" << std::endl;
    std::cout << std::endl;
    std::cout << "hello world" << std::endl;

    Request r;
    std::ifstream fs("example.in");
    if (!fs)
    {
        std::cout << "file not found\n";
        exit(1);
    }

    fs >> r;
    r.computeAdjacentStreetIndices();

    std::cout << r;

    return 0;

}



