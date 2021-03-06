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
    if (m_request->getNmbHam(a_slice) < m_request->minNmbHam)
        return;

    if (a_slice.getNmbCells() <= m_request->maxNmbCells)
    {
        m_overallCells += a_slice.getNmbCells();
        m_slices.push_back(a_slice);
        return;
    }

    size_t thisSize = a_slice.m_row2 - a_slice.m_row1 + 1;
    size_t otherSize = a_slice.m_column2 - a_slice.m_column1 + 1;

    Slice first = a_slice;
    Slice second = a_slice;
    Slice test = a_slice;

    const size_t* beginOrig = &a_slice.m_row1;
    const size_t* endOrig = &a_slice.m_row2;
    size_t* endOfFirst = &first.m_row2;
    size_t* beginOfSecond = &second.m_row1;
    size_t* beginTest = &test.m_row1;
    size_t* endTest = &test.m_row2;
    if (otherSize > thisSize)
    {
        beginOrig = &a_slice.m_column1;
        endOrig = &a_slice.m_column2;
        endOfFirst = &first.m_column2;
        beginOfSecond = &second.m_column1;
        beginTest = &test.m_column1;
        endTest = &test.m_column2;
    }

    //    size_t split = (*beginOrig + *endOrig) / 2;

    size_t nmbHamOrig = m_request->getNmbHam(a_slice);
    size_t sumHam = 0;

    size_t split = *beginOrig;
    for (split = *beginOrig; split < *endOrig; ++split)
    {
        *beginTest = split;
        *endTest = split;
        sumHam += m_request->getNmbHam(test);
        if ((sumHam > nmbHamOrig / 2) && sumHam >= m_request->minNmbHam)
            break;
    }

    *endOfFirst = split;

    if (*endOfFirst >= *endOrig)
        return;

    calcSlice(first);

    *beginOfSecond = split + 1;
    if (*beginOfSecond <= *endOrig)
        calcSlice(second);
}


void Response::visualize() const
{
    SVGWriter writer("visualize.html", 1300, 30);

    for (size_t c = 0; c < m_request->getNmbColumns(); ++c)
    {
        for (size_t r = 0; r < m_request->getNmbRows(); ++r)
        {
            if (m_request->m_pizza[c][r] == Taste::Ham)
                writer.drawRectangle(r, c, r+1, c+1, 0);
            else
                writer.drawRectangle(r, c, r+1, c+1, 1);
        }
    }

    for (const Slice& s : m_slices)
    {
        writer.drawRectangle(s.m_row1, s.m_column1, s.m_row2+1, s.m_column2+1, 7, 1);
    }
}


size_t Response::getOverallCells()
{
    return m_overallCells;
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
