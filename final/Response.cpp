#include "Response.h"
#include "Request.h"
#include "SVGWriter.h"
#include "Assertion.h"


Response::Response(const Request& a_request)
: m_request(&a_request)
, m_altitudeMoves(m_request->m_nmbTurns, std::vector<size_t>(m_request->m_nmbBallons, 0))
{
}


void Response::solve()
{
    //m_result = m_request->m_test * m_request->m_test;
    //ASSERT(m_result == 42 * 42);
}


void Response::visualize() const
{
    SVGWriter writer("visualize.html", 800, 30);

    writer.drawCircle(5, 12, 1, 20);
    writer.drawCircle(300, 12, 2, 20);

    writer.drawText(20, 10, "Google Hash 2015 - Final", 20);
    writer.drawText(20, 18, "Team: A211 - Mines Saint-Etienne - Gardanne - SFL", 14);

}


std::ostream& operator<<(std::ostream& a_os, const Response& a_response)
{
    bool initial = true;
    for (size_t t = 0; t != a_response.m_request->m_nmbTurns; ++t)
    {
        if (!initial)
            a_os << "\n";

        for (size_t b = 0; b != a_response.m_request->m_nmbBallons; ++b)
        {
            a_os << a_response.m_altitudeMoves[t][b] << " ";
        }
        initial = false;
    }
    return a_os;
}

