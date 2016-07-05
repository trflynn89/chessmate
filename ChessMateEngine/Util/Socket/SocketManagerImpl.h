#pragma once

#include <Util/Utilities.h>
#include <Util/Socket/SocketManager.h>

#if defined(BUILD_WINDOWS)
    #include <Util/Socket/WinSocketManager.h>
#elif defined(BUILD_LINUX)
    #include <Util/Socket/NixSocketManager.h>
#endif
