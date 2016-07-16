#pragma once

#include <map>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include <Util/Utilities.h>
#include <Util/File/Parser.h>
#include <Util/Logging/Logger.h>
#include <Util/String/String.h>

namespace Util {

DEFINE_CLASS_PTRS(IniParser);

/**
 * Implementation of the Parser interface for .ini files.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 16, 2016
 */
class IniParser : public Parser
{
    /**
     * A parsed name-value pair.
     */
    typedef std::pair<std::string, std::string> IniValue;

    /**
     * A list of parsed name-value pairs.
     */
    typedef std::vector<IniValue> IniValueList;

    /**
     * A map of parsed section names to that section's list of name-value pairs.
     */
    typedef std::map<std::string, IniValueList> IniSection;

public:
    /**
     * Constructor.
     *
     * @param string Directory containing the file to parse.
     * @param string Name of the file to parse.
     */
    IniParser(const std::string &, const std::string &);

    /**
     * Parse the configured file and store parsed values.
     *
     * @throws ParserException Thrown if an error occurs parsing the file.
     */
    virtual void Parse();

    /**
     * Get a value from the parser converted to a basic type, e.g. int or bool.
     * If the value could not be returned, or could not be converted to the
     * given type, returns a provided default value.
     *
     * @tparam T The basic return type of the value.
     *
     * @param string The name of the section containing the value.
     * @param string The name of the value.
     * @param T Default value to use if the value could not be found or converted.
     *
     * @return The converted value or the default value.
     */
    template <typename T>
    T GetValue(const std::string &, const std::string &, T) const;

    /**
     * Get the number of sections that have been parsed.
     *
     * @return The number of parsed sections.
     */
    size_t GetSize() const;

    /**
     * Get the number of name/value pairs that have been parsed for a section.
     *
     * @param string The name of the section to count.
     *
     * @return The number of parsed values, or -1 if the section doesn't exist.
     */
    ssize_t GetSize(const std::string &) const;

private:
    /**
     * Parse a line containing a section name.
     *
     * @param string Line containing the section.
     *
     * @return The parsed section name.
     */
    std::string onSection(const std::string &);

    /**
     * Parse a line containing a name/value pair.
     *
     * @param string Section containing the pair.
     * @param string Line containing the pair.
     */
    void onValue(const std::string &, const std::string &);

    /**
     * If the given string begins and ends with the given character, remove that
     * character from each end of the string.
     *
     * @param string The string the trim.
     * @param char The character to look for.
     *
     * @return True if the string was trimmed.
     *
     * @throws ParserException Thrown if the character was found at one end of
     *     the string, but not the other.
     */
    bool trimValue(std::string &, char) const;

    /**
     * If the given string begins with the first given character and ends with
     * the second given character, remove those characters from each end of the
     * string.
     *
     * @param string The string the trim.
     * @param char The character to look for at the beginning of the string.
     * @param char The character to look for at the end of the string.
     *
     * @return True if the string was trimmed.
     *
     * @throws ParserException Thrown if the one of the start/end characters
     *     was found, but not the other.
     */
    bool trimValue(std::string &, char, char) const;

    /**
     * Get a value from the parser.
     *
     * @param string The name of the section containing the value.
     * @param string The name of the value.
     * @param string String to store the parsed value in.
     *
     * @return True if the value could be found.
     */
    bool getValue(const std::string &, const std::string &, std::string &) const;

    mutable std::mutex m_sectionsMutex;
    IniSection m_sections;
};

//==============================================================================
template <typename T>
T IniParser::GetValue(const std::string &section, const std::string &key, T def) const
{
    std::lock_guard<std::mutex> lock(m_sectionsMutex);
    std::string value;

    if (getValue(section, key, value))
    {
        try
        {
            return String::Convert<T>(value);
        }
        catch (...)
        {
            LOGW(-1, "Could not parse: [%s] %s = %s", section, key, value);
        }
    }
    else
    {
        LOGW(-1, "Could not find: [%s] %s", section, key);
    }

    return def;
}

}
