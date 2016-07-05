#pragma once

#include <chrono>
#include <string>

#include <Util/File/FileMonitor.h>

namespace Util {

/**
 * Windows implementation of the FileMonitor interface.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 5, 2016
 */
class FileMonitorImpl : public FileMonitor
{
public:
    FileMonitorImpl(FileEventCallback, const std::string &, const std::string &);
    virtual ~FileMonitorImpl();

protected:
    bool IsValid() const;
    void Poll(const std::chrono::milliseconds &);
};

}
