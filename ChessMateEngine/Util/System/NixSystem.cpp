/**
 * Linux implementation of the SystemImpl interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version November 11, 2013
 */
#include "NixSystem.h"

#include <execinfo.h>
#include <unistd.h>

namespace Util { namespace System {

//=============================================================================
void SystemImpl::PrintBacktrace(int num)
{
	void *trace[num];
	int traceSize = backtrace(trace, num);
	backtrace_symbols_fd(trace, traceSize, STDERR_FILENO);
}

}}
