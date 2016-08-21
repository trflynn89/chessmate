#include "game_config.h"

namespace Game {

//==============================================================================
GameConfig::GameConfig()
{
}

//==============================================================================
GameConfig::~GameConfig()
{
}

//==============================================================================
std::string GameConfig::GetName()
{
    return "game";
}

//==============================================================================
int GameConfig::AcceptPort() const
{
    return GetValue<int>("accept_port", 12389);
}

//==============================================================================
std::chrono::seconds GameConfig::QueueWaitTime() const
{
    return std::chrono::seconds(
        GetValue<std::chrono::seconds::rep>("queue_wait_time", I64(1))
    );
}

//==============================================================================
bool GameConfig::IncreaseEndGameDifficulty() const
{
    return GetValue<bool>("increase_end_game_difficulty", true);
}

//==============================================================================
value_type GameConfig::EndGameDifficultyIncrease() const
{
    return GetValue<value_type>("end_game_difficulty_increase", 2);
}

}
