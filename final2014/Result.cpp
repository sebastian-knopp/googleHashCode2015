#include "Result.h"
#include "Request.h"
#include <tuple>
#include <algorithm>
#include <fstream>
#include <iostream>


Result::Result(const Request& a_request)
:m_request(&a_request)
, m_isStreetTraversed(m_request->m_streets.size(),false)
, m_usedCarSeconds(m_request->m_nmbCars, 0)
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


void Result::addJunction(size_t a_carIndex, size_t a_junctionIndex)
{
    for (size_t s : m_request->m_adjacentStreetIndices[a_junctionIndex])
    {
        const Street& str = m_request->m_streets[s];
        size_t oppositeJunctionIndex = str.m_junction1Index;

        if (str.m_junction1Index == a_junctionIndex)
            oppositeJunctionIndex = str.m_junction2Index;

        if (oppositeJunctionIndex != m_itineraries[a_carIndex].back())
        {
            std::cerr << "addJunction invalid" << std::endl;
            exit(1);
        }

        m_itineraries[a_carIndex].push_back(a_junctionIndex);
        if (!m_isStreetTraversed[s])
        {
            m_totalDistance += str.m_length;
            m_isStreetTraversed[s] = true;
        }
    }
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

//Abdel
//Creating an initial solution, by just using a greedy search
void Result::searchGreedily(){
    //The cost of the solution
    int totalDistance=0;
    std::vector<int> timeSpanForEachCar(m_request->m_nmbCars,0);
    int minTimeSpan=0;
    m_itineraries.resize(m_request->m_nmbCars);
    //Initializing itinieraries
    for(size_t i=0;i<m_request->m_nmbCars;++i)
        m_itineraries[i].push_back(m_request->m_initialJunctionIndex);
    //Start with vehicle 0
    int currentCarIndex=0;
    //To know if we continue or not
    bool go=true;
    //Main loop, we stop when the limit is reached
    while(go){
        //Choosing next street for current car
        size_t j=0;
        size_t currentJunctionIndex=m_itineraries[currentCarIndex].back();
        //While we do not find an available adjacent street that is not traversed and that we have time to go to, we check the following ones
        while(j<m_request->m_adjacentStreetIndices[currentJunctionIndex].size()
              &&(m_isStreetTraversed[m_request->m_adjacentStreetIndices[currentJunctionIndex][j]]==true
              || timeSpanForEachCar[currentCarIndex]+m_request->m_streets[m_request->m_adjacentStreetIndices[currentJunctionIndex][j]].m_cost
              > m_request->m_availableSecondsPerCar || !carCanUseJunction(currentCarIndex,currentJunctionIndex) ) )
            j++;
        if(j<m_request->m_adjacentStreetIndices[currentJunctionIndex].size()){//Means we found one that is not crossed and we
            //have time to cross it, we add it
            size_t newStreetIndex=m_request->m_adjacentStreetIndices[currentJunctionIndex][j];
            //Here, we have to check which junction is the following one in order to add it
            if(m_request->m_streets[newStreetIndex].m_junction1Index == currentJunctionIndex)
                m_itineraries[currentCarIndex].push_back(m_request->m_streets[newStreetIndex].m_junction2Index);
            else
                m_itineraries[currentCarIndex].push_back(m_request->m_streets[newStreetIndex].m_junction1Index);
            //The street is now crossed
            m_isStreetTraversed[newStreetIndex]=true;
            //The distance is updated
            totalDistance+=m_request->m_streets[newStreetIndex].m_length;
            //The time assigned to the car is updated
            timeSpanForEachCar[currentCarIndex]+=m_request->m_streets[newStreetIndex].m_cost;
            //Display
            std::cout<<"Vehicle "<<currentCarIndex <<" : from junction "<< currentJunctionIndex
               <<" to junction "<<m_itineraries[currentCarIndex][m_itineraries[currentCarIndex].size()-1]<<std::endl;
        }
        else{//Otherwise, either nothing can be crossed due to time limit, or all streets are traversed.
            //We loop again by relaxing traversed condition
            j=0;
            bool stop=false;
            //Here, we check that the current junction is not already visited
            while(!stop){
                while(j<m_request->m_adjacentStreetIndices[currentJunctionIndex].size()
                      && (!carCanUseJunction(currentCarIndex,currentJunctionIndex)
                          || timeSpanForEachCar[currentCarIndex]
                          + m_request->m_streets[m_request->m_adjacentStreetIndices[currentJunctionIndex][j]].m_cost
                          > m_request->m_availableSecondsPerCar) ){
                    j++;
                }
                //Here, we found a street that is traversable, but already traversed. Let us check
                //that it does not lead to an already visited junction for this car
                if(j<m_request->m_adjacentStreetIndices[currentJunctionIndex].size()){
                    size_t possibleStreetIndex=m_request->m_adjacentStreetIndices[currentJunctionIndex][j];
                    size_t possibleJunctionIndex;
                    if(m_request->m_streets[possibleStreetIndex].m_junction1Index == currentJunctionIndex)
                        possibleJunctionIndex=m_request->m_streets[possibleStreetIndex].m_junction2Index;
                    else
                        possibleJunctionIndex=m_request->m_streets[possibleStreetIndex].m_junction1Index;
                    if(std::find(m_itineraries[currentCarIndex].begin(),m_itineraries[currentCarIndex].end(),possibleJunctionIndex)
                            != m_itineraries[currentCarIndex].end()){ //The new junction is already visited
                        j++;
                    }
                    else{
                        stop=true;
                    }
                }
                else
                    stop=true;
            }

            if(j<m_request->m_adjacentStreetIndices[currentJunctionIndex].size()){
                //Means we found one that we have time to cross and is already traversed
                size_t newStreetIndex=m_request->m_adjacentStreetIndices[currentJunctionIndex][j];
                //Here, we have to check which junction is the following one in order to add it
                if(m_request->m_streets[newStreetIndex].m_junction1Index == currentJunctionIndex)
                    m_itineraries[currentCarIndex].push_back(m_request->m_streets[newStreetIndex].m_junction2Index);
                else
                    m_itineraries[currentCarIndex].push_back(m_request->m_streets[newStreetIndex].m_junction1Index);
                //The distance is updated if it is crossed for the first time
                if(m_isStreetTraversed[newStreetIndex]==false)
                    totalDistance+=m_request->m_streets[newStreetIndex].m_length;
                //The street is now crossed
                m_isStreetTraversed[newStreetIndex]=true;
                //The time assigned to the car is updated
                timeSpanForEachCar[currentCarIndex]+=m_request->m_streets[newStreetIndex].m_cost;
                //Display
                std::cout<<"Vehicle "<<currentCarIndex <<" : from junction "<< currentJunctionIndex
                   <<" to junction "<<m_itineraries[currentCarIndex][m_itineraries[currentCarIndex].size()-1]<<std::endl;
            }
            else{//Here, every reachable junction is already visited so we take the first one that is available
                j=0;
                while(j<m_request->m_adjacentStreetIndices[currentJunctionIndex].size()
                      && (!carCanUseJunction(currentCarIndex,currentJunctionIndex)
                          || timeSpanForEachCar[currentCarIndex]
                          + m_request->m_streets[m_request->m_adjacentStreetIndices[currentJunctionIndex][j]].m_cost
                          > m_request->m_availableSecondsPerCar) ){
                    j++;
                }
                if(j<m_request->m_adjacentStreetIndices[currentJunctionIndex].size()){
                    //Means we found one that we have time to cross and is already traversed
                    size_t newStreetIndex=m_request->m_adjacentStreetIndices[currentJunctionIndex][j];
                    //Here, we have to check which junction is the following one in order to add it
                    if(m_request->m_streets[newStreetIndex].m_junction1Index == currentJunctionIndex)
                        m_itineraries[currentCarIndex].push_back(m_request->m_streets[newStreetIndex].m_junction2Index);
                    else
                        m_itineraries[currentCarIndex].push_back(m_request->m_streets[newStreetIndex].m_junction1Index);
                    //The distance is updated if it is crossed for the first time
                    if(m_isStreetTraversed[newStreetIndex]==false)
                        totalDistance+=m_request->m_streets[newStreetIndex].m_length;
                    //The street is now crossed
                    m_isStreetTraversed[newStreetIndex]=true;
                    //The time assigned to the car is updated
                    timeSpanForEachCar[currentCarIndex]+=m_request->m_streets[newStreetIndex].m_cost;
                    //Display
                    std::cout<<"Vehicle "<<currentCarIndex <<" : from junction "<< currentJunctionIndex
                       <<" to junction "<<m_itineraries[currentCarIndex][m_itineraries[currentCarIndex].size()-1]<<std::endl;
                }
                else{
                    //There is nothing we can do, so we stop there for that car
                    timeSpanForEachCar[currentCarIndex]=m_request->m_availableSecondsPerCar;
                    std::cout<<"Vehicle "<<currentCarIndex<<" is full\n\n\n\n";
                }
            }
        }

        //Just changing cars for next iteration
        currentCarIndex++;
        if(currentCarIndex==m_request->m_nmbCars)
            currentCarIndex=0;

        //Here, we check if we should continue
        minTimeSpan=timeSpanForEachCar[0];
        for(size_t it=1;it<m_request->m_nmbCars;++it)
            if(timeSpanForEachCar[it]<minTimeSpan)
                minTimeSpan=timeSpanForEachCar[it];
        //If all vehicles are full
        if(minTimeSpan==m_request->m_availableSecondsPerCar)
            go=false;

        std::cout<<"Distance "<<totalDistance<<"\n\n";
    }
}
