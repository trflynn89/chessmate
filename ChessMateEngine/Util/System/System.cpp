#include "System.h"

#if defined(BUILD_WINDOWS)
    #include "WinSystem.h"
#elif defined(BUILD_LINUX)
    #include "NixSystem.h"
#endif

namespace Util {

//=============================================================================
void System::PrintBacktrace()
{
    SystemImpl::PrintBacktrace();
}

//=============================================================================
std::string System::LocalTime()
{
    return SystemImpl::LocalTime("%m-%d-%Y %H:%M:%S");
}

//=============================================================================
std::string System::GetLastError()
{
    return SystemImpl::GetLastError();
}

}
