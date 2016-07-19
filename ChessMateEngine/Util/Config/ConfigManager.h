#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <string>

#include <Util/Utilities.h>
#include <Util/Config/Config.h>
#include <Util/File/FileMonitorImpl.h>
#include <Util/File/Parser.h>
#include <Util/Traits/TypeTraits.h>

namespace Util {

DEFINE_CLASS_PTRS(ConfigManager);

/**
 * Class to create and manage a set of configurations.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 18, 2016
 */
class ConfigManager : public std::enable_shared_from_this<ConfigManager>
{
    /**
     * Map of configuration group names to configuration objects.
     */
    typedef std::map<std::string, ConfigPtr> ConfigMap;

public:
    /**
     * Enumerated list of supported configuration file formats.
     */
    enum ConfigFileType
    {
        CONFIG_TYPE_INI
    };

    /**
     * Constructor.
     *
     * @param ConfigFileType File format of the configuration file.
     * @param string Directory containing the configuration file.
     * @param string Name of the configuration file.
     */
    ConfigManager(
        ConfigFileType,
        const std::string &,
        const std::string &
    );

    /**
     * Destructor. Stop the configuration manager.
     */
    virtual ~ConfigManager();

    /**
     * Start the configuration manager and underlying objects.
     *
     * @return True if the monitor could be started.
     */
    bool StartConfigManager();

    /**
     * Stop the configuration manager and underlying objects.
     */
    void StopConfigManager();

    /**
     * Create a configuration object, or if one with the given type's name
     * exists, fetch it.
     *
     * @tparam T Config type (must derive from or be Util::Config).
     *
     * @return A reference to the created/found configuration.
     */
    template <typename T, enable_if_all<std::is_base_of<Config, T>>...>
    std::shared_ptr<T> CreateConfig();

    /**
     * Given type is not a configuration, raise compile error.
     */
    template <typename T, enable_if_none<std::is_base_of<Config, T>>...>
    std::shared_ptr<T> CreateConfig();

    /**
     * Get the number of configuration objects currently created.
     *
     * @return The number of configurations.
     */
    ConfigMap::size_type GetSize() const;

private:
    /**
     * Handle an update to the configuration file on disk.
     */
    void onConfigChange(FileMonitor::FileEvent);

    FileMonitorPtr m_spMonitor;
    ParserPtr m_spParser;

    const std::string m_path;
    const std::string m_file;

    mutable std::mutex m_configsMutex;
    ConfigMap m_configs;
};

//==============================================================================
template <typename T, enable_if_all<std::is_base_of<Config, T>>...>
std::shared_ptr<T> ConfigManager::CreateConfig()
{
    std::shared_ptr<T> spConfig;
    std::string name(T::GetName());

    std::lock_guard<std::mutex> lock(m_configsMutex);
    ConfigMap::const_iterator it = m_configs.find(name);

    if (it == m_configs.end())
    {
        spConfig = std::make_shared<T>();
        spConfig->Update(m_spParser->GetValues(name));

        m_configs[name] = spConfig;
    }
    else
    {
        spConfig = std::static_pointer_cast<T>(it->second);
    }

    return spConfig;
}

//==============================================================================
template <typename T, enable_if_none<std::is_base_of<Config, T>>...>
std::shared_ptr<T> ConfigManager::CreateConfig()
{
    static_assert(std::is_base_of<Config, T>::value,
        "Given type is not a configuration type");
}

}
