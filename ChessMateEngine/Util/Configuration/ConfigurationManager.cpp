#include "ConfigurationManager.h"

#include <functional>
#include <memory>

#include <Util/File/IniParser.h>
#include <Util/Logging/Logger.h>

namespace Util {

//==============================================================================
ConfigurationManager::ConfigurationManager(
    ConfigurationFileType fileType,
    const std::string &path,
    const std::string &file
) :
    m_path(path),
    m_file(file)
{
    switch (fileType)
    {
    case CONFIG_TYPE_INI:
        m_spParser = std::make_shared<IniParser>(path, file);
        break;

    default:
        LOGE(-1, "Unrecognized configuration type: %d", fileType);
        break;
    }
}

//==============================================================================
ConfigurationManager::~ConfigurationManager()
{
    StopConfigurationManager();
}

//==============================================================================
bool ConfigurationManager::StartConfigurationManager()
{
    if (m_spParser)
    {
        ConfigurationManagerPtr spThis = shared_from_this();

        static const auto onChange = &ConfigurationManager::onConfigChange;
        auto callback = std::bind(onChange, spThis, std::placeholders::_1);

        m_spMonitor = std::make_shared<FileMonitorImpl>(callback, m_path, m_file);

        if (m_spMonitor->StartMonitor())
        {
            onConfigChange(FileMonitor::FILE_NO_CHANGE);
            return true;
        }
    }

    return false;
}

//==============================================================================
void ConfigurationManager::StopConfigurationManager()
{
    if (m_spMonitor)
    {
        m_spMonitor->StopMonitor();
    }
}

//==============================================================================
ConfigurationWPtr ConfigurationManager::CreateConfiguration(
    const std::string &name
)
{
    ConfigurationPtr spConfiguration;

    std::lock_guard<std::mutex> lock(m_configurationsMutex);
    ConfigurationMap::const_iterator it = m_configurations.find(name);

    if (it == m_configurations.end())
    {
        spConfiguration = std::make_shared<Configuration>();
        spConfiguration->Update(m_spParser->GetValues(name));

        m_configurations[name] = spConfiguration;
    }
    else
    {
        spConfiguration = it->second;
    }

    return spConfiguration;
}

//==============================================================================
size_t ConfigurationManager::GetSize() const
{
    std::lock_guard<std::mutex> lock(m_configurationsMutex);
    return m_configurations.size();
}

//==============================================================================
void ConfigurationManager::onConfigChange(FileMonitor::FileEvent)
{
    try
    {
        m_spParser->Parse();
    }
    catch (const ParserException &)
    {
        LOGW(-1, "Could not parse file, ignoring update");
        return;
    }

    std::lock_guard<std::mutex> lock(m_configurationsMutex);

    for (auto &configuration : m_configurations)
    {
        Parser::ValueList values = m_spParser->GetValues(configuration.first);
        configuration.second->Update(values);
    }
}

}
