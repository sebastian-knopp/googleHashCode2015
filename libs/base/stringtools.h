#pragma once

#include <string>
#include <sstream>


namespace base {
namespace stringtools {


namespace detail {

inline void addToStream(std::stringstream& /*a_stream*/)
{
}

template<typename T, typename... Args>
void addToStream(std::stringstream& a_stream, const T& a_value, Args... a_args)
{
    a_stream << a_value;
    addToStream(a_stream, a_args...);
}

} // namespace detail


template<typename... Args>
std::string concat(Args... a_args)
{
    std::stringstream s;
    detail::addToStream(s, a_args...);
    return s.str();
}


template<typename T>
std::string join(const T& a_container, std::string a_separator)
{
    std::stringstream s;

    auto it = begin(a_container);
    const auto endIt = end(a_container);

    if (it != endIt)
    {
        s << *it;
        ++it;
    }

    while (it != endIt)
    {
        s << a_separator;
        s << *it;
        ++it;
    }

    return s.str();
}


} // namespace stringtools
} // namespace base

