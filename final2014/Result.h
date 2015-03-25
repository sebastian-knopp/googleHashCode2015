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

    void visualize();


private:

    bool carCanUseJunction(size_t a_carIndex, size_t a_junctionIndex) const;



public:

    const Request* m_request = nullptr;

    Itineraries m_itineraries;

};



std::ostream& operator<<(std::ostream& a_is, const Result& a_result);


