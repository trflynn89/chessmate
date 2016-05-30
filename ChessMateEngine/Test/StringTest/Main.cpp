#include <regex>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <Util/Logging/Logger.h>
#include <Util/String/String.h>

namespace
{
    //=========================================================================
    class Base
    {
    public:
        Base(const std::string &str, int num) :
            m_str(str),
            m_num(num)
        {
        }

        std::string GetStr() const { return m_str; };
        int GetNum() const { return m_num; };

        size_t Hash() const
        {
            static std::hash<std::string> strHasher;
            static std::hash<int> numHasher;
            static int magic = 0x9e3779b9;

            size_t strHash = strHasher(m_str);
            size_t numHash = numHasher(m_num);

            // Derived from boost::hash_combine
            return (strHash ^ (numHash + magic + (strHash << 6) + (strHash >> 2)));
        }

    private:
        std::string m_str;
        int m_num;
    };

    //=========================================================================
    class Hashable : public Base
    {
    public:
        Hashable(const std::string &str, int num) : Base(str, num)
        {
        }
    };

    //=========================================================================
    class Streamable : public Base
    {
    public:
        Streamable(const std::string &str, int num) : Base(str, num)
        {
        }

        friend std::ostream &operator << (std::ostream &, const Streamable &);
    };

    std::ostream &operator << (std::ostream &stream, const Streamable &obj)
    {
        stream << '[' << obj.GetStr() << ' ' << std::hex << obj.GetNum() << std::dec << ']';
        return stream;
    }

    //=========================================================================
    class HashableAndStreamable : public Base
    {
    public:
        HashableAndStreamable(const std::string &str, int num) : Base(str, num)
        {
        }

        friend std::ostream &operator << (std::ostream &, const HashableAndStreamable &);
    };

    std::ostream &operator << (std::ostream &stream, const HashableAndStreamable &obj)
    {
        stream << '[' << obj.GetStr() << ' ' << std::hex << obj.GetNum() << std::dec << ']';
        return stream;
    }
}

//=============================================================================
namespace std
{
    template <>
    struct hash<Hashable>
    {
        size_t operator()(const Hashable &value) const
        {
            return value.Hash();
        }
    };

    template <>
    struct hash<HashableAndStreamable>
    {
        size_t operator()(const HashableAndStreamable &value) const
        {
            return value.Hash();
        }
    };
}

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

//=============================================================================
TEST(StringTest, JoinTest)
{
    Hashable obj1("hello", 0xdead);
    Streamable obj2("goodbye", 0xbeef);
    HashableAndStreamable obj3("world", 0xf00d);

    std::string str("a");
    const char *ctr = "b";
    char arr[] = { 'c', '\0' };
    char chr = 'd';

    ASSERT_EQ("a", Util::String::Join('.', str));
    ASSERT_EQ("b", Util::String::Join('.', ctr));
    ASSERT_EQ("c", Util::String::Join('.', arr));
    ASSERT_EQ("d", Util::String::Join('.', chr));

    ASSERT_EQ("a,a", Util::String::Join(',', str, str));
    ASSERT_EQ("a,b", Util::String::Join(',', str, ctr));
    ASSERT_EQ("a,c", Util::String::Join(',', str, arr));
    ASSERT_EQ("a,d", Util::String::Join(',', str, chr));
    ASSERT_EQ("b,a", Util::String::Join(',', ctr, str));
    ASSERT_EQ("b,b", Util::String::Join(',', ctr, ctr));
    ASSERT_EQ("b,c", Util::String::Join(',', ctr, arr));
    ASSERT_EQ("b,d", Util::String::Join(',', ctr, chr));
    ASSERT_EQ("c,a", Util::String::Join(',', arr, str));
    ASSERT_EQ("c,b", Util::String::Join(',', arr, ctr));
    ASSERT_EQ("c,c", Util::String::Join(',', arr, arr));
    ASSERT_EQ("c,d", Util::String::Join(',', arr, chr));
    ASSERT_EQ("d,a", Util::String::Join(',', chr, str));
    ASSERT_EQ("d,b", Util::String::Join(',', chr, ctr));
    ASSERT_EQ("d,c", Util::String::Join(',', chr, arr));
    ASSERT_EQ("d,d", Util::String::Join(',', chr, chr));

    ASSERT_EQ("[goodbye beef]", Util::String::Join('.', obj2));
    ASSERT_EQ("a:[goodbye beef]:c:d", Util::String::Join(':', str, obj2, arr, chr));
    ASSERT_EQ("a:c:d", Util::String::Join(':', str, arr, chr));

#ifndef BUILD_WINDOWS

    std::regex test("(\\[0x[0-9a-f]+\\]:2:\\[goodbye beef\\]:\\[world f00d\\])");
    ASSERT_TRUE(std::regex_match(Util::String::Join(':', obj1, 2, obj2, obj3), test));

#endif // BUILD_WINDOWS
}
