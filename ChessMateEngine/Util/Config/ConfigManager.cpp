#include "ConfigManager.h"

#include <functional>
#include <memory>

#include <Util/File/IniParser.h>
#include <Util/Logging/Logger.h>

namespace Util {

//==============================================================================
ConfigManager::ConfigManager(
    ConfigFileType fileType,
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
ConfigManager::~ConfigManager()
{
    StopConfigManager();
}

//==============================================================================
bool ConfigManager::StartConfigManager()
{
    if (m_spParser)
    {
        ConfigManagerPtr spThis = shared_from_this();

        static const auto onChange = &ConfigManager::onConfigChange;
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
void ConfigManager::StopConfigManager()
{
    if (m_spMonitor)
    {
        m_spMonitor->StopMonitor();
    }
}

//==============================================================================
size_t ConfigManager::GetSize() const
{
    std::lock_guard<std::mutex> lock(m_configsMutex);
    return m_configs.size();
}

//==============================================================================
void ConfigManager::onConfigChange(FileMonitor::FileEvent)
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

    std::lock_guard<std::mutex> lock(m_configsMutex);

    for (auto &configuration : m_configs)
    {
        Parser::ValueList values = m_spParser->GetValues(configuration.first);
        configuration.second->Update(values);
    }
}

}
