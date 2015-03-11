#pragma once


#include <boost/range/algorithm/adjacent_find.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/unique.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/counting_range.hpp>
#include <vector>
#include <set>
#include <map>


namespace base {
namespace algorithm {


//! Returns if the given vector is unique
template<typename T>
bool isUnique(const std::vector<T>& a_vector)
{
    using namespace boost;
    auto temp = a_vector;
    return adjacent_find<return_found>(sort(temp)) == std::end(temp);
}


//! Sorts the given vector and removes all duplicates
template<typename T>
void makeUnique(std::vector<T>& a_vector)
{
    using namespace boost;
    auto newEnd = unique<return_found>(sort(a_vector));
    a_vector.erase(newEnd, std::end(a_vector));
}


//! Returns if a vector contains a given element.
template<typename ContainerT, typename ValueT>
bool contains(const ContainerT& a_vector, const ValueT& a_element)
{
    using namespace boost;
    return std::end(a_vector) != find(a_vector, a_element);
}


//! Returns if a set contains a given element.
template<typename T>
bool contains(const std::set<T>& a_set, const T& a_element)
{
    return end(a_set) != a_set.find(a_element);
}


//! Returns if a map contains a given element.
template<typename T, typename U>
bool contains(const std::map<T, U>& a_set, const T& a_element)
{
    return end(a_set) != a_set.find(a_element);
}


//! Removes all elements for which the predicate is true.
template<typename T, typename PredicateT>
void removeIf(T& a_vector, PredicateT a_predicate)
{
    auto newEnd = std::remove_if(begin(a_vector), end(a_vector), a_predicate);
    a_vector.erase(newEnd, std::end(a_vector));
}


template<typename T>
void setMin(T& a_minimum, const T& a_value)
{
    if (a_value < a_minimum)
        a_minimum = a_value;
}


template<typename T>
void setMax(T& a_maximum, const T& a_value)
{
    if (a_value > a_maximum)
        a_maximum = a_value;
}


template<typename T>
auto indices(const T& a_container) -> decltype(boost::counting_range(size_t(0), a_container.size()))
{
    return boost::counting_range(size_t(0), a_container.size());
}


} // namespace algorithm
} // namespace base



