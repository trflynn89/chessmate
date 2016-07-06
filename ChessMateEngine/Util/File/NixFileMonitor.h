#pragma once

#include <chrono>
#include <poll.h>
#include <string>

#include <Util/File/FileMonitor.h>

namespace Util {

/**
 * Linux implementation of the FileMonitor interface.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 4, 2016
 */
class FileMonitorImpl : public FileMonitor
{
public:
    FileMonitorImpl(FileEventCallback, const std::string &, const std::string &);
    virtual ~FileMonitorImpl();

protected:
    bool IsValid() const;
    void Poll(const std::chrono::milliseconds &);

private:
    bool handleEvents();
    FileMonitor::FileEvent convertToEvent(int);
    void close();

    int m_monitorDescriptor;
    int m_watchDescriptor;
};

}
