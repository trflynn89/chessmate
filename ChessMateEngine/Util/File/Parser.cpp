#include "Parser.h"

#include <Util/String/String.h>

namespace Util {

//=============================================================================
Parser::Parser(const std::string &path, const std::string &file) :
    m_path(path),
    m_file(file),
    m_line(1)
{
}

//=============================================================================
ParserException::ParserException(
    const std::string &file,
    int line,
    const std::string &message
) :
    m_message(String::Format("ParserException: Error parsing %s on line %d: %s",
        file, line, message))
{
}

//=============================================================================
const char *ParserException::what() const noexcept
{
    return m_message.c_str();
}

}
