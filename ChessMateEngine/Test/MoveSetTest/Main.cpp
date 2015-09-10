#include <iostream>
#include <vector>

#include <Movement/MoveSet2.h>
#include <Util/Logging/Logger.h>

int main()
{
    Movement::MoveSet ms;
    Movement::MoveLists w = ms.GetWhitePawnMoves();

    for (int i = 0; i < Game::BOARD_SIZE; ++i)
    {
        std::cout << w[i].size() << std::endl;
    }

    return 0;
}
