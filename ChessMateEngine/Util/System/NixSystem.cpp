#include "NixSystem.h"

#include <atomic>
#include <chrono>
#include <cstring>
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

#include <Util/Logging/Logger.h>

namespace Util {

namespace
{
    static std::atomic_bool g_aKeepRunning(true);
    static std::atomic_int g_aExitCode(0);

    //=========================================================================
    void handleSignal(int sig)
    {
        LOGC_NO_LOCK("Received signal %d", sig);
        LOGI(-1, "Received signal %d", sig);

        LoggerPtr spLogger = Logger::GetInstance();

        bool fatalSignal = false;
        bool cleanExit = false;

        switch (sig)
        {
        case SIGINT:
        case SIGTERM:
            LOGC_NO_LOCK("Non-fatal exit signal caught");
            cleanExit = true;
            break;

        case SIGSYS:
        case SIGBUS:
        case SIGILL:
        case SIGFPE:
        case SIGABRT:
        case SIGSEGV:
            LOGC_NO_LOCK("Fatal exit signal caught");
            fatalSignal = true;
            cleanExit = true;
            break;

        default:
            break;
        }

        if (cleanExit)
        {
            int exitCode = 0;

            if (fatalSignal)
            {
                SystemImpl::PrintBacktrace();
                exitCode = sig;
            }

            SystemImpl::CleanExit(exitCode);
        }
    }
}

//=============================================================================
void SystemImpl::PrintBacktrace()
{
    void *trace[10];
    int traceSize = backtrace(trace, 10);
    backtrace_symbols_fd(trace, traceSize, STDERR_FILENO);
}

//=============================================================================
std::string SystemImpl::LocalTime(const std::string &fmt)
{
    auto sys = std::chrono::system_clock::now();
    time_t now = std::chrono::system_clock::to_time_t(sys);

    struct tm timeVal;

    if (localtime_r(&now, &timeVal) != NULL)
    {
        char timeStr[32];

        if (strftime(timeStr, sizeof(timeStr), fmt.c_str(), &timeVal) != 0)
        {
            return std::string(timeStr);
        }
    }

    return std::string();
}

//=============================================================================
std::string SystemImpl::GetLastError(int *pCode)
{
    int error = errno;

    if (pCode != NULL)
    {
        *pCode = error;
    }

    return "(" + std::to_string(error) + ") " + strerror(error);
}

//=============================================================================
void SystemImpl::SetupSignalHandler()
{
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);
    signal(SIGSYS, handleSignal);
    signal(SIGBUS, handleSignal);
    signal(SIGILL, handleSignal);
    signal(SIGFPE, handleSignal);
    signal(SIGABRT, handleSignal);
    signal(SIGSEGV, handleSignal);
}

//=============================================================================
void SystemImpl::CleanExit(int exitCode)
{
    g_aExitCode.store(exitCode);
    g_aKeepRunning.store(false);
}

//=============================================================================
bool SystemImpl::KeepRunning()
{
    return g_aKeepRunning.load();
}

//=============================================================================
int SystemImpl::ExitCode()
{
    return g_aExitCode.load();
}

}
