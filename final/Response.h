#pragma once
#include <ostream>

class Request;


class Response {

    friend std::ostream& operator<<(std::ostream&, const Response&);

public:

    explicit Response(const Request& a_request);

    void solve();

    void visualize() const;


private:

    int m_result = 0;

    const Request* m_request;

};


std::ostream& operator<<(std::ostream& a_is, const Response& a_request);

