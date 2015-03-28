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
{
}



void Response::solve()
{
    srand(time(NULL));
    for (int b = 0; b != m_request->m_nmbBallons; ++b)
    {
        m_altitudeMoves[0][b] = 1;
    }

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

}


void Response::visualize()
{
    SVGWriter writer("visualize.html", 800, 30);

    writer.drawCircle(5, 12, 1, 20);
    writer.drawCircle(300, 12, 2, 20);

    writer.drawText(20, 10, "Google Hash 2015 - Final", 20);
    writer.drawText(20, 18, "Team: A211 - Mines Saint-Etienne - Gardanne - SFL", 14);


    std::vector<Coordinate> targets = getBalloonTargets();
    for(size_t i=0;i<targets.size();++i){ //Draws a 3 pixel diameter circle for each target
        writer.drawCircle(targets[i].m_column,targets[i].m_row,1,3);
        std::cout<<"("<<targets[i].m_row<<","<<targets[i].m_column<<")\n";
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
std::vector<int> Response::getShortestPath(Coordinate a_from, Coordinate a_to)
{
    if (a_from == a_to)
        return std::vector<int>();

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
        if (currentQE.m_nodeIndex == a_to)
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

