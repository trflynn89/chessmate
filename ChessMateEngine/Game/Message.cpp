#include "Message.h"

namespace Game {

//=============================================================================
Message::Message() : m_type(Message::INVALID_TYPE)
{
}

//=============================================================================
Message::Message(const std::string &raw) : m_type(Message::INVALID_TYPE)
{
    // Message type is raw[0]
    if (raw.length() > 0)
    {
        // TODO message type may be more than 1 char
        m_type = (Message::MessageType) std::stoi(raw.substr(0, 1));

        // Message data is raw[2 : end]
        if (raw.length() > 1)
        {
            m_data = raw.substr(2);
        }
    }
}

//=============================================================================
Message::Message(Message::MessageType type, const std::string &data) : m_type(type), m_data(data)
{
}

//=============================================================================
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

//=============================================================================
Message::MessageType Message::GetMessageType() const
{
    return m_type;
}

//=============================================================================
std::string Message::GetData() const
{
    return m_data;
}

//=============================================================================
std::string Message::Serialize() const
{
    std::string ret = std::to_string(m_type);

    if (!m_data.empty())
    {
        ret.append(" ");
        ret.append(m_data);
    }

    return ret;
}

}
