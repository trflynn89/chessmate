#include <iostream>
#include <vector>

#include <Movement/WholeMoveSet.h>
#include <Util/Logging/Logger.h>

int main()
{
    Movement::WholeMoveSet ms;
    Movement::MoveLists w = ms.GetWhitePawnMoves();

    for (int i = 0; i < Game::BOARD_SIZE; ++i)
    {
        std::cout << w[i].size() << std::endl;
    }

    return 0;
}
