#include <cstdio>
#include <fstream>
#include <functional>

#include <gtest/gtest.h>

#include <Util/File/FileMonitorImpl.h>
#include <Util/String/String.h>
#include <Util/System/System.h>

//=============================================================================
class FileMonitorTest : public ::testing::Test
{
public:
    FileMonitorTest() :
        m_spMonitor(),
        m_path(Util::System::GetTempDirectory()),
        m_file(Util::String::GenerateRandomString(10) + ".txt"),
        m_numCreatedFiles(0),
        m_numDeletedFiles(0),
        m_numChangedFiles(0),
        m_numOtherEvents(0)
    {
    }

    /**
     * Create and start the file monitor.
     */
    virtual void SetUp()
    {
        ASSERT_TRUE(Util::System::MakeDirectory(m_path));

        auto callback = std::bind(&FileMonitorTest::handleEvent, this, std::placeholders::_1);
        m_spMonitor = std::make_shared<Util::FileMonitorImpl>(callback, m_path, m_file);

        ASSERT_TRUE(m_spMonitor && m_spMonitor->StartMonitor());
    }

    /**
     * Stop the file monitor and delete the created file.
     */
    virtual void TearDown()
    {
        m_spMonitor->StopMonitor();
        std::remove(GetFullPath().c_str());

        ASSERT_TRUE(m_spMonitor.unique());
    }

protected:
    /**
     * Handle a file change notification.
     *
     * @param FileEvent The type of event that occurred.
     */
    void handleEvent(Util::FileMonitor::FileEvent eventType)
    {
        switch (eventType)
        {
        case Util::FileMonitor::FILE_CREATED:
            ++m_numCreatedFiles;
            break;

        case Util::FileMonitor::FILE_DELETED:
            ++m_numDeletedFiles;
            break;

        case Util::FileMonitor::FILE_CHANGED:
            ++m_numChangedFiles;
            break;

        default:
            ++m_numOtherEvents;
            break;
        }
    }

    /**
     * @return The full path to the file being monitored.
     */
    std::string GetFullPath() const
    {
        static const char sep = Util::System::GetSeparator();
        return Util::String::Join(sep, m_path, m_file);
    }

    Util::FileMonitorPtr m_spMonitor;
    std::ofstream m_stream;

    std::string m_path;
    std::string m_file;

    unsigned int m_numCreatedFiles;
    unsigned int m_numDeletedFiles;
    unsigned int m_numChangedFiles;
    unsigned int m_numOtherEvents;
};

//=============================================================================
TEST_F(FileMonitorTest, NoChangeTest)
{
    EXPECT_EQ(m_numCreatedFiles, 0);
    EXPECT_EQ(m_numDeletedFiles, 0);
    EXPECT_EQ(m_numChangedFiles, 0);
    EXPECT_EQ(m_numOtherEvents, 0);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(m_numCreatedFiles, 0);
    EXPECT_EQ(m_numDeletedFiles, 0);
    EXPECT_EQ(m_numChangedFiles, 0);
    EXPECT_EQ(m_numOtherEvents, 0);
}

//=============================================================================
TEST_F(FileMonitorTest, CreateTest)
{
    EXPECT_EQ(m_numCreatedFiles, 0);
    EXPECT_EQ(m_numDeletedFiles, 0);
    EXPECT_EQ(m_numChangedFiles, 0);
    EXPECT_EQ(m_numOtherEvents, 0);

    m_stream.open(GetFullPath(), std::ios::out);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(m_numCreatedFiles, 1);
    EXPECT_EQ(m_numDeletedFiles, 0);
    EXPECT_EQ(m_numChangedFiles, 0);
    EXPECT_EQ(m_numOtherEvents, 0);
}

//=============================================================================
TEST_F(FileMonitorTest, DeleteTest)
{
    EXPECT_EQ(m_numCreatedFiles, 0);
    EXPECT_EQ(m_numDeletedFiles, 0);
    EXPECT_EQ(m_numChangedFiles, 0);
    EXPECT_EQ(m_numOtherEvents, 0);

    m_stream.open(GetFullPath(), std::ios::out);
    std::remove(GetFullPath().c_str());

    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(m_numCreatedFiles, 1);
    EXPECT_EQ(m_numDeletedFiles, 1);
    EXPECT_EQ(m_numChangedFiles, 0);
    EXPECT_EQ(m_numOtherEvents, 0);
}

//=============================================================================
TEST_F(FileMonitorTest, ChangeTest)
{
    EXPECT_EQ(m_numCreatedFiles, 0);
    EXPECT_EQ(m_numDeletedFiles, 0);
    EXPECT_EQ(m_numChangedFiles, 0);
    EXPECT_EQ(m_numOtherEvents, 0);

    m_stream.open(GetFullPath(), std::ios::out);
    m_stream << "abcdefghi";
    m_stream.close();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(m_numCreatedFiles, 1);
    EXPECT_EQ(m_numDeletedFiles, 0);
    EXPECT_EQ(m_numChangedFiles, 1);
    EXPECT_EQ(m_numOtherEvents, 0);
}

//=============================================================================
TEST_F(FileMonitorTest, OtherFileTest)
{
    EXPECT_EQ(m_numCreatedFiles, 0);
    EXPECT_EQ(m_numDeletedFiles, 0);
    EXPECT_EQ(m_numChangedFiles, 0);
    EXPECT_EQ(m_numOtherEvents, 0);

    std::string file = GetFullPath() + ".diff";
    m_stream.open(file, std::ios::out);
    m_stream << "abcdefghi";
    m_stream.close();
    std::remove(file.c_str());

    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(m_numCreatedFiles, 0);
    EXPECT_EQ(m_numDeletedFiles, 0);
    EXPECT_EQ(m_numChangedFiles, 0);
    EXPECT_EQ(m_numOtherEvents, 0);

    file = file.substr(0, file.length() - 6);
    m_stream.open(file, std::ios::out);
    m_stream << "abcdefghi";
    m_stream.close();
    std::remove(file.c_str());

    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(m_numCreatedFiles, 0);
    EXPECT_EQ(m_numDeletedFiles, 0);
    EXPECT_EQ(m_numChangedFiles, 0);
    EXPECT_EQ(m_numOtherEvents, 0);
}
