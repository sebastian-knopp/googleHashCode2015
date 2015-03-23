#include "stringtools.h"
#include <gtest/gtest.h>


namespace base {
namespace stringtools {


TEST(stringtoolsTest, concat)
{
    EXPECT_EQ("ABC", concat("A", "B", "C"));
    EXPECT_EQ("A123", concat("A", 12, 3));
}


TEST(stringtoolsTest, join)
{
    EXPECT_EQ("A",       join( std::vector<std::string> {"A"}, ","));
    EXPECT_EQ("A,B,C",   join( std::vector<std::string> {"A", "B", "C"}, ","));
    EXPECT_EQ("A1 - B2", join( std::vector<std::string> {"A1", "B2"}, " - "));
    EXPECT_EQ("",        join( std::vector<std::string> { }, ","));
}


} // namespace stringtools
} // namespace base

