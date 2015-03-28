#include "Response.h"
#include "Request.h"
#include "SVGWriter.h"
#include "Assertion.h"
#include <queue>
#include <algorithm>
#include <ctime>


Response::Response(const Request& a_request)
: m_request(&a_request)
, m_altitudeMoves(m_request->m_nmbTurns, std::vector<int>(m_request->m_nmbBallons, 0))
, m_isReachable(m_request->m_nmbAltitudes, Grid<int>(m_request->m_nmbRows, m_request->m_nmbColumns))
{
}



void Response::solve()
{

    srand(time(NULL));
    Coordinate unreachable;
    unreachable.m_row = m_request->m_startCell.m_row + 1;
    unreachable.m_column = m_request->m_startCell.m_column + 1;
    getShortestPath(m_request->m_startCell, unreachable, false);

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

                    reachableTargets.push_back(coord);
                }
            }
        }
    }

    /*
    for (int t = 1; t != m_request->m_nmbTurns; ++t){
        int currentalt=1;
        for (int b = 0; b != m_request->m_nmbBallons; ++b){
            int dep=-1 + rand()%3;
            currentalt+=dep;
            m_altitudeMoves[t][b] = dep;
            if(currentalt==0 || currentalt>m_request->m_nmbAltitudes){
                m_altitudeMoves[t][b] = 0;
                currentalt-=dep;
            }
        }
    }
    */

    std::cout << "Found " << reachableTargets.size() << " reachable targets" << std::endl;

    const std::vector<Coordinate> targets = getBalloonTargets();
    ASSERT(targets.size() == static_cast<size_t>(m_request->m_nmbBallons));

    int r = 0;
    int nmbBallons = m_request->m_nmbBallons;
    for (int b = 0; b != nmbBallons; ++b)
    {
        int turnIndex = 0;
        Coordinate start = m_request->m_startCell;
        while (turnIndex < m_request->m_nmbTurns)
        {
            ++r;
            Coordinate target;
            target = reachableTargets[(r * 3513 + 99) % reachableTargets.size()];
            for (int a = 0; a != m_request->m_nmbAltitudes; ++a)
            {
                if (m_isReachable[a](targets[b].m_row, targets[b].m_column) == 1)
                {
                    //std::cout << "use reachable target" << std::endl;
                    target = targets[b];
                    target.m_alt = a;
                    break;
                }
                else
                {
                    //std::cout << "use random target" << std::endl;
                }
            }
            std::vector<int> path = getShortestPath(start, target, true);
            if (path.empty())
            {
                break;
            }

            for (int p = 0; p < static_cast<int>(path.size()); ++p)
            {
                if (turnIndex+p >= m_request->m_nmbTurns)
                    break;

                m_altitudeMoves[turnIndex+p][b] = path[p];
            }
            turnIndex += path.size();
            start = target;
        }

/*
        Coordinate from1 = target;

        int rowDiff = m_request->m_windVectors[target.m_alt](target.m_row, target.m_column).m_rowDiff;
        int columnDiff = m_request->m_windVectors[target.m_alt](target.m_row, target.m_column).m_columnDiff;

        from1.m_row += rowDiff;
        from1.m_column += columnDiff;
        if (from1.m_column < 0)
            from1.m_column += m_request->m_nmbColumns;
        from1.m_column = (from1.m_column % m_request->m_nmbColumns);

        std::vector<int> cycle = getShortestPath(from1, target, true);
        std::cout << "cycle length: " << cycle.size() << std::endl;

        std::vector<int> fullCycle;
        fullCycle.push_back(0);
        for (int m : cycle)
        {
            fullCycle.push_back(m);
        }

        int fullCycleIndex = 0;
        for (; turnIndex != m_request->m_nmbTurns; ++turnIndex)
        {
            m_altitudeMoves[turnIndex][b] = fullCycle[fullCycleIndex % fullCycle.size()];
            ++fullCycleIndex;
            //std::cout << "m " << fullCycle[fullCycleIndex] << std::endl;
        }
*/
    }
}


void Response::visualize()
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


    std::vector<Coordinate> targets = getBalloonTargets();
    for(size_t i=0;i<targets.size();++i){ //Draws a 3 pixel diameter circle for each target
        writer.drawCircle(targets[i].m_column,targets[i].m_row,1,3);
        //std::cout<<"("<<targets[i].m_row<<","<<targets[i].m_column<<")\n";
    }
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
std::vector<int> Response::getShortestPath(Coordinate a_from, Coordinate a_to, bool a_checkAltitude)
{
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

        bool altOk = !a_checkAltitude || currentQE.m_nodeIndex.m_alt == a_to.m_alt;
        if (currentQE.m_nodeIndex.m_row == a_to.m_row &&
            currentQE.m_nodeIndex.m_column == a_to.m_column &&
            altOk)
        {
            a_to.m_alt = currentQE.m_nodeIndex.m_alt;
            foundTarget = true;
            //std::cout << "found target" << std::endl;
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
            ASSERT(neighborCoord.m_alt >= 0 && neighborCoord.m_alt < m_request->m_nmbAltitudes);

            NodeInfo& neighborNode = info[neighborCoord.m_alt](neighborCoord.m_row, neighborCoord.m_column);
            if (costWhenUsingThisNeighbor < neighborNode.m_cost)
            {
                neighborNode.m_predIndex = currentQE.m_nodeIndex;
                neighborNode.m_cost = costWhenUsingThisNeighbor;
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

        result.push_back(move);
        currentAlt =currentQE.m_alt;

    }

    if (info[a_to.m_alt](a_to.m_row, a_to.m_column).m_cost == std::numeric_limits<int>::max())
    {
        result.empty(); // path not found
    }

    std::reverse(begin(result), end(result));
    return result;
}


/**/

std::vector<Coordinate> Response::getBalloonTargets()
{
    std::vector<bool> isTaken(m_request->m_nmbTargetCells,false);

    std::vector<Coordinate> targets(m_request->m_nmbBallons);
    //For each ballon, we set a target.
    for(int b=0;b<m_request->m_nmbBallons;++b){//South America
        //One third is going to South America, one third to Africa, the remainings in Australia
        if(b<m_request->m_nmbBallons/3){
            //All the targets for which the column index is between 0 and 100
            for(int tg=0;tg<m_request->m_nmbTargetCells;tg++){

                int colonne = m_request->m_targetCells[tg].m_column;
                if(colonne <=100){//Here, we are going to send one half to the north and the other one to the south
                    int ligne = m_request->m_targetCells[tg].m_row;
                    if(ligne<=37){//Improvement : compute the median row value
                        if(b<m_request->m_nmbBallons/6 && isTaken[tg]==false){
                            targets[b]=m_request->m_targetCells[tg];
                            //std::cout<<"("<<targets[b].m_row<<","<<targets[b].m_column<<")\n";
                            isTaken[tg]=true;
                            break;
}
                    }
                    else{
                        if(b>=m_request->m_nmbBallons/6 && isTaken[tg]==false){
                            targets[b]=m_request->m_targetCells[tg];
                            isTaken[tg]=true;
                            break;
                        }
                    }
                }
            }

        }
        else if(b<2*m_request->m_nmbBallons/3){//Africa
            //All the targets for which the column index is between 101 and 200

            for(int tg=0;tg<m_request->m_nmbTargetCells;tg++){
                int colonne = m_request->m_targetCells[tg].m_column;
                if(colonne <=200){//Here, we are going to send one half to the north and the other one to the south
                    int ligne = m_request->m_targetCells[tg].m_row;
                    if(ligne<=10){//Improvement : compute the median row value
                        if(b<m_request->m_nmbBallons/2 && isTaken[tg]==false){
                            targets[b]=m_request->m_targetCells[tg];
                            isTaken[tg]=true;
                            break;
                        }
                    }
                    else{
                        if(b>=m_request->m_nmbBallons/2 && isTaken[tg]==false){
                            targets[b]=m_request->m_targetCells[tg];
                            isTaken[tg]=true;
                            break;
                        }
                    }
                }
            }

        }
        else{//All the targets for which the column index is between 201 and 300
            for(int tg=0;tg<m_request->m_nmbTargetCells;tg++){//Australia
                int colonne = m_request->m_targetCells[tg].m_column;
                if(colonne <=300){//Here, we are going to send one half to the north and the other one to the south
                    int ligne = m_request->m_targetCells[tg].m_row;
                    if(ligne<=15){//Improvement : compute the median row value
                        if(b<5*m_request->m_nmbBallons/6 && isTaken[tg]==false){
                            targets[b]=m_request->m_targetCells[tg];
                            isTaken[tg]=true;
                            break;
                        }
                    }
                    else{
                        if(b>=5*m_request->m_nmbBallons/6 && isTaken[tg]==false){
                            targets[b]=m_request->m_targetCells[tg];
                            isTaken[tg]=true;
                            break;
                        }
                    }
                }
            }
        }
      }

    return targets;

}

