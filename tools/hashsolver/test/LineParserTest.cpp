#include "LineParser.cpp"
#include <gtest/gtest.h>


TEST(LineParserTest, basic1)
{
    std::stringstream s;
    s
    << "abc" << std::endl
    << "cde" << std::endl
    << "" << std::endl
    << "x" << std::endl;

    LineParser parser(s);

    EXPECT_EQ("abc", parser.getLine());
    ASSERT_TRUE(parser);
    EXPECT_NO_THROW(parser.expectLine("cde"));
    ASSERT_TRUE(parser);
    EXPECT_EQ("", parser.getLine());
    EXPECT_THROW(parser.expectLine("y"), LineParser::Exception);
}


TEST(LineParserTest, basic2)
{
    std::stringstream s;
    s << "abc" << std::endl;

    LineParser parser(s);

    ASSERT_TRUE(parser);
    EXPECT_EQ("abc", parser.getLine());
    ASSERT_FALSE(parser);
}

