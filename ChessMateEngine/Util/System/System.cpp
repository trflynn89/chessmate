/**
 * Implementation of the System interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version November 11, 2013
 */
#include "System.h"

#if defined(BUILD_WINDOWS)
	#include "WinSystem.h"
#elif defined(BUILD_LINUX)
	#include "NixSystem.h"
#endif

namespace Util { namespace System {

//=============================================================================
void System::PrintBacktrace(int num)
{
	SystemImpl::PrintBacktrace(num);
}

}}
