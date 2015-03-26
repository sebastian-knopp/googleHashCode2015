#include "Result.h"
#include "Request.h"
#include "SVGWriter.h"
#include <tuple>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>


Result::Result(const Request& a_request)
:m_request(&a_request)
, m_isStreetTraversed(m_request->m_streets.size(), false)
, m_usedCarSeconds(m_request->m_nmbCars, 0)
, m_itineraries(m_request->m_nmbCars)
, m_carAssignedToStreet(m_request->m_streets.size(), 0)
{
    for (Itinerary& it : m_itineraries)
    {
        it.push_back(m_request->m_initialJunctionIndex);
    }

    std::vector<size_t> streetPermutation;
    streetPermutation.reserve(m_request->m_streets.size());
    for (size_t i = 0; i != m_request->m_streets.size(); ++i)
    {
        streetPermutation.push_back(i);
    }

    std::sort(begin(streetPermutation), end(streetPermutation), [this] (size_t lhs, size_t rhs){

        const double angleLhs = getAngle(m_request->getOrigin(), m_request->m_junctions[m_request->m_streets[lhs].m_junction1Index]);
        const double angleRhs = getAngle(m_request->getOrigin(), m_request->m_junctions[m_request->m_streets[rhs].m_junction1Index]);
        return angleLhs < angleRhs;

    });

    int overallCost = 0;
    int overallLength = 0;
    for (const size_t s : streetPermutation)
    {
        overallCost += m_request->m_streets[s].m_cost;
        overallLength += m_request->m_streets[s].m_length;
    }
    std::cout << "overall cost: " << overallCost << std::endl;
    std::cout << "overall length: " << overallLength << std::endl;

    int costPerCar = overallCost / m_request->m_nmbCars;
    int currentCost = 0;
    size_t carIndex = 0;
    for (size_t i = 0; i != m_request->m_streets.size(); ++i)
    {
        m_carAssignedToStreet[streetPermutation[i]] = carIndex;
        currentCost += m_request->m_streets[streetPermutation[i]].m_cost;
        if (currentCost > costPerCar)
        {
            currentCost -= costPerCar;
            ++carIndex;
        }
    }
}


void Result::visualize() const
{
    SVGWriter writer("visualize.html", 1200, 10);
/*
    for (size_t j = 0; j != m_request->m_junctions.size(); ++j)
    {
        size_t carIndex = 0;
        for (; carIndex < m_request->m_nmbCars; ++carIndex)
        {
            if (carCanUseJunction(carIndex, j))
                break;
        }

        writer.drawCircle(m_request->m_junctions[j].m_long,
                          -m_request->m_junctions[j].m_lat,
                          carIndex + 1, 2);
    }
*/
    for (size_t sIndex = 0; sIndex != m_request->m_junctions.size(); ++sIndex)
    {
        const Street& str = m_request->m_streets[sIndex];

        writer.drawLine(m_request->m_junctions[str.m_junction1Index].m_long,
                        -m_request->m_junctions[str.m_junction1Index].m_lat,
                        m_request->m_junctions[str.m_junction2Index].m_long,
                        -m_request->m_junctions[str.m_junction2Index].m_lat,
                        m_isStreetTraversed[sIndex] ? 0 : 8);
    }

    for (size_t carIndex = 0; carIndex < m_request->m_nmbCars; ++carIndex)
    {
        for (size_t j : m_itineraries[carIndex])
        {
            writer.drawCircle(m_request->m_junctions[j].m_long,
                              -m_request->m_junctions[j].m_lat,
                              carIndex + 1,
                              4);
        }
    }

}


void Result::searchGreedily()
{
    size_t activeCars = m_request->m_nmbCars;
    std::vector<bool> carDone(m_request->m_nmbCars, false);

    for (size_t c = 0; ; )
    {
        while (!carDone[c])
        {
            const auto next = determineNextJunctions(c);

            if (next.empty())
                break;

            for (size_t j : next)
            {
                if (!addJunction(c, j))
                {
                    carDone[c] = true;
                    --activeCars;
                    if (activeCars == 0)
                        return;

                    break;
                }
            }
        }
        c = (c + 1) % m_request->m_nmbCars;
    }
}


bool Result::carCanUseJunction(size_t a_carIndex, size_t a_junctionIndex) const
{
    for (size_t s : m_request->m_adjacentStreetIndices[a_junctionIndex])
    {
        if (m_carAssignedToStreet[s] == a_carIndex)
            return true;
    }
    return false;
}


bool Result::addJunction(size_t a_carIndex, size_t a_junctionIndex)
{
    bool found = false;

    for (size_t s : m_request->m_adjacentStreetIndices[m_itineraries[a_carIndex].back()])
    {
        const Street& str = m_request->m_streets[s];

        if (str.getOppositeJunction(m_itineraries[a_carIndex].back()) != a_junctionIndex)
            continue;

        if (m_usedCarSeconds[a_carIndex] + str.m_cost > m_request->m_availableSecondsPerCar)
        {
            return false;
        }

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
        std::cerr << "addJunction invalid " << a_junctionIndex << std::endl;
        exit(1);
    }

    return true;
}


std::vector<size_t> Result::determineNextJunctions(size_t a_carIndex)
{
    size_t currentJunction = m_itineraries[a_carIndex].back();
    double maxDist = -1l;
    size_t i1 = 0;
    size_t i2 = 0;

    for (size_t j = 0; j != m_request->m_streets.size(); ++j)
    {
        if (!m_isStreetTraversed[j] && carCanUseJunction(a_carIndex, m_request->m_streets[j].m_junction1Index))
        {
            double dist = getDistance(m_request->m_junctions[currentJunction], m_request->m_junctions[m_request->m_streets[j].m_junction1Index]);
            if (dist > maxDist && m_request->m_streets[j].m_junction1Index != currentJunction)
            {
                i1 = m_request->m_streets[j].m_junction1Index;
                i2 = m_request->m_streets[j].m_junction2Index;
                maxDist = dist;
            }
        }
    }

    if (maxDist < 0)
        return std::vector<size_t>();

    std::vector<size_t> result = getShortestPath(currentJunction, i1, a_carIndex);

    if (result.size() > 1 && result[result.size() - 1] != i2)
        result.push_back(i2);

    return result;
}


std::vector<size_t> Result::getShortestPath(size_t a_fromJunction, size_t a_toJunction, size_t a_carIndex)
{
    if (a_fromJunction == a_toJunction)
        return std::vector<size_t>();

    auto getStreetCost = [&] (const size_t a_currentJunction, const size_t a_streetIndex) {
        int m = 1;
        if (!carCanUseJunction(a_carIndex, m_request->m_streets[a_streetIndex].getOppositeJunction(a_currentJunction)))
            m = 5;

        if (m_isStreetTraversed[a_streetIndex])
            return 10 * m * m_request->m_streets[a_streetIndex].m_cost;

        return m * m_request->m_streets[a_streetIndex].m_cost / (m_request->m_streets[a_streetIndex].m_length);
    };

    struct NodeInfo
    {
        int m_cost = std::numeric_limits<int>::max();
        size_t m_predIndex = std::numeric_limits<size_t>::max();
    };

    struct PQEntry
    {
        int m_cost;
        size_t m_nodeIndex;

        bool operator< (const PQEntry& e) const
        {
            return m_cost > e.m_cost;
        }
    };

    std::vector<NodeInfo> info(m_request->m_junctions.size());
    info[a_fromJunction].m_cost = 0;

    std::priority_queue<PQEntry> q;

    q.push( PQEntry { 0, a_fromJunction });
    while (!q.empty())
    {
        const PQEntry currentJunction = q.top();
        if (currentJunction.m_nodeIndex == a_toJunction)
            break;

        q.pop();
        if (info[currentJunction.m_nodeIndex].m_cost < currentJunction.m_cost)
            continue; // node already settled

        for (size_t s : m_request->m_adjacentStreetIndices[currentJunction.m_nodeIndex])
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
    size_t currentJunction = a_toJunction;
    while (currentJunction != std::numeric_limits<size_t>::max())
    {
        result.push_back(currentJunction);
        currentJunction = info[currentJunction].m_predIndex;
    }

    if (result.back() != a_fromJunction)
        result.empty(); // path not found
    else
        result.pop_back();

    std::reverse(begin(result), end(result));
    return result;
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

