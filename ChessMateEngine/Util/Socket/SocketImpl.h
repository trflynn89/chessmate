#pragma once

#include <Util/Utilities.h>
#include <Util/Socket/Socket.h>

#if defined(BUILD_WINDOWS)
    #include <Util/Socket/WinSocket.h>
#elif defined(BUILD_LINUX)
    #include <Util/Socket/NixSocket.h>
#endif
