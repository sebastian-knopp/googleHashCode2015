#include "Response.h"
#include "Request.h"
#include "SVGWriter.h"


Response::Response(const Request& a_request)
: m_request(&a_request)
{
}


void Response::solve()
{
    m_result = m_request->m_test * m_request->m_test;
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
    a_is << a_response.m_result;
    return a_is;
}

