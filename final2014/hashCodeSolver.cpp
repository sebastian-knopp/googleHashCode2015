#include "Request.h"
#include "Result.h"
#include <fstream>
#include <iostream>


int main()
{
    std::cout << "Google Hash Code Challenge 2015- Team A211" << std::endl;
    std::cout << std::endl;
    std::cout << "hello world" << std::endl;

    Request req;
    std::ifstream fs("example.in");
    if (!fs)
    {
        std::cout << "file not found\n";
        exit(1);
    }

    fs >> req;
    req.computeAdjacentStreetIndices();

    std::cout << req;

    Result result(req);

    std::vector<size_t> targetJunctionIndices;
    targetJunctionIndices.resize(req.m_nmbCars, 0);



    std::ofstream ofs("example.out");
    ofs << result;

    return 0;

}



