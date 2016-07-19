#include "Configuration.h"

namespace Util {

//==============================================================================
Configuration::Configuration()
{
}

//==============================================================================
Configuration::~Configuration()
{
}

//==============================================================================
std::string Configuration::GetName()
{
    return "Configuration";
}

//==============================================================================
void Configuration::Update(const Parser::ValueList &values)
{
    std::unique_lock<std::shared_timed_mutex> lock(m_valuesMutex);
    m_values.clear();

    for (const Parser::Value &value : values)
    {
        m_values[value.first] = value.second;
    }
}

}
