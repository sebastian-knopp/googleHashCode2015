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
//    std::cout << "nmbham " << m_request->getNmbHam(s) << std::endl;
//    std::cout << "cells " << s.getNmbCells() << std::endl;

    Slice s = { 0, 0, m_request->getNmbRows() - 1, m_request->getNmbColumns() - 1};

    calcSlice(s);
/*
    for (size_t r = 0; r < nmbRows; ++r)
    {
        for (size_t c = 0; c < nmbColumns; ++c)
        {

        }
    }
*/
}


void Response::calcSlice(const Slice a_slice)
{
    if (a_slice.m_column1 > a_slice.m_column2)
        return;
    if (a_slice.m_column2 >= m_request->getNmbColumns())
        return;
    if (a_slice.m_row1 > a_slice.m_row2)
        return;
    if (a_slice.m_row2 >= m_request->getNmbRows())
        return;

    if (a_slice.getNmbCells() <= m_request->maxNmbCells)
    {
        if (m_request->getNmbHam(a_slice) >= m_request->minNmbHam)
        {
            m_slices.push_back(a_slice);
        }
        return;
    }

    if (a_slice.m_row1 == a_slice.m_row2)
        return;
    if (a_slice.m_column1 == a_slice.m_column2)
        return;

    Slice s1 = a_slice;
    Slice s2 = a_slice;

    size_t splitRow = (a_slice.m_row2 + a_slice.m_row1) / 2;
    s1.m_row2 = splitRow;
    s2.m_row1 = splitRow + 1;

    Slice s3 = s1;
    Slice s4 = s2;

    size_t splitColumn = (a_slice.m_column2 + a_slice.m_column1) / 2;
    s3.m_column1 = splitColumn + 1;
    s4.m_column1 = splitColumn + 1;
    s1.m_column2 = splitColumn;
    s2.m_column2 = splitColumn;


    ASSERT(s1.m_row2 < s2.m_row1);
    ASSERT(s3.m_row2 < s4.m_row1);
    ASSERT(s1.m_column2 < s3.m_column1);
    ASSERT(s2.m_column2 < s4.m_column1);

    calcSlice(s1);
    calcSlice(s2);
    calcSlice(s3);
    calcSlice(s4);
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
    a_is << a_response.m_slices.size() << "\n";
    bool initial = true;
    for (const Slice& s : a_response.m_slices)
    {
        if (!initial)
            a_is << "\n";

        initial = false;

        a_is << s;
    }
    return a_is;
}


std::ostream& operator<<(std::ostream& a_os, const Slice& a_slice)
{
    a_os << a_slice.m_row1 << " ";
    a_os << a_slice.m_column1 << " ";
    a_os << a_slice.m_row2 << " ";
    a_os << a_slice.m_column2 << " ";
    return a_os;
}
