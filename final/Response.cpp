#include "Response.h"
#include "Request.h"
#include "SVGWriter.h"
#include "Assertion.h"
#include <queue>
#include <algorithm>


Response::Response(const Request& a_request)
: m_request(&a_request)
, m_altitudeMoves(m_request->m_nmbTurns, std::vector<int>(m_request->m_nmbBallons, 0))
, m_isReachable(m_request->m_nmbAltitudes, Grid<int>(m_request->m_nmbRows, m_request->m_nmbColumns))
{
}


void Response::solve()
{
    Coordinate unreachable;
    unreachable.m_row = m_request->m_startCell.m_row + 1;
    unreachable.m_column = m_request->m_startCell.m_column + 1;
    getShortestPath(m_request->m_startCell, unreachable);

    std::vector<Coordinate> reachableTargets;
    for (int a = 0; a != m_request->m_nmbAltitudes; ++a)
    {
        for (int r = 0; r != m_request->m_nmbRows; ++r)
        {
            for (int c = 0; c != m_request->m_nmbColumns; ++c)
            {
                if (m_isReachable[a](r, c) == 1)
                {
                    Coordinate coord { c, r, a};
/*
                    std::cout << "c row: " << coord.m_row << std::endl;
                    std::cout << "c col: " << coord.m_column << std::endl;
                    std::cout << "c alt: " << coord.m_alt << std::endl;
*/
                    reachableTargets.push_back(coord);
                }
            }
        }
    }

    std::cout << "Found " << reachableTargets.size() << " reachable targets" << std::endl;

    const std::vector<Coordinate> targets = getBalloonTargets();

    int nmbBallons = 1; // m_request->m_nmbBallons;
    for (int b = 0; b != nmbBallons; ++b)
    {
        Coordinate target;
        //target.m_row = 27; //m_request->m_startCell.m_row + 1;
        //target.m_column = 169; //m_request->m_startCell.m_column + 1;

        target = reachableTargets[b * 5 % reachableTargets.size()];
/*
        std::cout << "t row: " << target.m_row << std::endl;
        std::cout << "t col: " << target.m_column << std::endl;
        std::cout << "t alt: " << target.m_alt << std::endl;
*/
//        target.m_row = b * 17 % m_request->m_nmbRows;
//        target.m_column = b * 5 % m_request->m_nmbColumns;

        std::vector<int> path = getShortestPath(m_request->m_startCell, target);
        for (size_t i = 0; i != path.size(); ++i)
        {
            std::cout << "add " << path[i] << std::endl;
            m_altitudeMoves[i][b] = path[i];
        }
    }
}


void Response::visualize() const
{
    SVGWriter writer("visualize.html", 800, 30);

    for (int r = 0; r != m_request->m_nmbRows; ++r)
    {
        for (int c = 0; c != m_request->m_nmbColumns; ++c)
        {
            for (int a = 0; a != m_request->m_nmbAltitudes; ++a)
            {
                if (m_isReachable[a](r, c))
                {
                    writer.drawRectangle(c, r, c+1, r+1, 0);
                    break;
                }
            }
        }
    }

    writer.drawRectangle(m_request->m_startCell.m_row, m_request->m_startCell.m_column,
                         m_request->m_startCell.m_row + 1, m_request->m_startCell.m_column + 1, 4);

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
    /*
    std::cout << "a_from row: " << a_from.m_row << std::endl;
    std::cout << "a_from col: " << a_from.m_column << std::endl;
    std::cout << "a_from alt: " << a_from.m_alt << std::endl;

    std::cout << "a_to row: " << a_to.m_row << std::endl;
    std::cout << "a_to col: " << a_to.m_column << std::endl;
    std::cout << "a_to alt: " << a_to.m_alt << std::endl;
*/
    if (a_from.m_row == a_to.m_row &&
        a_from.m_column == a_to.m_column)
    {
        std::cout << "already there" << std::endl;
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
    bool foundTarget = false;

    q.push( PQEntry { 0, a_from });
    while (!q.empty())
    {
        const PQEntry currentQE = q.top();

        m_isReachable[currentQE.m_nodeIndex.m_alt](currentQE.m_nodeIndex.m_row, currentQE.m_nodeIndex.m_column) = 1;

        if (currentQE.m_nodeIndex.m_row == a_to.m_row &&
            currentQE.m_nodeIndex.m_column == a_to.m_column)
        {
            a_to.m_alt = currentQE.m_nodeIndex.m_alt;
            foundTarget = true;
            std::cout << "found target" << std::endl;
            break;
        }

        q.pop();
        auto& currentNode = info[currentQE.m_nodeIndex.m_alt](currentQE.m_nodeIndex.m_row, currentQE.m_nodeIndex.m_column);
        ASSERT(currentNode.m_cost < 999999);
        if (currentQE.m_cost > currentNode.m_cost || (!currentNode.m_isValid && !q.empty()))
        {
            std::cout << "stop on settled" << std::endl;
            continue; // node already settled
        }

        {
            static int count = 0;
            ++count;
/*
            //if (count < 100)
            {
                std::cout << "cost: " << currentQE.m_cost << std::endl;
                std::cout << "row: " << currentQE.m_nodeIndex.m_row << std::endl;
                std::cout << "col: " << currentQE.m_nodeIndex.m_column << std::endl;
                std::cout << "alt: " << currentQE.m_nodeIndex.m_alt << std::endl;
                std::cout << "valid: " << currentNode.m_isValid << std::endl;
                std::cout << std::endl;
            }
*/
            //ASSERT(count == 1 || currentNode.m_isValid);
        }

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
            neighborCoord.m_column += columnDiff;
            if (neighborCoord.m_column < 0)
                neighborCoord.m_column += m_request->m_nmbColumns;
            neighborCoord.m_column = (neighborCoord.m_column % m_request->m_nmbColumns);

            if (neighborCoord.m_row < 0 || neighborCoord.m_row >= m_request->m_nmbRows)
                continue;

            neighborCoord.m_alt += s;

            NodeInfo& neighborNode = info[neighborCoord.m_alt](neighborCoord.m_row, neighborCoord.m_column);
            if (costWhenUsingThisNeighbor < neighborNode.m_cost)
            {
                neighborNode.m_predIndex = currentQE.m_nodeIndex;
                neighborNode.m_cost = costWhenUsingThisNeighbor;
/*
                std::cout << "push row: " << neighborCoord.m_row << std::endl;
                std::cout << "push col: " << neighborCoord.m_column << std::endl;
                std::cout << "push alt: " << neighborCoord.m_alt << std::endl;
*/
                q.push( PQEntry { neighborNode.m_cost, neighborCoord });
            }

        }
    }

    std::vector<int> result;
    if (!foundTarget)
    {
        std::cout << "could not find target" << std::endl;
        return result;
    }

    Coordinate currentQE = a_to;
    int currentAlt = currentQE.m_alt;
    while (info[currentQE.m_alt](currentQE.m_row, currentQE.m_column).m_isValid)
    {
        currentQE = info[currentQE.m_alt](currentQE.m_row, currentQE.m_column).m_predIndex;
        int move = currentAlt - currentQE.m_alt;
/*
        std::cout << "m " << move << std::endl;
        std::cout << "m row: " << currentQE.m_row << std::endl;
        std::cout << "m col: " << currentQE.m_column << std::endl;
        std::cout << "m alt: " << currentQE.m_alt << std::endl;
*/
        result.push_back(move);
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
