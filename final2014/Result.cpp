#include "Result.h"
#include "Request.h"
#include "SVGWriter.h"
#include <tuple>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>


Result::Result(const Request& a_request)
:m_request(&a_request)
, m_isStreetTraversed(m_request->m_streets.size(),false)
, m_usedCarSeconds(m_request->m_nmbCars, 0)
{
    m_itineraries.resize(m_request->m_nmbCars);
    //Initializing itinieraries
    for(size_t i=0;i<m_request->m_nmbCars;++i)
        m_itineraries[i].push_back(m_request->m_initialJunctionIndex);
}


void Result::visualize() const
{
    SVGWriter writer("visualize.html");

    for (const Street& str : m_request->m_streets)
    {
        writer.drawLine(m_request->m_junctions[str.m_junction1Index].m_lat,
                        m_request->m_junctions[str.m_junction1Index].m_long,
                        m_request->m_junctions[str.m_junction2Index].m_lat,
                        m_request->m_junctions[str.m_junction2Index].m_long,
                        0);
    }
/*
    for (size_t j = 0; j != m_request->m_junctions.size()/3; ++j)
    {
        size_t carIndex = 0;
        for (; carIndex < m_request->m_nmbCars; ++carIndex)
        {
            if (carCanUseJunction(carIndex, j))
                break;
        }

        writer.drawCircle(m_request->m_junctions[j].m_lat,
                          m_request->m_junctions[j].m_long,
                          carIndex + 1, 1);
    }
*/
    for (size_t carIndex = 0; carIndex < m_request->m_nmbCars; ++carIndex)
    {
        for (size_t j : m_itineraries[carIndex])
        {
            writer.drawCircle(m_request->m_junctions[j].m_lat,
                              m_request->m_junctions[j].m_long,
                              carIndex + 1,
                              4);
        }
    }

}


void Result::searchGreedilySeb()
{
    size_t randomizer = 0;
    for (size_t c = 0; c < m_request->m_nmbCars; ++c)
    {
        bool remainingSeconds = true;
        while (remainingSeconds)
        {
            size_t currentJunction = m_itineraries[c].back();

            bool foundDrivableStreet = false;
            size_t nextJunction = 0;

            for (size_t i = 0; i != m_request->m_adjacentStreetIndices[currentJunction].size(); ++i)
            {
                size_t s = m_request->m_adjacentStreetIndices[currentJunction][(i + randomizer) % m_request->m_adjacentStreetIndices[currentJunction].size()];
                const Street& str = m_request->m_streets[s];
                if (m_usedCarSeconds[c] + str.m_cost < m_request->m_availableSecondsPerCar)
                {
                    if (!foundDrivableStreet)
                    {
                        nextJunction = str.getOppositeJunction(currentJunction);
                        foundDrivableStreet = true;
                    }

                    if (!m_isStreetTraversed[s])
                        nextJunction = str.getOppositeJunction(currentJunction);
                }
            }

            if (foundDrivableStreet)
                addJunction(c,nextJunction);

            remainingSeconds = foundDrivableStreet;
            ++randomizer;
        }
    }
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


std::string Result::getJunctionCoordinatesForTikz(size_t a_junctionIndex) const
{
    std::stringstream str;

    str << "(" << 100 * (m_request->m_junctions[a_junctionIndex].m_lat - m_request->m_junctions[0].m_lat)
        << ", "
        << 100 * (m_request->m_junctions[a_junctionIndex].m_long - m_request->m_junctions[0].m_long) << ")";

    return str.str();
}


void Result::addJunction(size_t a_carIndex, size_t a_junctionIndex)
{
    bool found = false;

    for (size_t s : m_request->m_adjacentStreetIndices[m_itineraries[a_carIndex].back()])
    {
        const Street& str = m_request->m_streets[s];

        if (str.getOppositeJunction(m_itineraries[a_carIndex].back()) != a_junctionIndex)
            continue;

        found = true;
        m_itineraries[a_carIndex].push_back(a_junctionIndex);
        if (!m_isStreetTraversed[s])
        {
            m_totalDistance += str.m_length;
            m_isStreetTraversed[s] = true;
        }
        m_usedCarSeconds[a_carIndex] += str.m_cost;
        break;
    }

    if (!found)
    {
        std::cerr << "addJunction invalid " << std::endl;
        exit(1);
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
