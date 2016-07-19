#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <string>

#include <Util/Utilities.h>
#include <Util/Configuration/Configuration.h>
#include <Util/File/FileMonitorImpl.h>
#include <Util/File/Parser.h>

namespace Util {

DEFINE_CLASS_PTRS(ConfigurationManager);

/**
 * Class to create and manage a set of configurations.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 18, 2016
 */
class ConfigurationManager : public std::enable_shared_from_this<ConfigurationManager>
{
    /**
     * Map of configuration group names to configuration objects.
     */
    typedef std::map<std::string, ConfigurationPtr> ConfigurationMap;

public:
    /**
     * Enumerated list of supported configuration file formats.
     */
    enum ConfigurationFileType
    {
        CONFIG_TYPE_INI
    };

    /**
     * Constructor.
     *
     * @param ConfigurationFileType File format of the configuration file.
     * @param string Directory containing the configuration file.
     * @param string Name of the configuration file.
     */
    ConfigurationManager(
        ConfigurationFileType,
        const std::string &,
        const std::string &
    );

    /**
     * Destructor. Stop the configuration manager.
     */
    virtual ~ConfigurationManager();

    /**
     * Start the configuration manager and underlying objects.
     *
     * @return True if the monitor could be started.
     */
    bool StartConfigurationManager();

    /**
     * Stop the configuration manager and underlying objects.
     */
    void StopConfigurationManager();

    /**
     * Create a configuration object, or if one with the given name exists,
     * fetch it.
     *
     * @param string The name to associate with the configuration.
     *
     * @return A weak reference to the created/found configuration.
     */
    ConfigurationWPtr CreateConfiguration(const std::string &);

    /**
     * Get the number of configuration objects currently created.
     *
     * @return The number of configurations.
     */
    size_t GetSize() const;

private:
    /**
     * Handle an update to the configuration file on disk.
     */
    void onConfigChange(FileMonitor::FileEvent);

    FileMonitorPtr m_spMonitor;
    ParserPtr m_spParser;

    const std::string m_path;
    const std::string m_file;

    mutable std::mutex m_configurationsMutex;
    ConfigurationMap m_configurations;
};

}
