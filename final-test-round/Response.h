#pragma once
#include <ostream>
#include <vector>
#include "Slice.h"


class Request;


class Response {

    friend std::ostream& operator<<(std::ostream&, const Response&);

public:

    explicit Response(const Request& a_request);

    void solve();

    void calcSlice(const Slice a_slice);

    void visualize() const;

    size_t getOverallCells();

private:

    const Request* m_request;

    std::vector<Slice> m_slices;

    size_t m_overallCells = 0;

};


std::ostream& operator<<(std::ostream& a_os, const Response& a_request);

std::ostream& operator<<(std::ostream& a_os, const Slice& a_slice);

