#pragma once

#include <Util/Utilities.h>

#if defined(BUILD_WINDOWS)
	#include "WinSocketManager.h"
#elif defined(BUILD_LINUX)
	#include "NixSocketManager.h"
#endif
