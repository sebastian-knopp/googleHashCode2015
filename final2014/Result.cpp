#include "Result.h"
#include "Request.h"
#include <tuple>
#include <algorithm>


Result::Result(const Request& a_request)
:m_request(&a_request)
{
}


bool Result::carCanUseJunction(size_t a_carIndex, size_t a_junctionIndex) const
{
    if (getDistance(m_request->getOrigin(), m_request->m_junctions[a_junctionIndex]) < 100)
        return true;

    double angle = getAngle(m_request->getOrigin(), m_request->m_junctions[a_junctionIndex]);
    double minAngle;
    double maxAngle;
    std::tie(minAngle, maxAngle) =
        std::minmax(static_cast<double>(a_carIndex + 1 % m_request->m_nmbCars) * pi() / static_cast<double>(m_request->m_nmbCars),
                    static_cast<double>(a_carIndex) * pi() / static_cast<double>(m_request->m_nmbCars));

    return minAngle <= angle && angle <= maxAngle;
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

