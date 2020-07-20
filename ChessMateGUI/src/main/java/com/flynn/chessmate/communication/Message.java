package com.flynn.chessmate.communication;

/**
 * Class to represent a message send over the wire. Contains a message type
 * and message data.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
public class Message
{
    static private char CHESSMATE_EOM = 0x04;

    private MessageType m_type;
    private String m_data;

    // Enumeration of message types
    public enum MessageType {
        INVALID_TYPE(-1),
        START_GAME(0),
        INVALID_MOVE(1),
        MAKE_MOVE(2),
        GET_MOVE(3),
        DISCONNECT(4),
        NUM_TYPES(5);

        private final int m_val;

        MessageType(int val)
        {
            m_val = val;
        }

        public int getValue()
        {
            return m_val;
        }

        public String toString()
        {
            return String.valueOf(m_val);
        }
    }

    public static boolean isEoM(char character)
    {
        return (character == CHESSMATE_EOM);
    }

    /**
     * Default constructor to create an invalid message.
     */
    public Message()
    {
        m_type = MessageType.INVALID_TYPE;
        m_data = "";
    }

    /**
     * Constructor to determine type and data from a raw string.
     */
    public Message(String raw)
    {
        if (raw != null && raw.length() > 0)
        {
            m_type = MessageType.values()[(Integer.parseInt(raw.substring(0, 1))) + 1];

            m_data = raw.substring(2);
        }
        else
        {
            m_type = MessageType.INVALID_TYPE;
            m_data = "";
        }
    }

    /**
     * Constructor to stored a known type.
     */
    public Message(MessageType type)
    {
        m_type = type;
        m_data = "";
    }

    /**
     * Constructor to stored a known type and data.
     */
    public Message(MessageType type, String data)
    {
        m_type = type;
        m_data = data;
    }

    /**
     * @return The message's enumerated type.
     */
    public MessageType getMessageType()
    {
        return m_type;
    }

    /**
     * @return The message's data.
     */
    public String getData()
    {
        return m_data;
    }

    /**
     * Return this message as a string.
     */
    public String serialize()
    {
        return (m_type.toString() + " " + m_data + CHESSMATE_EOM);
    }
}
