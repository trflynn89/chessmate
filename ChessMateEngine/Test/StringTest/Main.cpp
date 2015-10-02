#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <Util/Logging/Logger.h>
#include <Util/String/String.h>

//=============================================================================
TEST(StringTest, SplitTest)
{
    static const int numSectors = 10;
    std::vector<std::string> inputSplit(numSectors);

    std::string input;
    char delim = ' ';

    for (int i = 0; i < numSectors; ++i)
    {
        std::string curr = Util::String::GenerateRandomString(10);

        inputSplit[i] = curr;
        input += curr + delim;
    }

    std::vector<std::string> outputSplit = Util::String::Split(input, delim);
    ASSERT_EQ(inputSplit.size(), outputSplit.size());

    for (int i = 0; i < numSectors; ++i)
    {
        ASSERT_EQ(inputSplit[i], outputSplit[i]);
    }
}

//=============================================================================
TEST(StringTest, ReplaceAllTest)
{
    std::string source("To Be Replaced! To Be Replaced!");
    std::string search("Be Replaced");
    std::string replace("new value");
    std::string result("To new value! To new value!");

    Util::String::ReplaceAll(source, search, replace);
    ASSERT_EQ(source, result);
}

//=============================================================================
TEST(StringTest, ReplaceAllWithEmptyTest)
{
    std::string source("To Be Replaced! To Be Replaced!");
    std::string search;
    std::string replace("new value");
    std::string result("To Be Replaced! To Be Replaced!");

    Util::String::ReplaceAll(source, search, replace);
    ASSERT_EQ(source, result);
}

//=============================================================================
TEST(StringTest, RemoveAllTest)
{
    std::string source("To Be Replaced! To Be Replaced!");
    std::string search("Be Rep");
    std::string result("To laced! To laced!");

    Util::String::RemoveAll(source, search);
    ASSERT_EQ(source, result);
}

//=============================================================================
TEST(StringTest, RemoveAllWithEmptyTest)
{
    std::string source("To Be Replaced! To Be Replaced!");
    std::string search;
    std::string result("To Be Replaced! To Be Replaced!");

    Util::String::RemoveAll(source, search);
    ASSERT_EQ(source, result);
}

//=============================================================================
TEST(StringTest, GenerateRandomStringTest)
{
    static const int length = (1 << 20);

    std::string random = Util::String::GenerateRandomString(length);
    ASSERT_EQ(length, random.length());
}

//=============================================================================
TEST(StringTest, EntropyTest)
{
    std::string str1 = Util::String::GenerateRandomString(1 << 10);
    std::string str2("A quick brown fox jumped over the lazy dog");
    std::string str3("repeatrepeatrepeatrepeatrepeatrepeatrepeat");
    std::string str4("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");

    float ent1 = Util::String::CalculateEntropy(str1);
    float ent2 = Util::String::CalculateEntropy(str2);
    float ent3 = Util::String::CalculateEntropy(str3);
    float ent4 = Util::String::CalculateEntropy(str4);

    LOGC("E1=%f, E2=%f, E3=%f, E4=%f", ent1, ent2, ent3, ent4);

    // Expect entropy to decrease for less random strings
    ASSERT_GT(ent1, ent2);
    ASSERT_GT(ent2, ent3);
    ASSERT_GT(ent3, ent4);
}

//=============================================================================
TEST(StringTest, FormatTest)
{
    ASSERT_EQ("", Util::String::Format(""));
    ASSERT_EQ("%", Util::String::Format("%"));
    ASSERT_EQ("%%", Util::String::Format("%%"));
    ASSERT_EQ("%d", Util::String::Format("%d"));
    ASSERT_EQ("This is a test", Util::String::Format("This is a test"));
    ASSERT_EQ("there are no formatters", Util::String::Format("there are no formatters", 1, 2, 3, 4));
    ASSERT_EQ("test some string s", Util::String::Format("test %s %c", std::string("some string"), 's'));
    ASSERT_EQ("test 1 true 2.100000 false 1.230000e+02 0xff", Util::String::Format("test %d %d %f %d %e %x", 1, true, 2.1f, false, 123.0, 255));
}
