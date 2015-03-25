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
        writer.drawLine(m_request->m_junctions[str.m_junction1Index].m_long,
                        -m_request->m_junctions[str.m_junction1Index].m_lat,
                        m_request->m_junctions[str.m_junction2Index].m_long,
                        -m_request->m_junctions[str.m_junction2Index].m_lat,
                        0);
    }

    for (size_t j = 0; j != m_request->m_junctions.size()/3; ++j)
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
    for (size_t c = 0; c < m_request->m_nmbCars; ++c)
    {
        bool carDone = false;
        while (!carDone)
        {
            const auto next = determineNextJunctions(c);
            ++m_randomizer;

            if (next.empty())
                break;

            for (size_t j : next)
            {
                if (!addJunction(c, j))
                {
                    carDone = true;
                    break;
                }
            }
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
        {
        SVGWriter writer("invalid.html");

        for (const Street& str : m_request->m_streets)
        {
            writer.drawLine(m_request->m_junctions[str.m_junction1Index].m_long,
                            -m_request->m_junctions[str.m_junction1Index].m_lat,
                            m_request->m_junctions[str.m_junction2Index].m_long,
                            -m_request->m_junctions[str.m_junction2Index].m_lat,
                            0);
        }

        writer.drawCircle(m_request->m_junctions[a_junctionIndex].m_long,
                          -m_request->m_junctions[a_junctionIndex].m_lat,
                          2,
                          3);

        writer.drawCircle(m_request->m_junctions[m_itineraries[a_carIndex].back()].m_long,
                          -m_request->m_junctions[m_itineraries[a_carIndex].back()].m_lat,
                          3,
                          3);

        writer.drawCircle(m_request->m_junctions[5065].m_long,
                          -m_request->m_junctions[5065].m_lat,
                          4,
                          1);

        std::cerr << "addJunction invalid " << a_junctionIndex << std::endl;
        }
        exit(1);
    }

    return true;
}


std::vector<size_t> Result::determineNextJunctions(size_t a_carIndex)
{
    size_t currentJunction = m_itineraries[a_carIndex].back();
    {
        double maxDist = 0l;
        size_t i = 0;
        for (size_t j = 0; j != m_request->m_junctions.size(); ++j)
        {
            if (carCanUseJunction(a_carIndex, j))
            {
                double dist = getDistance(m_request->m_junctions[currentJunction], m_request->m_junctions[j]);
                if (dist > maxDist)
                {
                    i = j;
                    maxDist = dist;
                }
            }
        }
        return getShortestPath(currentJunction, i);
    }
}


std::vector<size_t> Result::getShortestPath(size_t a_fromJunction, size_t a_toJunction)
{
    if (a_fromJunction == a_toJunction)
        return std::vector<size_t>();

    auto getStreetCost = [&] (const size_t a_streetIndex) {
        if (m_isStreetTraversed[a_streetIndex])
            return m_request->m_streets[a_streetIndex].m_cost;

        return m_request->m_streets[a_streetIndex].m_cost / m_request->m_streets[a_streetIndex].m_length;
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
            const int costWhenUsingThisStreet = info[currentJunction.m_nodeIndex].m_cost + getStreetCost(s);

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

