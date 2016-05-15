#pragma once

#include <string>

#include <Util/Utilities.h>

namespace Util {

/**
 * Static class to provide interface to system calls.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version May 15, 2016
 */
class System
{
public:
    /**
     * Print the backtrace to stderr.
     */
    static void PrintBacktrace();

    /**
     * @return The local time formatted as a string.
     */
    static std::string LocalTime();

    /**
     * Get the last system error as a string, and optionally store the numeric
     * error code.
     *
     * @param int* Pointer to store the numeric error code, or NULL.
     *
     * @return The last system error as a string.
     */
    static std::string GetLastError(int *code = NULL);

    /**
     * Setup handlers for fatal and non-fatal exit codes, to allow the system
     * to cleanly exit;
     */
    static void SetupSignalHandler();

    /**
     * Signal the main thread to exit with the given exit code.
     *
     * @param int Code to exit with.
     */
    static void CleanExit(int);

    /**
     * @return Whether the system is in a state in which it should keep running.
     */
    static bool KeepRunning();

    /**
     * @return The code the system should exit with.
     */
    static int ExitCode();
};

}
