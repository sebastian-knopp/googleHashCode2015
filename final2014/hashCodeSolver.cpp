#include "Request.h"
#include "Result.h"
#include <fstream>
#include <iostream>


int main()
{
    std::cout << "Google Hash Code Challenge 2014- Team A211" << std::endl;
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

    Result result(r);

    std::ofstream ofs("example.out");
    ofs << result;

    return 0;

}



