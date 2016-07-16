#include "IniParser.h"

#include <cstring>
#include <fstream>

#include <Util/Logging/Logger.h>
#include <Util/System/System.h>

namespace Util {

//=============================================================================
IniParser::IniParser(const std::string &path, const std::string &file) :
    Parser(path, file)
{
}

//=============================================================================
void IniParser::Parse()
{
    std::string fullPath = String::Join(System::GetSeparator(), m_path, m_file);
    std::ifstream stream(fullPath.c_str(), std::ios::in);

    std::string line, section;
    m_line = 0;

    std::lock_guard<std::mutex> lock(m_sectionsMutex);
    m_sections.clear();

    while (stream.good() && std::getline(stream, line))
    {
        String::Trim(line);
        ++m_line;

        if (line.empty() || String::StartsWith(line, ';'))
        {
            // Ignore comments and blank lines
        }
        else if (trimValue(line, '[', ']'))
        {
            section = onSection(line);
        }
        else if (!section.empty())
        {
            onValue(section, line);
        }
        else
        {
            throw ParserException(m_file, m_line,
                "A section must be defined before name=value pairs"
            );
        }
    }
}

//=============================================================================
size_t IniParser::GetSize() const
{
    std::lock_guard<std::mutex> lock(m_sectionsMutex);
    return m_sections.size();
}

//=============================================================================
ssize_t IniParser::GetSize(const std::string &section) const
{
    std::lock_guard<std::mutex> lock(m_sectionsMutex);

    IniSection::const_iterator it = m_sections.find(section);
    ssize_t size = -1;

    if (it != m_sections.end())
    {
        size = it->second.size();
    }

    return size;
}

//=============================================================================
std::string IniParser::onSection(const std::string &line)
{
    std::string section = line;
    String::Trim(section);

    if (m_sections.find(section) != m_sections.end())
    {
        throw ParserException(m_file, m_line,
            "Section names must be unique"
        );
    }
    else if (trimValue(section, '\'') || trimValue(section, '\"'))
    {
        throw ParserException(m_file, m_line,
            "Section names must not be quoted"
        );
    }

    return section;
}

//=============================================================================
void IniParser::onValue(const std::string &section, const std::string &line)
{
    static const size_t size = 2;

    std::vector<std::string> nameValue = String::Split(line, '=', size);

    if (nameValue.size() == size)
    {
        std::string name(nameValue[0]), value(nameValue[1]);

        String::Trim(name);
        String::Trim(value);

        if (trimValue(name, '\'') || trimValue(name, '\"'))
        {
            throw ParserException(m_file, m_line,
                "Value names must not be quoted"
            );
        }

        trimValue(value, '\'');
        trimValue(value, '\"');

        IniValueList &list = m_sections[section];

        for (const IniValue &value : list)
        {
            if (name.compare(value.first) == 0)
            {
                throw ParserException(m_file, m_line,
                    "Value names must be unique within a section"
                );
            }
        }

        list.push_back(IniValue(name, value));
    }
    else
    {
        throw ParserException(m_file, m_line,
            "Require name/value pairs of the form name=value"
        );
    }
}

//=============================================================================
bool IniParser::trimValue(std::string &str, char ch) const
{
    return trimValue(str, ch, ch);
}

//=============================================================================
bool IniParser::trimValue(std::string &str, char start, char end) const
{
    bool startsWithChar = String::StartsWith(str, start);
    bool endsWithChar = String::EndsWith(str, end);

    if (startsWithChar || endsWithChar)
    {
        if (startsWithChar && endsWithChar)
        {
            str = str.substr(1, str.size() - 2);
        }
        else
        {
            throw ParserException(m_file, m_line, String::Format(
                "Imbalanced characters: \"%c\" and \"%c\"", start, end
            ));
        }
    }

    return (startsWithChar && endsWithChar);
}

//=============================================================================
bool IniParser::getValue(
    const std::string &section,
    const std::string &key,
    std::string &value
) const
{
    IniSection::const_iterator it = m_sections.find(section);

    if (it != m_sections.end())
    {
        for (const IniValue &iniValue : it->second)
        {
            if (key.compare(iniValue.first) == 0)
            {
                value = iniValue.second;
                return true;
            }
        }
    }

    return false;
}

}
