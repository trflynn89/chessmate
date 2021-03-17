#include "message.h"

#include <fly/types/string/string.hpp>
#include <fly/types/string/string_lexer.hpp>

#include <optional>

namespace chessmate {

namespace {

    char s_chessmate_eom = 0x04;

} // namespace

//==================================================================================================
Message::Message() : m_type(Message::INVALID_TYPE)
{
}

//==================================================================================================
Message::Message(const std::string &raw) : m_type(Message::INVALID_TYPE)
{
    using StringLexer = fly::BasicStringLexer<std::string>;

    StringLexer lexer(raw);

    if (auto type = lexer.consume_number(); type)
    {
        m_type = static_cast<Message::MessageType>(type.value());

        if (!lexer.consume_if(' '))
        {
            return;
        }
        else if (lexer.position() >= raw.size())
        {
            return;
        }

        m_data = raw.substr(lexer.position(), lexer.view().size() - lexer.position() - 1);
    }
}

//==================================================================================================
Message::Message(Message::MessageType type, const std::string &data) : m_type(type), m_data(data)
{
}

//==================================================================================================
bool Message::IsValid() const
{
    bool isValid = false;

    switch (m_type)
    {
        // START_GAME data of the form "<engine color> <difficulty>"
        case Message::START_GAME:
            isValid = (m_data.length() > 2);
            break;

        // MAKE_MOVE, INVALID_MOVE data of the form "<PGN string> [stalemate]"
        case Message::INVALID_MOVE:
        case Message::MAKE_MOVE:
            isValid = (m_data.length() > 0);
            break;

        // GET_MOVE, DISCONNECT have no data
        case Message::GET_MOVE:
        case Message::DISCONNECT:
            isValid = (m_data.length() == 0);
            break;

        default:
            break;
    }

    return isValid;
}

//==================================================================================================
Message::MessageType Message::GetMessageType() const
{
    return m_type;
}

//==================================================================================================
std::string Message::GetData() const
{
    return m_data;
}

//==================================================================================================
std::string Message::Serialize() const
{
    return fly::String::format(
        "{}{}{}",
        m_type,
        (m_data.empty() ? "" : " " + m_data),
        s_chessmate_eom);
}

} // namespace chessmate
