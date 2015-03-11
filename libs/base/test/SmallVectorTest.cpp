
#include "SmallVector.h"
#include <gtest/gtest.h>


namespace base{


TEST(IntervalTest, simple)
{
    SmallVector<int, 2> v;

    EXPECT_TRUE(v.empty());

    v.push_back(1);
    v.push_back(5);
    v.push_back(7);

    ASSERT_EQ(3, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(5, v[1]);
    EXPECT_EQ(7, v[2]);
}


TEST(IntervalTest, init)
{
    SmallVector<int, 5> v = { 4, 8, 9, 1};

    EXPECT_FALSE(v.empty());

    ASSERT_EQ(4, v.size());
    EXPECT_EQ(4, v[0]);
    EXPECT_EQ(8, v[1]);
    EXPECT_EQ(9, v[2]);
    EXPECT_EQ(1, v[3]);
}


} // namespace base

