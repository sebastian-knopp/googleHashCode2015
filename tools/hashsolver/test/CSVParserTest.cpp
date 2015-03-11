#include "CSVParser.cpp"
#include <gtest/gtest.h>


TEST(CSVParserTest, simpleWithHeader)
{
    std::stringstream s;
    s
    << "one;two;three" << std::endl
    << "1;2;3" << std::endl
    << "101;102;103" << std::endl
    << "a;b;c" << std::endl;

    CSVParser parser(s, ';');

    parser.readHeader();
    EXPECT_EQ(3, parser.getNmbColumns());

    parser.getLine();
    EXPECT_EQ("1", parser.get("one"));
    EXPECT_EQ("2", parser.get("two"));
    EXPECT_EQ("3", parser.get("three"));
    EXPECT_EQ(3, parser.getNmbColumns());
    EXPECT_TRUE(parser);

    parser.getLine();
    EXPECT_EQ("101", parser.get("one"));
    EXPECT_EQ("102", parser.get("two"));
    EXPECT_EQ("103", parser.get("three"));
    EXPECT_EQ(3, parser.getNmbColumns());
    EXPECT_TRUE(parser);

    parser.getLine();
    EXPECT_EQ("a", parser.get("one"));
    EXPECT_EQ("b", parser.get("two"));
    EXPECT_EQ("c", parser.get("three"));
    EXPECT_EQ(3, parser.getNmbColumns());
    EXPECT_FALSE(parser);
}


TEST(CSVParserTest, simpleWithoutHeader)
{
    std::stringstream s;
    s
    << "1;2;3" << std::endl
    << "101;102;103" << std::endl
    << "a;b;c" << std::endl;

    CSVParser parser(s, ';');

    parser.getLine();
    EXPECT_EQ("1", parser.get(0));
    EXPECT_EQ("2", parser.get(1));
    EXPECT_EQ("3", parser.get(2));
    EXPECT_EQ(3, parser.getNmbColumns());
    EXPECT_TRUE(parser);

    parser.getLine();
    EXPECT_EQ("101", parser.get(0));
    EXPECT_EQ("102", parser.get(1));
    EXPECT_EQ("103", parser.get(2));
    EXPECT_EQ(3, parser.getNmbColumns());
    EXPECT_TRUE(parser);

    parser.getLine();
    EXPECT_EQ("a", parser.get(0));
    EXPECT_EQ("b", parser.get(1));
    EXPECT_EQ("c", parser.get(2));
    EXPECT_EQ(3, parser.getNmbColumns());
    EXPECT_FALSE(parser);
}


TEST(CSVParserTest, invalidKey)
{
    std::stringstream s;
    s
    << "one;two;three" << std::endl
    << "a;b;c" << std::endl;

    CSVParser parser(s, ';');

    parser.readHeader();
    parser.getLine();

    EXPECT_THROW(parser.get("invalidKey"), CSVParser::Exception);
}


TEST(CSVParserTest, invalidIndex)
{
    std::stringstream s;
    s
    << "one;two;three" << std::endl;

    CSVParser parser(s, ';');
    parser.getLine();

    EXPECT_THROW(parser.get(4), CSVParser::Exception);
}


TEST(CSVParserTest, missingColumnsWithHeader)
{
    std::stringstream s;
    s
    << "one;two;three;four" << std::endl
    << "1;;" << std::endl
    << ";2" << std::endl
    << ";;" << std::endl
    << ";;3" << std::endl;

    CSVParser parser(s, ';');

    parser.readHeader();
    EXPECT_EQ(4, parser.getNmbColumns());

    parser.getLine();
    EXPECT_EQ("1", parser.get("one"));
    EXPECT_EQ("", parser.get("two"));
    EXPECT_EQ("", parser.get("three"));
    EXPECT_EQ("", parser.get("four"));
    EXPECT_EQ(2, parser.getNmbColumns());

    parser.getLine();
    EXPECT_EQ("", parser.get("one"));
    EXPECT_EQ("2", parser.get("two"));
    EXPECT_EQ("", parser.get("three"));
    EXPECT_EQ("", parser.get("four"));
    EXPECT_EQ(2, parser.getNmbColumns());

    parser.getLine();
    EXPECT_EQ("", parser.get("one"));
    EXPECT_EQ("", parser.get("two"));
    EXPECT_EQ("", parser.get("three"));
    EXPECT_EQ("", parser.get("four"));
    EXPECT_EQ(2, parser.getNmbColumns());

    parser.getLine();
    EXPECT_EQ("", parser.get("one"));
    EXPECT_EQ("", parser.get("two"));
    EXPECT_EQ("3", parser.get("three"));
    EXPECT_EQ("", parser.get("four"));
    EXPECT_EQ(3, parser.getNmbColumns());
}

