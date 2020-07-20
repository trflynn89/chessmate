#pragma once

#include <string>

namespace chessmate {

/**
 * Class to represent a message send over the wire. Contains a message type
 * and message data.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version March 3, 2013
 */
class Message
{
public:
    /**
     * Enumerated list of message types.
     */
    enum MessageType
    {
        INVALID_TYPE = -1,
        START_GAME,
        INVALID_MOVE,
        MAKE_MOVE,
        GET_MOVE,
        DISCONNECT
    };

    /**
     * Default constructor to create an invalid message.
     */
    Message();

    /**
     * Constructor to determine type and data from a raw string.
     *
     * @param string The raw string to parse.
     */
    Message(const std::string &);

    /**
     * Constructor to store a known type and data.
     *
     * @param MessageType The type ID of the message.
     * @param string The message's data.
     */
    Message(MessageType, const std::string &);

    /**
     * Determine if the message is valid. Validity depends on the message type.
     *
     * @return True if the message is valid, false otherwise.
     */
    bool IsValid() const;

    /**
     * @return The message's enumerated type.
     */
    Message::MessageType GetMessageType() const;

    /**
     * @return The message's data.
     */
    std::string GetData() const;

    /**
     * Return this message as a string.
     *
     * @return A string that can be sent over-the-wire.
     */
    std::string Serialize() const;

private:
    Message::MessageType m_type;
    std::string m_data;
};

} // namespace chessmate
