#include "System.h"

#if defined(BUILD_WINDOWS)
    #include "WinSystem.h"
#elif defined(BUILD_LINUX)
    #include "NixSystem.h"
#endif

namespace Util {

//=============================================================================
bool System::MakeDirectory(const std::string &path)
{
    return SystemImpl::MakeDirectory(path);
}

//=============================================================================
char System::GetSeparator()
{
    return SystemImpl::GetSeparator();
}

//=============================================================================
std::string System::GetTempDirectory()
{
    return SystemImpl::GetTempDirectory();
}

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
std::string System::GetLastError(int *code)
{
    return SystemImpl::GetLastError(code);
}

//=============================================================================
void System::SetupSignalHandler()
{
    return SystemImpl::SetupSignalHandler();
}

//=============================================================================
void System::CleanExit(int exitCode)
{
    return SystemImpl::CleanExit(exitCode);
}

//=============================================================================
bool System::KeepRunning()
{
    return SystemImpl::KeepRunning();
}

//=============================================================================
int System::ExitCode()
{
    return SystemImpl::ExitCode();
}

}
