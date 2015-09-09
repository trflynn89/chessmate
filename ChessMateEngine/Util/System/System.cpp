#include "System.h"

#if defined(BUILD_WINDOWS)
    #include "WinSystem.h"
#elif defined(BUILD_LINUX)
    #include "NixSystem.h"
#endif

namespace Util {

//=============================================================================
void System::PrintBacktrace(int num)
{
    SystemImpl::PrintBacktrace(num);
}

}
