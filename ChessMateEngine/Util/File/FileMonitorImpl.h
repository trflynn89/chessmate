#pragma once

#include <Util/Utilities.h>
#include <Util/File/FileMonitor.h>

#if defined(BUILD_WINDOWS)
    #include <Util/File/WinFileMonitor.h>
#elif defined(BUILD_LINUX)
    #include <Util/File/NixFileMonitor.h>
#endif
