#pragma once

#include "game/board_types.h"

#include <fly/config/config.hpp>

#include <chrono>

namespace chessmate {

/**
 * Class to hold configuration values related to games and the game manager.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 19, 2016
 */
class GameConfig : public fly::config::Config
{
public:
    static constexpr const char *identifier = "game";

    /**
     * @return Port used to listen for new game connections.
     */
    int AcceptPort() const;

    /**
     * @return Sleep time for logger IO thread.
     */
    std::chrono::milliseconds QueueWaitTime() const;

    /**
     * @return Whether to increase difficulty during the end game.
     */
    bool IncreaseEndGameDifficulty() const;

    /**
     * @return Amount by which to increase game difficulty during the end game.
     */
    value_type EndGameDifficultyIncrease() const;
};

} // namespace chessmate
