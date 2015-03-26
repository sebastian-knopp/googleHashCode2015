#pragma once

#include <vector>
#include <ostream>
#include <stdint.h>


//! List of junction indices visited by one car.
typedef std::vector<size_t> Itinerary;
typedef std::vector<Itinerary> Itineraries;


class Request;

class Result {

public:

    explicit Result(const Request& a_request);

    void visualize() const;

    void searchGreedily();


private:

    bool carCanUseJunction(size_t a_carIndex, size_t a_junctionIndex) const;

    //! Adds an adjacent junction to the itinary of the given car.
    bool addJunction(size_t a_carIndex, size_t a_junctionIndex);

    std::vector<size_t> determineNextJunctions(size_t a_carIndex);

    std::vector<size_t> getShortestPath(size_t a_fromJunction, size_t a_toJunction, size_t a_carIndex);


public:

    const Request* m_request = nullptr;

    std::vector<bool> m_isStreetTraversed; //For each street, says if it is traversed or not

    int m_totalDistance = 0;

    std::vector<int> m_usedCarSeconds;

    Itineraries m_itineraries;

    std::vector<size_t> m_carAssignedToStreet;

};



std::ostream& operator<<(std::ostream& a_is, const Result& a_result);



