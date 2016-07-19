#include "LoggerConfig.h"

namespace Util {

//==============================================================================
LoggerConfig::LoggerConfig()
{
}

//==============================================================================
LoggerConfig::~LoggerConfig()
{
}

//==============================================================================
std::string LoggerConfig::GetName()
{
    return "LoggerConfig";
}

//==============================================================================
size_t LoggerConfig::MaxLogFileSize() const
{
    return GetValue<size_t>("max_log_file_size", 20 << 20);
}

}
