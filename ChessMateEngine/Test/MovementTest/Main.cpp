#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include <Movement/MoveSet.h>
#include <Util/Logging/Logger.h>

//==============================================================================
TEST(MovementTest, FooTest)
{
    Movement::MoveSet ms;

    for (int i = 0; i < Game::BOARD_SIZE; ++i)
    {
        Movement::MoveList w = ms.GetWhitePawnMoves(i);
        LOGC("%d", w.size());
    }
}
