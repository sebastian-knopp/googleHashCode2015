#pragma once
#include <ostream>
#include <vector>


struct Slice
{
    size_t m_row1;
    size_t m_row2;
    size_t m_column1;
    size_t m_column2;
};


class Request;


class Response {

    friend std::ostream& operator<<(std::ostream&, const Response&);

public:

    explicit Response(const Request& a_request);

    void solve();

    void visualize() const;


private:

    std::vector<Slice> m_slices;

    const Request* m_request;

};


std::ostream& operator<<(std::ostream& a_os, const Response& a_request);

std::ostream& operator<<(std::ostream& a_os, const Slice& a_slice);

