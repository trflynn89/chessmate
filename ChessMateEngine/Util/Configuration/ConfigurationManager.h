#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <string>

#include <Util/Utilities.h>
#include <Util/Configuration/Configuration.h>
#include <Util/File/FileMonitorImpl.h>
#include <Util/File/Parser.h>
#include <Util/Traits/TypeTraits.h>

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
     * Create a configuration object, or if one with the given type's name
     * exists, fetch it.
     *
     * @tparam T Configuration type (must derive from or be Util::Configuration).
     *
     * @return A reference to the created/found configuration.
     */
    template <typename T, enable_if_all<std::is_base_of<Configuration, T>>...>
    std::shared_ptr<T> CreateConfiguration();

    /**
     * Given type is not a configuration, raise compile error.
     */
    template <typename T, enable_if_none<std::is_base_of<Configuration, T>>...>
    std::shared_ptr<T> CreateConfiguration();

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

//==============================================================================
template <typename T, enable_if_all<std::is_base_of<Configuration, T>>...>
std::shared_ptr<T> ConfigurationManager::CreateConfiguration()
{
    std::shared_ptr<T> spConfiguration;
    std::string name(T::GetName());

    std::lock_guard<std::mutex> lock(m_configurationsMutex);
    ConfigurationMap::const_iterator it = m_configurations.find(name);

    if (it == m_configurations.end())
    {
        spConfiguration = std::make_shared<T>();
        spConfiguration->Update(m_spParser->GetValues(name));

        m_configurations[name] = spConfiguration;
    }
    else
    {
        spConfiguration = std::static_pointer_cast<T>(it->second);
    }

    return spConfiguration;
}

//==============================================================================
template <typename T, enable_if_none<std::is_base_of<Configuration, T>>...>
std::shared_ptr<T> ConfigurationManager::CreateConfiguration()
{
    static_assert(std::is_base_of<Configuration, T>::value,
        "Given type is not a configuration type");
}

}
