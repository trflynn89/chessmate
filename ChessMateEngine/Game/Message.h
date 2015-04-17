/**
 * Class to represent a message send over the wire. Contains a message type
 * and message data.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
#pragma once

#include <memory>
#include <string>

#include <ChessMateEngine.h>

namespace ChessMate { namespace Game {

DEFINE_CLASS_PTRS(Message);

class Message
{
public:
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
	 */
	Message(std::string);

	/**
	 * Constructor to store a known type and data.
	 */
	Message(MessageType, std::string);

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
	 */
	std::string Serialize() const;

private:
	Message::MessageType m_type;
	std::string m_data;
};

}}
