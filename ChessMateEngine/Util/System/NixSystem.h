#pragma once

#include <string>

namespace Util {

/**
 * Linux declaration of the SystemImpl interface.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version February 3, 2016
 */
class SystemImpl
{
public:
    static void PrintBacktrace();
    static std::string LocalTime(const std::string &);
    static std::string GetLastError();
};

}
