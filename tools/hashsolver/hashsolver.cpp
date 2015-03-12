#include <iostream>
#include <fstream>
#include <limits>
#include "Server.h"


void readFile(std::string filename)
{
    std::ifstream ifs(filename.c_str());
    if (!ifs)
        std::cout << "cannot open file" << std::endl;
}


int main (int , char *[])
{
    std::cout << "Google Hash 2015" << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "Team A211: Abdel Bitar and Sebastian Knopp" << std::endl;


    readFile("example.txt");
}



