#pragma once

#include <Util/Utilities.h>

namespace Util {

/**
 * Static class to provide interface to system calls.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version November 11, 2013
 */
class System
{
public:
    /**
     * Print the backtrace to stderr.
     */
    static void PrintBacktrace();
};

}
