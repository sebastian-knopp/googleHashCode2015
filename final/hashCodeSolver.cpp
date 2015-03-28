#include "Request.h"
#include "Response.h"
#include <iostream>
#include <fstream>


int solve(int, char* [])
{
    Request request;
    std::ifstream ifs("final_round.in");
    if (!ifs)
        throw "Cannot open file.";

    ifs >> request;
    std::cout << request << std::endl;

    Response resp(request);

    resp.solve();
    resp.visualize();

    std::ofstream ofs("example.out");
    ofs << resp << std::endl;

    return 0;
}


int main(int a_argn, char* a_argv[])
{
    std::cout << " --------------------------------------------------- " << std::endl;
    std::cout << "|         Google Hash Code Challenge 2015           |" << std::endl;
    std::cout << "| Team: A211 - Mines Saint-Etienne - Gardanne - SFL |" << std::endl;
    std::cout << " --------------------------------------------------- " << std::endl;
    std::cout << std::endl;

    try
    {
        return solve(a_argn, a_argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Caught std::exception: " << e.what() << std::endl;
    }
    catch(const std::string& s)
    {
        std::cerr << "Caught std::string: " << s << std::endl;
    }
    catch(const char* c)
    {
        std::cerr << "Caught char*: " << c << std::endl;
    }
    catch(...)
    {
        std::cerr << "Caught unknown exception.";
    }

    return 1;
}



