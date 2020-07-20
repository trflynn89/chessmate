#include "game_config.h"

#include <fly/types/numeric/literals.hpp>

using namespace fly::literals::numeric_literals;

namespace chessmate {

//==================================================================================================
int GameConfig::AcceptPort() const
{
    return get_value<int>("accept_port", 12389);
}

//==================================================================================================
std::chrono::milliseconds GameConfig::QueueWaitTime() const
{
    return std::chrono::milliseconds(
        get_value<std::chrono::milliseconds::rep>("queue_wait_time", 100_i64));
}

//==================================================================================================
bool GameConfig::IncreaseEndGameDifficulty() const
{
    return get_value<bool>("increase_end_game_difficulty", true);
}

//==================================================================================================
value_type GameConfig::EndGameDifficultyIncrease() const
{
    return get_value<value_type>("end_game_difficulty_increase", 2);
}

} // namespace chessmate
