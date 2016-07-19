#pragma once

#include <map>
#include <shared_mutex>

#include <Util/Utilities.h>
#include <Util/File/Parser.h>
#include <Util/String/String.h>
#include <Util/Logging/Logger.h>

namespace Util {

DEFINE_CLASS_PTRS(Configuration);

/**
 * Class to hold a set of related configuration values.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 18, 2016
 */
class Configuration
{
    /**
     * Map of configuration names to their values. This is just an unfolding of
     * Parser::ValueList.
     */
    typedef std::map<
        Parser::Value::first_type,
        Parser::Value::second_type> ValueMap;

public:
    /**
     * Constructor.
     */
    Configuration();

    /**
     * Destructor.
     */
    virtual ~Configuration();

    /**
     * Get a value converted to a basic type, e.g. int or bool. If the value
     * could not be found, or could not be converted to the given type, returns
     * the provided default value.
     *
     * @tparam T The basic return type of the value.
     *
     * @param string The name of the value.
     * @param T Default value to use if the value could not be found or converted.
     *
     * @return The converted value or the default value.
     */
    template <typename T>
    T GetValue(const std::string &, T) const;

    /**
     * Update this configuration with a new set of parsed values.
     */
    void Update(const Parser::ValueList &);

private:
    mutable std::shared_timed_mutex m_valuesMutex;
    ValueMap m_values;
};

//==============================================================================
template <typename T>
T Configuration::GetValue(const std::string &name, T def) const
{
    std::shared_lock<std::shared_timed_mutex> lock(m_valuesMutex);
    ValueMap::const_iterator it = m_values.find(name);

    if (it != m_values.end())
    {
        try
        {
            return String::Convert<T>(it->second);
        }
        catch (...)
        {
            LOGW(-1, "Could not parse: %s = %s", name, it->second);
        }
    }
    else
    {
        LOGW(-1, "Could not find: %s", name);
    }

    return def;
}

}
