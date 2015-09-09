#include <Game/Message.h>
#include <Util/Logging/Logger.h>

//=============================================================================
bool RunConstructorTest()
{
    // INVALID_TYPE
    Game::Message m1;
    if (m1.GetMessageType() != Game::Message::INVALID_TYPE)
    {
        LOGC("Expected message type to be INVALID_TYPE");
        return false;
    }
    else if (m1.GetData().length() != 0)
    {
        LOGC("Expected message data to be empty");
        return false;
    }

    // START_GAME
    Game::Message m2("0 abc");
    if (m2.GetMessageType() != Game::Message::START_GAME)
    {
        LOGC("Expected message type to be START_GAME");
        return false;
    }
    else if (m2.GetData() != "abc")
    {
        LOGC("Expected message data to be \"abc\"");
        return false;
    }

    // INVALID_MOVE
    Game::Message m3("1 def");
    if (m3.GetMessageType() != Game::Message::INVALID_MOVE)
    {
        LOGC("Expected message type to be INVALID_MOVE");
        return false;
    }
    else if (m3.GetData() != "def")
    {
        LOGC("Expected message data to be \"def\"");
        return false;
    }

    // MAKE_MOVE
    Game::Message m4("2 ghi");
    if (m4.GetMessageType() != Game::Message::MAKE_MOVE)
    {
        LOGC("Expected message type to be MAKE_MOVE");
        return false;
    }
    else if (m4.GetData() != "ghi")
    {
        LOGC("Expected message data to be \"ghi\"");
        return false;
    }

    // GET_MOVE
    Game::Message m5("3 jkl");
    if (m5.GetMessageType() != Game::Message::GET_MOVE)
    {
        LOGC("Expected message type to be GET_MOVE");
        return false;
    }
    else if (m5.GetData() != "jkl")
    {
        LOGC("Expected message data to be \"jkl\"");
        return false;
    }

    // DISCONNECT
    Game::Message m6("4 mno");
    if (m6.GetMessageType() != Game::Message::DISCONNECT)
    {
        LOGC("Expected message type to be DISCONNECT");
        return false;
    }
    else if (m6.GetData() != "mno")
    {
        LOGC("Expected message data to be \"mno\"");
        return false;
    }

    return true;
}

//=============================================================================
bool RunIsValidTest()
{
    // INVALID_TYPE
    Game::Message m1(Game::Message::INVALID_TYPE, "");
    if (m1.IsValid())
    {
        LOGC("INVALID_TYPE message incorrectly valid");
        return false;
    }

    // START_GAME
    Game::Message m2(Game::Message::START_GAME, "");
    if (m2.IsValid())
    {
        LOGC("START_GAME message incorrectly valid");
        return false;
    }

    Game::Message m3(Game::Message::START_GAME, "0");
    if (m3.IsValid())
    {
        LOGC("START_GAME message incorrectly valid");
        return false;
    }

    Game::Message m4(Game::Message::START_GAME, "0 ");
    if (m4.IsValid())
    {
        LOGC("START_GAME message incorrectly valid");
        return false;
    }

    Game::Message m5(Game::Message::START_GAME, "0 0");
    if (!m5.IsValid())
    {
        LOGC("START_GAME message incorrectly invalid");
        return false;
    }

    // INVALID_MOVE
    Game::Message m6(Game::Message::INVALID_MOVE, "");
    if (m6.IsValid())
    {
        LOGC("INVALID_MOVE message incorrectly valid");
        return false;
    }

    Game::Message m7(Game::Message::INVALID_MOVE, "0");
    if (!m7.IsValid())
    {
        LOGC("INVALID_MOVE message incorrectly invalid");
        return false;
    }

    // MAKE_MOVE
    Game::Message m8(Game::Message::MAKE_MOVE, "");
    if (m8.IsValid())
    {
        LOGC("MAKE_MOVE message incorrectly valid");
        return false;
    }

    Game::Message m9(Game::Message::MAKE_MOVE, "0");
    if (!m9.IsValid())
    {
        LOGC("MAKE_MOVE message incorrectly invalid");
        return false;
    }

    // GET_MOVE
    Game::Message m10(Game::Message::GET_MOVE, "");
    if (!m10.IsValid())
    {
        LOGC("GET_MOVE message incorrectly invalid");
        return false;
    }

    Game::Message m11(Game::Message::GET_MOVE, "0");
    if (m11.IsValid())
    {
        LOGC("GET_MOVE message incorrectly valid");
        return false;
    }

    // DISCONNECT
    Game::Message m12(Game::Message::DISCONNECT, "");
    if (!m12.IsValid())
    {
        LOGC("DISCONNECT message incorrectly invalid");
        return false;
    }

    Game::Message m13(Game::Message::DISCONNECT, "0");
    if (m13.IsValid())
    {
        LOGC("DISCONNECT message incorrectly valid");
        return false;
    }

    return true;
}

//=============================================================================
bool RunSerializeTest()
{
    // INVALID_TYPE
    Game::Message m1;
    if (m1.Serialize() != "-1")
    {
        LOGC("Unexpected value while serializing INVALID_TYPE");
        return false;
    }

    // START_GAME
    Game::Message m2("0 abc");
    if (m2.Serialize() != "0 abc")
    {
        LOGC("Expected message data to be \"abc\"");
        return false;
    }

    // INVALID_MOVE
    Game::Message m3("1 def");
    if (m3.Serialize() != "1 def")
    {
        LOGC("Expected message data to be \"def\"");
        return false;
    }

    // MAKE_MOVE
    Game::Message m4("2 ghi");
    if (m4.Serialize() != "2 ghi")
    {
        LOGC("Expected message data to be \"ghi\"");
        return false;
    }

    // GET_MOVE
    Game::Message m5("3 jkl");
    if (m5.Serialize() != "3 jkl")
    {
        LOGC("Expected message data to be \"jkl\"");
        return false;
    }

    // DISCONNECT
    Game::Message m6("4 mno");
    if (m6.Serialize() != "4 mno")
    {
        LOGC("Expected message data to be \"mno\"");
        return false;
    }

    return true;
}

//=============================================================================
int main()
{
    int numErrors = 0;

    if (RunConstructorTest())
    {
        LOGC("Constructor test passed");
    }
    else
    {
        LOGC("Constructor test failed");
        ++numErrors;
    }

    if (RunIsValidTest())
    {
        LOGC("Is valid test passed");
    }
    else
    {
        LOGC("Is valid test failed");
        ++numErrors;
    }

    if (RunSerializeTest())
    {
        LOGC("Serialize test passed");
    }
    else
    {
        LOGC("Serialize test failed");
        ++numErrors;
    }

    return numErrors;
}
