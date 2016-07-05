#include "WinFileMonitor.h"

namespace Util {

//=============================================================================
FileMonitorImpl::FileMonitorImpl(
    FileEventCallback handler,
    const std::string &path,
    const std::string &file
) :
    FileMonitor(handler, path, file)
{
}

//=============================================================================
FileMonitorImpl::~FileMonitorImpl()
{
}

//=============================================================================
bool FileMonitorImpl::IsValid() const
{
    return false;
}

//=============================================================================
void FileMonitorImpl::Poll(const std::chrono::milliseconds &timeout)
{
}

}
