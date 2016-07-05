#include "NixFileMonitor.h"

#include <cstdlib>
#include <cstring>
#include <sys/inotify.h>
#include <unistd.h>

#include <Util/Logging/Logger.h>
#include <Util/System/System.h>

namespace Util {

//=============================================================================
FileMonitorImpl::FileMonitorImpl(
    FileEventCallback handler,
    const std::string &path,
    const std::string &file
) :
    FileMonitor(handler, path, file),
    m_monitorDescriptor(-1),
    m_watchDescriptor(-1)
{
    m_monitorDescriptor = ::inotify_init1(IN_NONBLOCK);

    if (m_monitorDescriptor == -1)
    {
        LOGW(-1, "Could not initialize monitor for \"%s\": %s",
            m_path, Util::System::GetLastError()
        );
    }
    else
    {
        int flags = IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_CLOSE_WRITE;

        m_watchDescriptor = ::inotify_add_watch(
            m_monitorDescriptor, m_path.c_str(), flags
        );

        if (m_watchDescriptor == -1)
        {
            LOGW(-1, "Could not add watcher for \"%s\": %s",
                m_path, Util::System::GetLastError()
            );

            close();
        }
    }
}

//=============================================================================
FileMonitorImpl::~FileMonitorImpl()
{
    close();
}

//=============================================================================
bool FileMonitorImpl::IsValid() const
{
    return (m_monitorDescriptor != -1);
}

//=============================================================================
void FileMonitorImpl::Poll(const std::chrono::milliseconds &timeout)
{
    struct pollfd pollFd;

    pollFd.fd = m_monitorDescriptor;
    pollFd.events = POLLIN;

    int numEvents = ::poll(&pollFd, 1, timeout.count());

    if (numEvents == -1)
    {
        int error = 0;

        LOGW(-1, "Could not create poller for \"%s\": %s",
            m_path, Util::System::GetLastError(&error)
        );

        close();
    }
    else if ((numEvents > 0) && (pollFd.revents & POLLIN))
    {
        while (handleEvents())
        {
        }
    }
}

//=============================================================================
bool FileMonitorImpl::handleEvents()
{
    const struct inotify_event *event;
    size_t eventSize = sizeof(struct inotify_event);

    char buff[4096];
    char *ptr;

    ssize_t len = ::read(m_monitorDescriptor, buff, sizeof(buff));

    if (len <= 0)
    {
        if (len == -1)
        {
            int error = 0;
            std::string errorStr = Util::System::GetLastError(&error);

            if (error != EAGAIN)
            {
                LOGW(-1, "Could not read polled event for \"%s\": %s", m_path, error);
                close();
            }
        }

        return false;
    }

    for (ptr = buff; ptr < buff + len; ptr += eventSize + event->len)
    {
        event = (const struct inotify_event *)ptr;

        if ((event->len > 0) && (std::string(event->name).compare(m_file) == 0))
        {
            FileMonitor::FileEvent type = FileMonitor::FILE_NO_CHANGE;

            if (event->mask & IN_CREATE)
            {
                type = FileMonitor::FILE_CREATED;
            }
            else if ((event->mask & IN_DELETE) || (event->mask & IN_DELETE_SELF))
            {
                type = FileMonitor::FILE_DELETED;
            }
            else if (event->mask & IN_CLOSE_WRITE)
            {
                type = FileMonitor::FILE_CHANGED;
            }

            if (type != FileMonitor::FILE_NO_CHANGE)
            {
                LOGI(-1, "Handling event %d for \"%s\"", type, m_file);

                std::lock_guard<std::mutex> lock(m_callbackMutex);

                if (m_handler != nullptr)
                {
                    m_handler(type);
                }
            }
        }
    }

    return true;
}

//=============================================================================
void FileMonitorImpl::close()
{
    if (m_monitorDescriptor != -1)
    {
        if (m_watchDescriptor != -1)
        {
            ::inotify_rm_watch(m_monitorDescriptor, m_watchDescriptor);
            m_watchDescriptor = -1;
        }

        ::close(m_monitorDescriptor);
        m_monitorDescriptor = -1;
    }
}

}
