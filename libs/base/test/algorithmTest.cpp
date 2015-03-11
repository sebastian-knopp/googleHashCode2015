#include "algorithm.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::ElementsAre; // gmock


namespace base {
namespace algorithm {


TEST(algorithmTest, isUnique)
{
    std::vector<int> numbers = { 1, 9, 4, 3, 5};
    EXPECT_TRUE(isUnique(numbers));
}


TEST(algorithmTest, isNotUnique)
{
    std::vector<int> numbers = { 1, 9, 1, 2, 9};
    EXPECT_FALSE(isUnique(numbers));
}


TEST(algorithmTest, makeUnique)
{
    std::vector<int> numbers = { 1, 2, 1, 4, 1, 5, 9, 4};

    makeUnique(numbers);

    EXPECT_THAT(numbers,
                ElementsAre(1, 2, 4, 5, 9));
}


TEST(algorithmTest, vectorContains)
{
    std::vector<int> numbers = { 1, 9, 1, 2, 9};

    EXPECT_FALSE(contains(numbers, 0));
    EXPECT_TRUE(contains(numbers, 1));
    EXPECT_TRUE(contains(numbers, 2));
    EXPECT_FALSE(contains(numbers, 3));
}


TEST(algorithmTest, mapContains)
{
    std::map<int, int> x;
    x[0] = 99;
    x[4] = 99;
    x[10] = 99;

    EXPECT_TRUE(contains(x, 0));
    EXPECT_FALSE(contains(x, 1));
    EXPECT_FALSE(contains(x, 2));
    EXPECT_FALSE(contains(x, 3));
    EXPECT_TRUE(contains(x, 4));
    EXPECT_FALSE(contains(x, 5));
    EXPECT_FALSE(contains(x, 6));
    EXPECT_FALSE(contains(x, 7));
    EXPECT_FALSE(contains(x, 8));
    EXPECT_FALSE(contains(x, 9));
    EXPECT_TRUE(contains(x, 10));
    EXPECT_FALSE(contains(x, 11));
    EXPECT_FALSE(contains(x, 99));
}


TEST(algorithmTest, removeIf)
{
    std::vector<int> numbers = { 2, 3, 4, 7, 87, 42, 2, 13, 8 };

    removeIf(numbers, ([] (int v) { return v % 2 == 1; }));

    EXPECT_THAT(numbers,
                ElementsAre( 2, 4, 42, 2, 8));
}


} // namespace algorithm
} // namespace base

