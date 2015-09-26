#include "WinSystem.h"

#include <Windows.h>

#include <Util/Logging/Logger.h>

namespace Util {

//=============================================================================
void SystemImpl::PrintBacktrace()
{
    void *trace[10];
    const USHORT traceSize = CaptureStackBackTrace(0, 10, trace, NULL);

    for (USHORT i = 0; i < traceSize; ++i)
    {
        LOGC("[%3u] %x", i, trace[i]);
    }
}

}
