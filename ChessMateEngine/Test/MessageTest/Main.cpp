#include <gtest/gtest.h>

#include <Game/Message.h>
#include <Util/Logging/Logger.h>

//=============================================================================
TEST(MessageTest, ConstructorTest)
{
    // INVALID_TYPE
    Game::Message m1;
    ASSERT_EQ(m1.GetMessageType(), Game::Message::INVALID_TYPE);
    ASSERT_EQ(m1.GetData().length(), 0);

    // START_GAME
    Game::Message m2("0 abc");
    ASSERT_EQ(m2.GetMessageType(), Game::Message::START_GAME);
    ASSERT_EQ(m2.GetData(), "abc");

    // INVALID_MOVE
    Game::Message m3("1 def");
    ASSERT_EQ(m3.GetMessageType(), Game::Message::INVALID_MOVE);
    ASSERT_EQ(m3.GetData(), "def");

    // MAKE_MOVE
    Game::Message m4("2 ghi");
    ASSERT_EQ(m4.GetMessageType(), Game::Message::MAKE_MOVE);
    ASSERT_EQ(m4.GetData(), "ghi");

    // GET_MOVE
    Game::Message m5("3 jkl");
    ASSERT_EQ(m5.GetMessageType(), Game::Message::GET_MOVE);
    ASSERT_EQ(m5.GetData(), "jkl");

    // DISCONNECT
    Game::Message m6("4 mno");
    ASSERT_EQ(m6.GetMessageType(), Game::Message::DISCONNECT);
    ASSERT_EQ(m6.GetData(), "mno");
}

//=============================================================================
TEST(MessageTest, ValidityTest)
{
    // INVALID_TYPE
    Game::Message m1(Game::Message::INVALID_TYPE, "");
    ASSERT_FALSE(m1.IsValid());

    // START_GAME
    Game::Message m2(Game::Message::START_GAME, "");
    ASSERT_FALSE(m2.IsValid());

    Game::Message m3(Game::Message::START_GAME, "0");
    ASSERT_FALSE(m3.IsValid());

    Game::Message m4(Game::Message::START_GAME, "0 ");
    ASSERT_FALSE(m4.IsValid());

    Game::Message m5(Game::Message::START_GAME, "0 0");
    ASSERT_TRUE(m5.IsValid());

    // INVALID_MOVE
    Game::Message m6(Game::Message::INVALID_MOVE, "");
    ASSERT_FALSE(m6.IsValid());

    Game::Message m7(Game::Message::INVALID_MOVE, "0");
    ASSERT_TRUE(m7.IsValid());

    // MAKE_MOVE
    Game::Message m8(Game::Message::MAKE_MOVE, "");
    ASSERT_FALSE(m8.IsValid());

    Game::Message m9(Game::Message::MAKE_MOVE, "0");
    ASSERT_TRUE(m9.IsValid());

    // GET_MOVE
    Game::Message m10(Game::Message::GET_MOVE, "");
    ASSERT_TRUE(m10.IsValid());

    Game::Message m11(Game::Message::GET_MOVE, "0");
    ASSERT_FALSE(m11.IsValid());

    // DISCONNECT
    Game::Message m12(Game::Message::DISCONNECT, "");
    ASSERT_TRUE(m12.IsValid());

    Game::Message m13(Game::Message::DISCONNECT, "0");
    ASSERT_FALSE(m13.IsValid());
}

//=============================================================================
TEST(MessageTest, SerializeTest)
{
    // INVALID_TYPE
    Game::Message m1;
    ASSERT_EQ(m1.Serialize(), "-1");

    // START_GAME
    Game::Message m2("0 abc");
    ASSERT_EQ(m2.Serialize(), "0 abc");

    // INVALID_MOVE
    Game::Message m3("1 def");
    ASSERT_EQ(m3.Serialize(), "1 def");

    // MAKE_MOVE
    Game::Message m4("2 ghi");
    ASSERT_EQ(m4.Serialize(), "2 ghi");

    // GET_MOVE
    Game::Message m5("3 jkl");
    ASSERT_EQ(m5.Serialize(), "3 jkl");

    // DISCONNECT
    Game::Message m6("4 mno");
    ASSERT_EQ(m6.Serialize(), "4 mno");
}
