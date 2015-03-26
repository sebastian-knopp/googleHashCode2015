#include "Request.h"
#include "Result.h"
#include <fstream>
#include <iostream>
#include <algorithm>


int main()
{
    std::cout << "Google Hash Code Challenge 2014- Team A211" << std::endl;
    std::cout << std::endl;
    std::cout << "hello world" << std::endl;

    Request r;
    std::ifstream fs("paris_54000.txt");
    if (!fs)
    {
        std::cout << "file not found\n";
        exit(1);
    }

    fs >> r;

    r.computeAdjacentStreetIndices();

    std::cout << r;

    Result result(r);

    result.searchGreedily();
    result.visualize();

    std::cout << "total distance " << result.m_totalDistance << std::endl;

    std::ofstream ofs("example.out");
    ofs << result;

    return 0;

}



