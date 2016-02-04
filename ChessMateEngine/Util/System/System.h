#pragma once

#include <string>

#include <Util/Utilities.h>

namespace Util {

/**
 * Static class to provide interface to system calls.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version February 3, 2016
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
};

}
