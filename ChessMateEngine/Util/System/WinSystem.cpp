/**
 * Windows implementation of the SystemImpl interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version November 11, 2013
 */
#include "WinSystem.h"

#include <Windows.h>

#include <Util/Logging/Logger.h>

namespace Util { namespace System {

//=============================================================================
void SystemImpl::PrintBacktrace(int num)
{
	void *trace[num];
	const USHORT traceSize = CaptureStackBackTrace(0, num, trace, NULL);

	for (USHORT i = 0; i < traceSize; ++i)
	{
		LOGC("[%3u] 0x%x", i, trace[i]);
	}
}

}}
