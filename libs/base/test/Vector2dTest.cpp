
#include "Vector2d.h"
#include <gtest/gtest.h>


using namespace base;


TEST(Vector2dTest, resize)
{
    Vector2d<int> v(5, 6);

    EXPECT_EQ(size_t(5), v.width());
    EXPECT_EQ(size_t(6), v.height());

    v.resize(7, 8);

    EXPECT_EQ(size_t(7), v.width());
    EXPECT_EQ(size_t(8), v.height());
}


TEST(Vector2dTest, atOutOfRangeConst)
{
    const Vector2d<int> v(5, 6);
    EXPECT_THROW(v.at(10,3), Vector2d<int>::OutOfRangeException);
}


TEST(Vector2dTest, atOutOfRangeMutable)
{
    Vector2d<int> v(5, 6);
    EXPECT_THROW(v.at(10,3), Vector2d<int>::OutOfRangeException);
}

