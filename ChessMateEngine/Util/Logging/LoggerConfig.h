#pragma once

#include <string>

#include <Util/Config/Config.h>

namespace Util {

DEFINE_CLASS_PTRS(LoggerConfig);

/**
 * Class to hold configuration values related to the logger.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 18, 2016
 */
class LoggerConfig : public Config
{
public:
    /**
     * Constructor.
     */
    LoggerConfig();

    /**
     * Destructor.
     */
    virtual ~LoggerConfig();

    /**
     * Get the name to associate with this configuration.
     */
    static std::string GetName();

    /**
     * @return Max log file size (in bytes) before rotating the log file.
     */
    size_t MaxLogFileSize() const;
};

}