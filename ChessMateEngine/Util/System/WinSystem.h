#pragma once

#include <string>

namespace Util {

/**
 * Windows declaration of the SystemImpl interface.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version May 15, 2016
 */
class SystemImpl
{
public:
    static bool MakeDirectory(const std::string &);
    static char GetSeparator();
    static std::string GetTempDirectory();
    static void PrintBacktrace();
    static std::string LocalTime(const std::string &);
    static std::string GetLastError(int *);
    static void SetupSignalHandler();
    static void CleanExit(int);
    static bool KeepRunning();
    static int ExitCode();
};

}
