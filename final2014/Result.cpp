#include "Result.h"
#include "Request.h"
#include <tuple>
#include <algorithm>
#include <fstream>
#include <iostream>


Result::Result(const Request& a_request)
:m_request(&a_request)
{
}


void Result::visualize()
{
    std::ofstream ofs("tikz.tex");
    ofs << "\\documentclass{article}\n"
    << "\\usepackage{tikz}\n"
    << "\\usetikzlibrary{shapes}\n"
    << "\\begin{document}\n"
    << "\\pagestyle{empty}\n"
    << "\\begin{tikzpicture}[scale=1, every node/.style={scale=0.3}]\n";

    auto printJunction = [&ofs, this] (size_t a_junctionIndex)
    {
        ofs << "	\\node[circle, ";

        if (a_junctionIndex == m_request->m_initialJunctionIndex)
            ofs << "fill=red";
        else if (carCanUseJunction(0, a_junctionIndex))
            ofs << "fill=yellow";
        else if (carCanUseJunction(1, a_junctionIndex))
            ofs << "fill=blue";
        else if (carCanUseJunction(2, a_junctionIndex))
            ofs << "fill=purple";
        else if (carCanUseJunction(3, a_junctionIndex))
            ofs << "fill=blue!50";
        else if (carCanUseJunction(4, a_junctionIndex))
            ofs << "fill=green!50";
        else if (carCanUseJunction(5, a_junctionIndex))
            ofs << "fill=red!50";
        else if (carCanUseJunction(6, a_junctionIndex))
            ofs << "fill=cyan";
        else if (carCanUseJunction(7, a_junctionIndex))
            ofs << "fill=green";
        else
            ofs << "draw=black";

        ofs << "] at (";
        ofs << 100 * (m_request->m_junctions[a_junctionIndex].m_lat - m_request->m_junctions[0].m_lat)
            << ", "
            << 100 * (m_request->m_junctions[a_junctionIndex].m_long - m_request->m_junctions[0].m_long)
            << ") {};\n";
    };

    printJunction(m_request->m_initialJunctionIndex);
    for (size_t j = 0; j != m_request->m_junctions.size(); ++j)
    {
        printJunction(j);
    }

    ofs << "\\end{tikzpicture}\n"
    << "\\end{document}\n";
}


bool Result::carCanUseJunction(size_t a_carIndex, size_t a_junctionIndex) const
{
    if (getDistance(m_request->getOrigin(), m_request->m_junctions[a_junctionIndex]) < 0.02)
        return true;

    double angle = getAngle(m_request->getOrigin(), m_request->m_junctions[a_junctionIndex]);

    double minAngle = static_cast<double>(a_carIndex) * 2 * pi() / static_cast<double>(m_request->m_nmbCars) - pi();
    double maxAngle = static_cast<double>(a_carIndex + 1) * 2 * pi() / static_cast<double>(m_request->m_nmbCars) - pi();

    return minAngle <= angle && angle <= maxAngle;
}


std::ostream& operator<<(std::ostream& a_is, const Result& a_result)
{
    a_is << a_result.m_itineraries.size() << "\n";
    for (const Itinerary& i : a_result.m_itineraries)
    {
        a_is << i.size() << "\n";
        for (const size_t j : i)
        {
            a_is << j << "\n";
        }
    }
    return a_is;
}

