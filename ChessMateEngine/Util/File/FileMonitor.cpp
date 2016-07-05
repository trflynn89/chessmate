#include "FileMonitor.h"

namespace Util {

namespace
{
    static const std::chrono::milliseconds s_pollTimeout(1000);
}

//=============================================================================
FileMonitor::FileMonitor(
    FileEventCallback handler,
    const std::string &path,
    const std::string &file
) :
    m_handler(handler),
    m_path(path),
    m_file(file),
    m_aKeepRunning(true)
{
}

//=============================================================================
FileMonitor::~FileMonitor()
{
    if (m_aKeepRunning.load())
    {
        StopMonitor();
    }
}

//=============================================================================
bool FileMonitor::StartMonitor()
{
    bool ret = false;

    if (IsValid())
    {
        const FileMonitorPtr &spThis = shared_from_this();
        auto function = &FileMonitor::monitorThread;

        m_future = std::async(std::launch::async, function, spThis);

        ret = m_future.valid();
    }

    return ret;
}

//=============================================================================
void FileMonitor::StopMonitor()
{
    m_aKeepRunning.store(false);

    if (m_future.valid())
    {
        m_future.get();
    }

    std::lock_guard<std::mutex> lock(m_callbackMutex);
    m_handler = nullptr;
}

//=============================================================================
void FileMonitor::monitorThread()
{
    while (m_aKeepRunning.load())
    {
        if (IsValid())
        {
            Poll(s_pollTimeout);
        }
        else
        {
            m_aKeepRunning.store(false);
        }
    }
}

}
