#pragma once
#include <stdexcept>
#include <iostream>


// #define ASSERT(_Expression)


#define ASSERT(_Expression) \
    { \
        if (!static_cast<bool>(_Expression)) \
        { \
            std::cerr << "Assertion:" << #_Expression << " failed (File: " << __FILE__ << ", line: " <<  __LINE__ << ")" << std::endl; \
            throw std::logic_error("Assertion failed"); \
        } \
    }



