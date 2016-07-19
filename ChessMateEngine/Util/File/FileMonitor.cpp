#include "FileMonitor.h"

#include <Util/Logging/Logger.h>

namespace Util {

namespace
{
    static const std::chrono::milliseconds s_pollTimeout(1000);
}

//==============================================================================
FileMonitor::FileMonitor(
    FileEventCallback handler,
    const std::string &path,
    const std::string &file
) :
    m_path(path),
    m_file(file),
    m_aKeepRunning(true),
    m_handler(handler)
{
}

//==============================================================================
FileMonitor::~FileMonitor()
{
    if (m_aKeepRunning.load())
    {
        StopMonitor();
    }
}

//==============================================================================
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

//==============================================================================
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

//==============================================================================
void FileMonitor::HandleEvent(FileEvent event)
{
    if (event != FileMonitor::FILE_NO_CHANGE)
    {
        m_eventQueue.Push(event);
    }
}

//==============================================================================
void FileMonitor::monitorThread()
{
    static const size_t updateInterval = 5;
    size_t interval = 0;

    while (m_aKeepRunning.load())
    {
        if (IsValid())
        {
            Poll(s_pollTimeout);

            if ((++interval % updateInterval) == 0)
            {
                FileEvent event = FileMonitor::FILE_NO_CHANGE;

                while (!m_eventQueue.IsEmpty())
                {
                    m_eventQueue.Pop(event);
                }

                if (event != FileMonitor::FILE_NO_CHANGE)
                {
                    std::lock_guard<std::mutex> lock(m_callbackMutex);

                    if (m_handler != nullptr)
                    {
                        LOGI(-1, "Handling event %d for \"%s\"", event, m_file);
                        m_handler(event);
                    }
                }
            }
        }
        else
        {
            m_aKeepRunning.store(false);
        }
    }
}

}
