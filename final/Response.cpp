#include "Response.h"
#include "Request.h"
#include "SVGWriter.h"
#include "Assertion.h"


Response::Response(const Request& a_request)
: m_request(&a_request)
{
}


void Response::solve()
{
    Slice s = { 0, 0, 1, 1};
    //std::cout << *m_request << std::endl;
    std::cout << "nmbham " << m_request->getNmbHam(s) << std::endl;
    std::cout << "cells " << s.getNmbCells() << std::endl;
    /*
    for (size_t r = 0; r < nmbRows; ++r)
    {
        for (size_t c = 0; c < nmbColumns; ++c)
        {

        }
    }
    */
}


void Response::visualize() const
{
    SVGWriter writer("visualize.html", 800, 30);

    writer.drawCircle(5, 12, 1, 20);
    writer.drawCircle(300, 12, 2, 20);

    writer.drawText(20, 10, "Google Hash 2015 - Final", 20);
    writer.drawText(20, 18, "Team: A211 - Mines Saint-Etienne - Gardanne - SFL", 14);

}


std::ostream& operator<<(std::ostream& a_is, const Response& a_response)
{
    for (const Slice& s : a_response.m_slices)
    {
        a_is << s;
    }
    return a_is;
}


std::ostream& operator<<(std::ostream& a_os, const Slice& a_slice)
{
    a_os << a_slice.m_row1;
    a_os << a_slice.m_column1;
    a_os << a_slice.m_row2;
    a_os << a_slice.m_column2;
    return a_os;
}
