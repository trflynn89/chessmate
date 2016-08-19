#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include <fly/logging/logger.h>

#include <Movement/MoveSet.h>

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
