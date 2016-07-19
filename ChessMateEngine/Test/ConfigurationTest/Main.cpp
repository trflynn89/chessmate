#include <fstream>

#include <gtest/gtest.h>

#include <Util/Configuration/Configuration.h>
#include <Util/Configuration/ConfigurationManager.h>
#include <Util/File/Parser.h>
#include <Util/String/String.h>
#include <Util/System/System.h>

//==============================================================================
class ConfigurationTest : public ::testing::Test
{
public:
    ConfigurationTest() :
        m_spConfiguration(std::make_shared<Util::Configuration>())
    {
    }

protected:
    Util::ConfigurationPtr m_spConfiguration;
};

//==============================================================================
TEST_F(ConfigurationTest, NonExistingTest)
{
    EXPECT_EQ(m_spConfiguration->GetValue<std::string>("bad-name", "def"), "def");
}

//==============================================================================
TEST_F(ConfigurationTest, NonCovertibleTest)
{
    const Util::Parser::ValueList values = {
        { "name", "John Doe" },
        { "address", "USA" }
    };

    m_spConfiguration->Update(values);

    EXPECT_EQ(m_spConfiguration->GetValue<int>("name", 12), 12);
    EXPECT_EQ(m_spConfiguration->GetValue<bool>("address", false), false);
}

//==============================================================================
TEST_F(ConfigurationTest, MutlipleValueTypeTest)
{
    const Util::Parser::ValueList values = {
        { "name", "John Doe" },
        { "address", "123" },
        { "employed", "1" },
        { "age", "26.2" }
    };

    m_spConfiguration->Update(values);

    EXPECT_EQ(m_spConfiguration->GetValue<std::string>("name", ""), "John Doe");

    EXPECT_EQ(m_spConfiguration->GetValue<std::string>("address", ""), "123");
    EXPECT_EQ(m_spConfiguration->GetValue<int>("address", 0), 123);
    EXPECT_EQ(m_spConfiguration->GetValue<unsigned int>("address", 0), 123);
    EXPECT_EQ(m_spConfiguration->GetValue<float>("address", 0.0f), 123.0f);
    EXPECT_EQ(m_spConfiguration->GetValue<double>("address", 0.0), 123.0);

    EXPECT_EQ(m_spConfiguration->GetValue<std::string>("age", ""), "26.2");
    EXPECT_EQ(m_spConfiguration->GetValue<int>("age", 0), 26);
    EXPECT_EQ(m_spConfiguration->GetValue<unsigned int>("age", 0), 26);
    EXPECT_EQ(m_spConfiguration->GetValue<float>("age", 0.0f), 26.2f);
    EXPECT_EQ(m_spConfiguration->GetValue<double>("age", 0.0), 26.2);

    EXPECT_EQ(m_spConfiguration->GetValue<std::string>("employed", ""), "1");
    EXPECT_EQ(m_spConfiguration->GetValue<bool>("employed", false), true);
    EXPECT_EQ(m_spConfiguration->GetValue<int>("employed", 0), 1);
}

//==============================================================================
class ConfigurationManagerTest : public ::testing::Test
{
public:
    ConfigurationManagerTest() :
        m_path(Util::System::GetTempDirectory()),
        m_file(Util::String::GenerateRandomString(10) + ".txt"),
        m_spConfigurationManager(std::make_shared<Util::ConfigurationManager>(
            Util::ConfigurationManager::CONFIG_TYPE_INI, m_path, m_file
        ))
    {
    }

    /**
     * Create the file directory.
     */
    virtual void SetUp()
    {
        ASSERT_TRUE(Util::System::MakeDirectory(m_path));
        ASSERT_TRUE(m_spConfigurationManager->StartConfigurationManager());
    }

    /**
     * Delete the created file.
     */
    virtual void TearDown()
    {
        m_spConfigurationManager->StopConfigurationManager();
        std::remove(GetFullPath().c_str());
    }

protected:
    /**
     * Create a file with the given contents.
     *
     * @param string Contents of the file to create.
     */
    void CreateFile(const std::string &contents)
    {
        std::ofstream stream(GetFullPath().c_str(), std::ios::out);
        stream << contents;
    }

    /**
     * @return The full path to the configuration file.
     */
    std::string GetFullPath() const
    {
        static const char sep = Util::System::GetSeparator();
        return Util::String::Join(sep, m_path, m_file);
    }

    std::string m_path;
    std::string m_file;

    Util::ConfigurationManagerPtr m_spConfigurationManager;
};

//==============================================================================
TEST_F(ConfigurationManagerTest, BadFileTypeTest)
{
    m_spConfigurationManager->StopConfigurationManager();

    m_spConfigurationManager = std::make_shared<Util::ConfigurationManager>(
        (Util::ConfigurationManager::ConfigurationFileType)-1, m_path, m_file
    );

    EXPECT_FALSE(m_spConfigurationManager->StartConfigurationManager());
}

//==============================================================================
TEST_F(ConfigurationManagerTest, CreateConfigurationTest)
{
    m_spConfigurationManager->CreateConfiguration<Util::Configuration>();
    EXPECT_EQ(m_spConfigurationManager->GetSize(), 1);
}

//==============================================================================
TEST_F(ConfigurationManagerTest, DuplicateConfigurationTest)
{
    m_spConfigurationManager->CreateConfiguration<Util::Configuration>();
    EXPECT_EQ(m_spConfigurationManager->GetSize(), 1);

    m_spConfigurationManager->CreateConfiguration<Util::Configuration>();
    EXPECT_EQ(m_spConfigurationManager->GetSize(), 1);
}

//==============================================================================
TEST_F(ConfigurationManagerTest, InitialFileFirstTest)
{
    const std::string contents(
        "[" + Util::Configuration::GetName() + "]\n"
        "name=John Doe\n"
        "address=USA"
    );

    CreateFile(contents);
    std::this_thread::sleep_for(std::chrono::seconds(6));

    auto spConfiguration = m_spConfigurationManager->CreateConfiguration<Util::Configuration>();

    EXPECT_EQ(spConfiguration->GetValue<std::string>("name", ""), "John Doe");
    EXPECT_EQ(spConfiguration->GetValue<std::string>("address", ""), "USA");
}

//==============================================================================
TEST_F(ConfigurationManagerTest, InitialFileSecondTest)
{
    auto spConfiguration = m_spConfigurationManager->CreateConfiguration<Util::Configuration>();

    const std::string contents(
        "[" + Util::Configuration::GetName() + "]\n"
        "name=John Doe\n"
        "address=USA"
    );

    CreateFile(contents);
    std::this_thread::sleep_for(std::chrono::seconds(6));

    EXPECT_EQ(spConfiguration->GetValue<std::string>("name", ""), "John Doe");
    EXPECT_EQ(spConfiguration->GetValue<std::string>("address", ""), "USA");
}

//==============================================================================
TEST_F(ConfigurationManagerTest, FileChangeTest)
{
    auto spConfiguration = m_spConfigurationManager->CreateConfiguration<Util::Configuration>();

    const std::string contents1(
        "[" + Util::Configuration::GetName() + "]\n"
        "name=John Doe\n"
        "address=USA"
    );

    CreateFile(contents1);
    std::this_thread::sleep_for(std::chrono::seconds(6));

    EXPECT_EQ(spConfiguration->GetValue<std::string>("name", ""), "John Doe");
    EXPECT_EQ(spConfiguration->GetValue<std::string>("address", ""), "USA");
    EXPECT_EQ(spConfiguration->GetValue<int>("age", -1), -1);

    const std::string contents2(
        "[" + Util::Configuration::GetName() + "]\n"
        "name=Jane Doe\n"
        "age=27"
    );

    CreateFile(contents2);
    std::this_thread::sleep_for(std::chrono::seconds(6));

    EXPECT_EQ(spConfiguration->GetValue<std::string>("name", ""), "Jane Doe");
    EXPECT_EQ(spConfiguration->GetValue<std::string>("address", ""), "");
    EXPECT_EQ(spConfiguration->GetValue<int>("age", -1), 27);
}

//==============================================================================
TEST_F(ConfigurationManagerTest, DeleteFileTest)
{
    auto spConfiguration = m_spConfigurationManager->CreateConfiguration<Util::Configuration>();

    const std::string contents(
        "[" + Util::Configuration::GetName() + "]\n"
        "name=John Doe\n"
        "address=USA"
    );

    CreateFile(contents);
    std::this_thread::sleep_for(std::chrono::seconds(6));

    EXPECT_EQ(spConfiguration->GetValue<std::string>("name", ""), "John Doe");
    EXPECT_EQ(spConfiguration->GetValue<std::string>("address", ""), "USA");

    std::remove(GetFullPath().c_str());
    std::this_thread::sleep_for(std::chrono::seconds(6));

    EXPECT_EQ(spConfiguration->GetValue<std::string>("name", ""), "");
    EXPECT_EQ(spConfiguration->GetValue<std::string>("address", ""), "");
}

//==============================================================================
TEST_F(ConfigurationManagerTest, BadUpdateTest)
{
    auto spConfiguration = m_spConfigurationManager->CreateConfiguration<Util::Configuration>();

    const std::string contents(
        "[" + Util::Configuration::GetName() + "]\n"
        "name"
    );

    CreateFile(contents);
    std::this_thread::sleep_for(std::chrono::seconds(6));

    EXPECT_EQ(spConfiguration->GetValue<std::string>("name", ""), "");
    EXPECT_EQ(spConfiguration->GetValue<std::string>("address", ""), "");
}
