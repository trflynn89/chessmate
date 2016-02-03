#include "NixSystem.h"

#include <execinfo.h>
#include <unistd.h>

namespace Util {

//=============================================================================
void SystemImpl::PrintBacktrace()
{
    void *trace[10];
    int traceSize = backtrace(trace, 10);
    backtrace_symbols_fd(trace, traceSize, STDERR_FILENO);
}

}
