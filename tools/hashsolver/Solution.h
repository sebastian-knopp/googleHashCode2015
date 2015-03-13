#pragma once

#include "PlacedServer.h"
#include "base/Vector2d.h"
#include <random>


class Request;


class Solution {

public:

    explicit Solution(const Request& a_request);

    size_t getRating() const;

    inline size_t getNmbRows() const
    {
        return m_assignedCapacity.width();
    }

    inline size_t getNmbPools() const
    {
        return m_assignedCapacity.height();
    }

    bool isAvailable(const Coordinate& a_coord, size_t size) const;

    void placeServer(const Coordinate& a_coord, size_t size);

    size_t getPoolCapacity(size_t a_pool) const;

    size_t getPoolWithMinCapacity(size_t a_row) const;


public:

    const Request* m_request;

    PlacedServers m_servers;

    //! capacity of a pool in a row
    base::Vector2d<size_t> m_assignedCapacity;

    base::Vector2d<size_t> m_isAssigned;


};


Solution simulatedAnnealing(std::mt19937& rndGenerator,
                            const Solution& a_solution,
                            double initialTemperature,
                            double coolingFactor,
                            size_t runs,
                            bool print);


//size_t getRating(const Request a_request, const Solution& a_solution);



std::ostream& operator<<(std::ostream& is, const Solution& a_solution);

