#include <fstream>

#include <gtest/gtest.h>

#include <Util/File/IniParser.h>
#include <Util/String/String.h>
#include <Util/System/System.h>

//==============================================================================
class IniParserTest : public ::testing::Test
{
public:
    IniParserTest() :
        m_spParser(),
        m_path(Util::System::GetTempDirectory()),
        m_file(Util::String::GenerateRandomString(10) + ".txt")
    {
    }

    /**
     * Create and start the file monitor.
     */
    virtual void SetUp()
    {
        ASSERT_TRUE(Util::System::MakeDirectory(m_path));
        m_spParser = std::make_shared<Util::IniParser>(m_path, m_file);
    }

    /**
     * Stop the file monitor and delete the created file.
     */
    virtual void TearDown()
    {
        std::remove(GetFullPath().c_str());
    }

protected:
    void CreateFile(const std::string &contents)
    {
        std::ofstream stream(GetFullPath().c_str(), std::ios::out);
        stream << contents;
    }

    /**
     * @return The full path to the file being monitored.
     */
    std::string GetFullPath() const
    {
        static const char sep = Util::System::GetSeparator();
        return Util::String::Join(sep, m_path, m_file);
    }

    Util::IniParserPtr m_spParser;

    std::string m_path;
    std::string m_file;
};

//==============================================================================
TEST_F(IniParserTest, EmptyFileTest)
{
    const std::string contents;
    CreateFile(contents);

    ASSERT_NO_THROW(m_spParser->Parse());
    EXPECT_EQ(m_spParser->GetSize(), 0);
}

//==============================================================================
TEST_F(IniParserTest, EmptySectionTest)
{
    const std::string contents("[section]");
    CreateFile(contents);

    ASSERT_NO_THROW(m_spParser->Parse());
    EXPECT_EQ(m_spParser->GetSize(), 0);
}

//==============================================================================
TEST_F(IniParserTest, NonEmptySectionTest)
{
    const std::string contents(
        "[section]\n"
        "name=John Doe\n"
        "address=USA"
    );

    CreateFile(contents);

    ASSERT_NO_THROW(m_spParser->Parse());

    EXPECT_EQ(m_spParser->GetSize(), 1);
    EXPECT_EQ(m_spParser->GetSize("section"), 2);

    EXPECT_EQ(m_spParser->GetValue<std::string>("section", "name", ""), "John Doe");
    EXPECT_EQ(m_spParser->GetValue<std::string>("section", "address", ""), "USA");
}

//==============================================================================
TEST_F(IniParserTest, NonExistingTest)
{
    const std::string contents(
        "[section]\n"
        "name=John Doe\n"
        "address=USA"
    );

    CreateFile(contents);

    ASSERT_NO_THROW(m_spParser->Parse());

    EXPECT_EQ(m_spParser->GetSize("bad-section"), -1);
    EXPECT_EQ(m_spParser->GetSize("section-bad"), -1);

    EXPECT_EQ(m_spParser->GetValue<std::string>("section", "bad-name", "def"), "def");
    EXPECT_EQ(m_spParser->GetValue<std::string>("bad-section", "address", "def"), "def");
}

//==============================================================================
TEST_F(IniParserTest, NonCovertibleTest)
{
    const std::string contents(
        "[section]\n"
        "name=John Doe\n"
        "address=USA"
    );

    CreateFile(contents);

    ASSERT_NO_THROW(m_spParser->Parse());

    EXPECT_EQ(m_spParser->GetValue<int>("section", "name", 12), 12);
    EXPECT_EQ(m_spParser->GetValue<bool>("section", "address", false), false);
}

//==============================================================================
TEST_F(IniParserTest, CommentTest)
{
    const std::string contents(
        "[section]\n"
        "name=John Doe\n"
        "; [other-section]\n"
        "; name=Jane Doe\n"
    );

    CreateFile(contents);

    ASSERT_NO_THROW(m_spParser->Parse());

    EXPECT_EQ(m_spParser->GetSize(), 1);
    EXPECT_EQ(m_spParser->GetSize("other-section"), -1);
}

//==============================================================================
TEST_F(IniParserTest, ErrantSpacesTest)
{
    const std::string contents(
        "   [section   ]  \n"
        "\t\t\n   name=John Doe\t  \n"
        "\taddress  = USA\t \r \n"
    );

    CreateFile(contents);

    ASSERT_NO_THROW(m_spParser->Parse());

    EXPECT_EQ(m_spParser->GetSize(), 1);
    EXPECT_EQ(m_spParser->GetSize("section"), 2);

    EXPECT_EQ(m_spParser->GetValue<std::string>("section", "name", ""), "John Doe");
    EXPECT_EQ(m_spParser->GetValue<std::string>("section", "address", ""), "USA");
}

//==============================================================================
TEST_F(IniParserTest, QuotedValueTest)
{
    const std::string contents(
        "[section]\n"
        "name=\"  John Doe  \"\n"
        "address= \t '\tUSA'"
    );

    CreateFile(contents);

    ASSERT_NO_THROW(m_spParser->Parse());

    EXPECT_EQ(m_spParser->GetSize(), 1);
    EXPECT_EQ(m_spParser->GetSize("section"), 2);

    EXPECT_EQ(m_spParser->GetValue<std::string>("section", "name", ""), "  John Doe  ");
    EXPECT_EQ(m_spParser->GetValue<std::string>("section", "address", ""), "\tUSA");
}

//==============================================================================
TEST_F(IniParserTest, MutlipleValueTypeTest)
{
    const std::string contents(
        "[section1]\n"
        "name=John Doe\n"
        "age=26\n"
        "[section2]\n"
        "name=Jane Doe\n"
        "age=30.12\n"
        "[section3]\n"
        "name=Joe Doe\n"
        "noage=1\n"
    );

    CreateFile(contents);

    ASSERT_NO_THROW(m_spParser->Parse());

    EXPECT_EQ(m_spParser->GetSize(), 3);
    EXPECT_EQ(m_spParser->GetSize("section1"), 2);
    EXPECT_EQ(m_spParser->GetSize("section2"), 2);
    EXPECT_EQ(m_spParser->GetSize("section3"), 2);

    EXPECT_EQ(m_spParser->GetValue<std::string>("section1", "name", "noname"), "John Doe");
    EXPECT_EQ(m_spParser->GetValue<std::string>("section1", "age", "0"), "26");
    EXPECT_EQ(m_spParser->GetValue<int>("section1", "age", 0), 26);
    EXPECT_EQ(m_spParser->GetValue<unsigned int>("section1", "age", 0), 26);

    EXPECT_EQ(m_spParser->GetValue<std::string>("section2", "name", "noname"), "Jane Doe");
    EXPECT_EQ(m_spParser->GetValue<int>("section2", "age", 0), 30);
    EXPECT_EQ(m_spParser->GetValue<float>("section2", "age", 0.0f), 30.12f);
    EXPECT_EQ(m_spParser->GetValue<double>("section2", "age", 0.0), 30.12);

    EXPECT_EQ(m_spParser->GetValue<std::string>("section3", "name", "noname"), "Joe Doe");
    EXPECT_EQ(m_spParser->GetValue<bool>("section3", "noage", false), true);
    EXPECT_EQ(m_spParser->GetValue<int>("section3", "noage", 0), 1);
}

//==============================================================================
TEST_F(IniParserTest, DuplicateSectionTest)
{
    const std::string contents1(
        "[section]\n"
        "name=John Doe\n"
        "[section]\n"
        "name=Jane Doe\n"
    );

    CreateFile(contents1);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    const std::string contents2(
        "[  \tsection]\n"
        "name=John Doe\n"
        "[section  ]\n"
        "name=Jane Doe\n"
    );

    CreateFile(contents2);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);
}

//==============================================================================
TEST_F(IniParserTest, DuplicateValueTest)
{
    const std::string contents(
        "[section]\n"
        "name=John Doe\n"
        "name=Jane Doe\n"
    );

    CreateFile(contents);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);
}

//==============================================================================
TEST_F(IniParserTest, ImbalancedBraceTest)
{
    const std::string contents1(
        "[section\n"
        "name=John Doe\n"
    );

    const std::string contents2(
        "section]\n"
        "name=John Doe\n"
    );

    CreateFile(contents1);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents2);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);
}

//==============================================================================
TEST_F(IniParserTest, ImbalancedQuoteTest)
{
    const std::string contents1(
        "[section]\n"
        "name=\"John Doe\n"
    );

    const std::string contents2(
        "[section]\n"
        "name=John Doe\"\n"
    );
    const std::string contents3(
        "[section]\n"
        "name='John Doe\n"
    );

    const std::string contents4(
        "[section]\n"
        "name=John Doe'\n"
    );

    const std::string contents5(
        "[section]\n"
        "name=\"John Doe'\n"
    );

    const std::string contents6(
        "[section]\n"
        "name='John Doe\"\n"
    );

    CreateFile(contents1);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents2);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents3);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents4);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents5);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents6);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);
}

//==============================================================================
TEST_F(IniParserTest, MisplacedQuoteTest)
{
    const std::string contents1(
        "[section]\n"
        "\"name\"=John Doe\n"
    );

    const std::string contents2(
        "[section]\n"
        "\'name\'=John Doe\n"
    );

    const std::string contents3(
        "[\"section\"]\n"
        "name=John Doe\n"
    );

    const std::string contents4(
        "[\'section\']\n"
        "name=John Doe\n"
    );

    const std::string contents5(
        "\"[section]\"\n"
        "name=John Doe\n"
    );

    const std::string contents6(
        "\'[section]\'\n"
        "name=John Doe\n"
    );

    CreateFile(contents1);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents2);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents3);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents4);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents5);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents6);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);
}

//==============================================================================
TEST_F(IniParserTest, MultipleAssignmentTest)
{
    const std::string contents1(
        "[section]\n"
        "name=John=Doe\n"
    );
    const std::string contents2(
        "[section]\n"
        "name=\"John=Doe\"\n"
    );

    CreateFile(contents1);
    ASSERT_NO_THROW(m_spParser->Parse());
    EXPECT_EQ(m_spParser->GetValue<std::string>("section", "name", ""), "John=Doe");

    CreateFile(contents2);
    ASSERT_NO_THROW(m_spParser->Parse());
    EXPECT_EQ(m_spParser->GetValue<std::string>("section", "name", ""), "John=Doe");
}

//==============================================================================
TEST_F(IniParserTest, MissingAssignmentTest)
{
    const std::string contents1(
        "[section]\n"
        "name\n"
    );

    const std::string contents2(
        "[section]\n"
        "name=\n"
    );

    CreateFile(contents1);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents2);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);
}

//==============================================================================
TEST_F(IniParserTest, EarlyAssignmentTest)
{
    const std::string contents1(
        "name=John Doe\n"
        "[section]\n"
    );

    const std::string contents2(
        "name=\n"
        "[section]\n"
    );

    const std::string contents3(
        "name\n"
        "[section]\n"
    );

    CreateFile(contents1);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents2);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);

    CreateFile(contents3);
    EXPECT_THROW(m_spParser->Parse(), Util::ParserException);
}

//==============================================================================
TEST_F(IniParserTest, MultipleParseTest)
{
    const std::string contents(
        "[section]\n"
        "name=John Doe\n"
        "address=USA"
    );

    CreateFile(contents);

    for (int i = 0; i < 5; ++i)
    {
        ASSERT_NO_THROW(m_spParser->Parse());

        EXPECT_EQ(m_spParser->GetSize(), 1);
        EXPECT_EQ(m_spParser->GetSize("section"), 2);

        EXPECT_EQ(m_spParser->GetValue<std::string>("section", "name", ""), "John Doe");
        EXPECT_EQ(m_spParser->GetValue<std::string>("section", "address", ""), "USA");
    }
}
