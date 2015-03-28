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
    for (size_t b = 0; b != m_request->m_nmbBallons; ++b)
    {
        m_altitudeMoves[0][b] = 1;
    }
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


/**/
std::vector<int> Result::getShortestPath(Coordinate a_from, Coordinate a_to)
{
    if (a_from == a_to)
        return std::vector<int>();

    struct NodeInfo
    {
        int m_cost = std::numeric_limits<int>::max();
        Coordinate m_predIndex;
        bool m_isValid=false;
    };

    struct PQEntry
    {
        int m_cost;
        Coordinate m_nodeIndex;

        bool operator< (const PQEntry& e) const
        {
            return m_cost > e.m_cost;
        }
    };


    std::vector<Grid<NodeInfo>> info(m_request->m_nmbAltitudes,
                                     Grid<Vector>(m_request->m_nmbRows, m_request->m_nmbColumns));

    auto getNodeInfo = [&] (const Coordinate& a_coord)
    {
            return info[a_coord.m_alt](a_coord.m_row,a_coord.m_column);
    };

    getNodeInfo(a_from).m_cost = 0;

    std::priority_queue<PQEntry> q;

    q.push( PQEntry { 0, a_from });
    while (!q.empty())
    {
        const PQEntry currentQE = q.top();
        if (currentQE.m_nodeIndex == a_to)
            break;

        q.pop();
        if (getNodeInfo(currenQE.m_nodeIndex).m_cost < currentQE.m_cost)
            continue; // node already settled

        std::vector<int> neighbours;
        neighbours.push_back(0);
        if(currentQE.m_alt>1)
            neighbours.push_back(-1);
        if(currentQE.m_alt<m_request.m_nmbAltitudes)
            neighbours.push_back(1);

        for (size_t s : neighbours)
        {
            const Street str = m_request->m_streets[s];
            const int costWhenUsingThisStreet = info[currentJunction.m_nodeIndex].m_cost + getStreetCost(currentJunction.m_nodeIndex, s);

            const size_t oppositeJunction = str.getOppositeJunction(currentJunction.m_nodeIndex);
            if (costWhenUsingThisStreet < info[oppositeJunction].m_cost)
            {
                info[oppositeJunction].m_predIndex = currentJunction.m_nodeIndex;
                info[oppositeJunction].m_cost = costWhenUsingThisStreet;
                q.push( PQEntry { info[oppositeJunction].m_cost, oppositeJunction });
            }
        }
    }

    std::vector<size_t> result;
    size_t currentQE = a_toJunction;
    while (currentQE != std::numeric_limits<size_t>::max())
    {
        result.push_back(currentQE);
        currentQE = info[currentQE].m_predIndex;
    }

    if (result.back() != a_from)
        result.empty(); // path not found
    else
        result.pop_back();

    std::reverse(begin(result), end(result));
    return result;
}

/**/
