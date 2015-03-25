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

    void searchGreedilySeb();

    void searchGreedily();


private:

    bool carCanUseJunction(size_t a_carIndex, size_t a_junctionIndex) const;

    std::string getJunctionCoordinatesForTikz(size_t a_junctionIndex) const;

    //! Adds an adjacent junction to the itinary of the given car.
    void addJunction(size_t a_carIndex, size_t a_junctionIndex);

    std::vector<size_t> determineNextJunctions(size_t a_carIndex);


public:

    const Request* m_request = nullptr;

    std::vector<bool> m_isStreetTraversed; //For each street, says if it is traversed or not

    int m_totalDistance = 0;

    std::vector<int> m_usedCarSeconds;

    Itineraries m_itineraries;

    size_t m_randomizer = 0;

};



std::ostream& operator<<(std::ostream& a_is, const Result& a_result);



