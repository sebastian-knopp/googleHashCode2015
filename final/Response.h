#pragma once
#include <ostream>
#include "Request.h"


class Response {

    friend std::ostream& operator<<(std::ostream&, const Response&);

public:

    explicit Response(const Request& a_request);

    void solve();

    void visualize() const;

    std::vector<int> getShortestPath(Coordinate a_from, Coordinate a_to);

    //! Returns a vector with size == nmb Balloons
    std::vector<Coordinate> getBalloonTargets();


private:

    const Request* m_request;

    std::vector<std::vector<size_t>> m_altitudeMoves;

    Grid<int> m_reachableCoordinates;

};


std::ostream& operator<<(std::ostream& a_os, const Response& a_request);

