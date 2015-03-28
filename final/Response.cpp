#include "Response.h"
#include "Request.h"
#include "SVGWriter.h"
#include "Assertion.h"
#include <queue>
#include <algorithm>


Response::Response(const Request& a_request)
: m_request(&a_request)
, m_altitudeMoves(m_request->m_nmbTurns, std::vector<size_t>(m_request->m_nmbBallons, 0))
{
}


void Response::solve()
{
    const std::vector<Coordinate> targets = getBalloonTargets();

    for (int b = 0; b != m_request->m_nmbBallons; ++b)
    {
        Coordinate target;
        target.m_row = b * 17 % m_request->m_nmbRows;
        target.m_column = b * 5 % m_request->m_nmbColumns;
        target.m_alt = 5;

        std::vector<int> path = getShortestPath(m_request->m_startCell, target);
        for (size_t i = 0; i != path.size(); ++i)
        {
            m_altitudeMoves[i][b] = path[i];
        }
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
    for (int t = 0; t != a_response.m_request->m_nmbTurns; ++t)
    {
        if (!initial)
            a_os << "\n";

        for (int b = 0; b != a_response.m_request->m_nmbBallons; ++b)
        {
            a_os << a_response.m_altitudeMoves[t][b] << " ";
        }
        initial = false;
    }
    return a_os;
}


/**/
std::vector<int> Response::getShortestPath(Coordinate a_from, Coordinate a_to)
{
    if (a_from.m_row == a_from.m_row &&
        a_to.m_column == a_to.m_column)
    {
        return std::vector<int>();
    }

    struct NodeInfo
    {
        int m_cost = std::numeric_limits<int>::max();
        Coordinate m_predIndex;
        bool m_isValid = true;
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
                                     Grid<NodeInfo>(m_request->m_nmbRows, m_request->m_nmbColumns));

    info[a_from.m_alt](a_from.m_row, a_from.m_column).m_cost = 0;
    info[a_from.m_alt](a_from.m_row, a_from.m_column).m_isValid = false;

    std::priority_queue<PQEntry> q;

    q.push( PQEntry { 0, a_from });
    while (!q.empty())
    {
        const PQEntry currentQE = q.top();
        if (currentQE.m_nodeIndex.m_row == a_to.m_row &&
            currentQE.m_nodeIndex.m_column == a_to.m_column)
            break;

        q.pop();
        auto& currentNode = info[currentQE.m_nodeIndex.m_alt](currentQE.m_nodeIndex.m_row, currentQE.m_nodeIndex.m_column);
        if (currentNode.m_cost < currentQE.m_cost)
            continue; // node already settled

        std::vector<int> neighbours;
        neighbours.reserve(3);
        neighbours.push_back(0);

        if (currentQE.m_nodeIndex.m_alt > 1)
            neighbours.push_back(-1);
        if (currentQE.m_nodeIndex.m_alt < m_request->m_nmbAltitudes - 1)
            neighbours.push_back(1);

        for (int s : neighbours)
        {
            const int costWhenUsingThisNeighbor = currentNode.m_cost + 1;
            Coordinate neighborCoord = currentQE.m_nodeIndex;

            int rowDiff = m_request->m_windVectors[currentQE.m_nodeIndex.m_alt + s](currentQE.m_nodeIndex.m_row, currentQE.m_nodeIndex.m_column).m_rowDiff;
            int columnDiff = m_request->m_windVectors[currentQE.m_nodeIndex.m_alt + s](currentQE.m_nodeIndex.m_row, currentQE.m_nodeIndex.m_column).m_columnDiff;
            neighborCoord.m_row += rowDiff;
            neighborCoord.m_column += columnDiff % m_request->m_nmbColumns;

            if (neighborCoord.m_row < 0 || neighborCoord.m_row >= m_request->m_nmbRows)
                continue;

            NodeInfo& oppositeNode = info[neighborCoord.m_alt](neighborCoord.m_row, neighborCoord.m_column);
            if (costWhenUsingThisNeighbor < oppositeNode.m_cost)
            {
                oppositeNode.m_predIndex = currentQE.m_nodeIndex;
                oppositeNode.m_cost = costWhenUsingThisNeighbor;
                q.push( PQEntry { oppositeNode.m_cost, neighborCoord });
            }

        }
    }

    std::vector<int> result;
    Coordinate currentQE = a_to;
    int currentAlt = currentQE.m_alt;
    while (info[currentQE.m_alt](currentQE.m_row, currentQE.m_column).m_isValid)
    {
        currentQE = info[currentQE.m_alt](currentQE.m_row, currentQE.m_column).m_predIndex;
        result.push_back(currentAlt - currentQE.m_alt);
        currentAlt =currentQE.m_alt;
    }

    if (info[a_to.m_alt](a_to.m_row, a_to.m_column).m_cost == std::numeric_limits<int>::max())
    {
        result.empty(); // path not found
    }
//    else
//        result.pop_back();

    std::reverse(begin(result), end(result));
    return result;
}


std::vector<Coordinate> Response::getBalloonTargets()
{
    return std::vector<Coordinate>(m_request->m_nmbBallons);
}


/**/
