#include "NixSystem.h"

#include <execinfo.h>
#include <unistd.h>

namespace Util {

//=============================================================================
void SystemImpl::PrintBacktrace(int num)
{
    void *trace[num];
    int traceSize = backtrace(trace, num);
    backtrace_symbols_fd(trace, traceSize, STDERR_FILENO);
}

}
