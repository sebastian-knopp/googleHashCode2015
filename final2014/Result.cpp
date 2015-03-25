#include "Result.h"
#include "Request.h"



Result::Result(const Request& a_request)
:m_request(&a_request)
{
}


bool Result::carCanUseJunction(size_t a_carIndex, size_t a_junctionIndex) const
{
    return true;
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
        size_t currentJunctionIndex=m_itineraries[currentCarIndex][m_itineraries[currentCarIndex].size()-1];
        //While we do not find an available adjacent street that is not crossed and that we have time to cross, we check the following ones
        while(m_request->m_streets[m_request->m_adjacentStreetIndices[currentJunctionIndex][j]].m_isCrossed==true
              && j<m_request->m_adjacentStreetIndices[currentJunctionIndex].size()
              && timeSpanForEachCar[currentCarIndex]+m_request->m_streets[m_request->m_adjacentStreetIndices[currentJunctionIndex][j]].m_cost
              > m_request->m_availableSecondsPerCar
              && !carCanUseJunction(currentCarIndex,currentJunctionIndex))
            j++;
        if(j<m_request->m_adjacentStreetIndices[currentJunctionIndex].size()){//Means we found one that is not crossed and we
            //have time to cross it, we add it
            size_t newStreetIndex=m_request->m_adjacentStreetIndices[currentJunctionIndex][j];
            //Here, we have to check which junction is the following one in order to add it
            if(m_request->m_streets[newStreetIndex].m_junction1Index == currentJunctionIndex)
                result.m_itineraries[currentCarIndex].push_back(m_request->m_streets[newStreetIndex].m_junction2Index);
            else
                result.m_itineraries[currentCarIndex].push_back(m_request->m_streets[newStreetIndex].m_junction1Index);
            //The street is now crossed
            m_request->m_streets[newStreetIndex].m_isCrossed=true;
            //The distance is updated
            totalDistance+=m_request->m_streets[newStreetIndex].m_length;
            //The time assigned to the car is updated
            timeSpanForEachCar[currentCarIndex]+=m_request->m_streets[newStreetIndex].m_cost;
        }
        else{//Otherwise, either nothing can be crossed due to time limit, or all streets are crossed.
            //We loop again by relaxing crossed-state condition
            j=0;
            while(j<m_request->m_adjacentStreetIndices[currentJunctionIndex].size()
                  && !carCanUseJunction(currentCarIndex,currentJunctionIndex)
                  && timeSpanForEachCar[currentCarIndex]
                  + m_request->m_streets[m_request->m_adjacentStreetIndices[currentJunctionIndex][j]].m_cost
                  > m_request->m_availableSecondsPerCar)
                j++;

            if(j<m_request->m_adjacentStreetIndices[currentJunctionIndex].size()){//Means we found one that we have time to cross, we add it
                size_t newStreetIndex=m_request->m_adjacentStreetIndices[currentJunctionIndex][j];
                //Here, we have to check which junction is the following one in order to add it
                if(m_request->m_streets[newStreetIndex].m_junction1Index == currentJunctionIndex)
                    result.m_itineraries[currentCarIndex].push_back(m_request->m_streets[newStreetIndex].m_junction2Index);
                else
                    result.m_itineraries[currentCarIndex].push_back(m_request->m_streets[newStreetIndex].m_junction1Index);
                //The distance is updated if it is crossed for the first time
                if(m_request->m_streets[newStreetIndex].m_isCrossed==false)
                    totalDistance+=m_request->m_streets[newStreetIndex].m_length;
                //The street is now crossed
                m_request->m_streets[newStreetIndex].m_isCrossed=true;
                //The time assigned to the car is updated
                timeSpanForEachCar[currentCarIndex]+=m_request->m_streets[newStreetIndex].m_cost;
            }
            else{//There is nothing we can do, so we stop there for that car
                timeSpanForEachCar[currentCarIndex]=m_request->m_availableSecondsPerCar;
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
    }
}
