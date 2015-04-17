#pragma once

#include <Util/Utilities.h>

#if defined(BUILD_WINDOWS)
	#include "WinSocket.h"
#elif defined (BUILD_LINUX)
	#include "NixSocket.h"
#endif
