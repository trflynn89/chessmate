#pragma once

#include <chrono>
#include <string>

#include <fly/fly.h>
#include <fly/config/config.h>

#include <game/board_types.h>

namespace Game {

DEFINE_CLASS_PTRS(GameConfig);

/**
 * Class to hold configuration values related to games and the game manager.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 19, 2016
 */
class GameConfig : public fly::Config
{
public:
    /**
     * Constructor.
     */
    GameConfig();

    /**
     * Destructor.
     */
    virtual ~GameConfig();

    /**
     * Get the name to associate with this configuration.
     */
    static std::string GetName();

    /**
     * @return Port used to listen for new game connections.
     */
    int AcceptPort() const;

    /**
     * @return Sleep time for logger IO thread.
     */
    std::chrono::seconds QueueWaitTime() const;

    /**
     * @return Whether to increase difficulty during the end game.
     */
    bool IncreaseEndGameDifficulty() const;

    /**
     * @return Amount by which to increase game difficulty during the end game.
     */
    value_type EndGameDifficultyIncrease() const;
};

}
