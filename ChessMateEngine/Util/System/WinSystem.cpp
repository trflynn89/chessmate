#include "WinSystem.h"

#include <Windows.h>

#include <Util/Logging/Logger.h>

namespace Util {

//=============================================================================
void SystemImpl::PrintBacktrace(int num)
{
    void *trace[num];
    const USHORT traceSize = CaptureStackBackTrace(0, num, trace, NULL);

    for (USHORT i = 0; i < traceSize; ++i)
    {
        LOGC("[%3u] %x", i, trace[i]);
    }
}

}
