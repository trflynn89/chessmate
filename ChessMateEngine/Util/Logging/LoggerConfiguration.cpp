#include "LoggerConfiguration.h"

namespace Util {

//==============================================================================
LoggerConfiguration::LoggerConfiguration()
{
}

//==============================================================================
LoggerConfiguration::~LoggerConfiguration()
{
}

//==============================================================================
std::string LoggerConfiguration::GetName()
{
    return "LoggerConfiguration";
}

//==============================================================================
size_t LoggerConfiguration::MaxLogFileSize() const
{
    return GetValue<size_t>("max_log_file_size", 20 << 20);
}

}
