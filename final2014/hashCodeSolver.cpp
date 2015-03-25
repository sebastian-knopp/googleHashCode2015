#include "Request.h"
#include "Result.h"
#include <fstream>
#include <iostream>
#include <algorithm>



//Abdel
//Comparing two streets, just to sort it correctly and give it as a parameter to the sort function of standard algorithm library
bool compareStreets(Street s1, Street s2){
    return ( ( (double)(s1.m_length)/(double)(s1.m_cost) > (double)(s2.m_length)/(double)(s2.m_cost)  ) || ( ( (double)(s1.m_length)/(double)(s1.m_cost) == (double)(s2.m_length)/(double)(s2.m_cost)) && (s1.m_length > s2.m_length)) );
}

//////////////////////////////////////////////////////////////////////////////////////////


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

    //Abdel
    //Sorting street list in the increasing order of length/cost
    sort(r.m_streets.begin(),r.m_streets.end(),compareStreets);
/*
    //for(size_t i=0;i<r.m_streets.size();++i)
    //    std::cout <<"length "<<r.m_streets[i].m_length<<" cost "<<r.m_streets[i].m_cost<<" ratio "<<(double)(r.m_streets[i].m_length)/(double)(r.m_streets[i].m_cost)<<std::endl;
*/
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    r.computeAdjacentStreetIndices();

    std::cout << r;

    Result result(r);

    //result.searchGreedily();
    result.visualize();

    std::cout << "total distance " << result.m_totalDistance << std::endl;

    std::ofstream ofs("example.out");
    ofs << result;

    return 0;

}



