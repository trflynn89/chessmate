#include <algorithm>

#include "MoveSelector.h"
#include <Movement/ValidMoveSet.h>

namespace Engine {

namespace
{
    static const int s_posInfinity =  32767;
    static const int s_negInfinity = -32767;
}

//=============================================================================
MoveSelector::MoveSelector(
    const Movement::MoveSetPtr &spMoveSet,
    const Game::BitBoardPtr &spBoard,
    const Game::color_type &engineColor
) :
    m_wpMoveSet(spMoveSet),
    m_wpBoard(spBoard),
    m_engineColor(engineColor),
    m_evaluator(engineColor)
{
}

//=============================================================================
Movement::Move MoveSelector::GetBestMove(const Game::value_type &maxDepth) const
{
    Game::BitBoardPtr spBoard = m_wpBoard.lock();

    Movement::ValidMoveSet vms(m_wpMoveSet, spBoard);
    Movement::MoveList moves = vms.GetMyValidMoves();

    Game::value_type bestValue = s_negInfinity;
    Movement::Move bestMove;

    for (auto it = moves.begin(); it != moves.end(); ++it)
    {
        Game::BitBoardPtr spResult = result(spBoard, *it);

        Game::value_type oldVal = bestValue;
        Game::value_type min = minValue(spResult, maxDepth, s_negInfinity, s_posInfinity);
        bestValue = std::max(bestValue, min);

        if (bestValue > oldVal)
        {
            bestMove = *it;
        }
    }

    return bestMove;
}

//=============================================================================
Game::value_type MoveSelector::maxValue(
    const Game::BitBoardPtr &spBoard,
    const Game::value_type &depth,
    Game::value_type alpha,
    Game::value_type beta
) const
{
    Movement::ValidMoveSet vms(m_wpMoveSet, spBoard);
    Game::value_type score = m_evaluator.Score(spBoard, vms);

    if (reachedEndState(depth, score))
    {
        return score;
    }

    Movement::MoveList moves = vms.GetMyValidMoves();
    Game::value_type v = s_negInfinity;

    for (auto it = moves.begin(); it != moves.end(); ++it)
    {
        Game::BitBoardPtr spResult = result(spBoard, *it);
        v = std::max(v, minValue(spResult, depth-1, alpha, beta));

        if (score >= beta)
        {
            return v;
        }

        alpha = std::max(alpha, v);
    }

    return v;
}

//=============================================================================
Game::value_type MoveSelector::minValue(
    const Game::BitBoardPtr &spBoard,
    const Game::value_type &depth,
    Game::value_type alpha,
    Game::value_type beta
) const
{
    Movement::ValidMoveSet vms(m_wpMoveSet, spBoard);
    Game::value_type score = m_evaluator.Score(spBoard, vms);

    if (reachedEndState(depth, score))
    {
        return score;
    }

    Movement::MoveList moves = vms.GetMyValidMoves();
    Game::value_type v = s_posInfinity;

    for (auto it = moves.begin(); it != moves.end(); ++it)
    {
        Game::BitBoardPtr spResult = result(spBoard, *it);
        v = std::min(v, maxValue(spResult, depth-1, alpha, beta));

        if (score <= alpha)
        {
            return v;
        }

        beta = std::min(beta, v);
    }

    return v;
}

//=============================================================================
Game::BitBoardPtr MoveSelector::result(
    const Game::BitBoardPtr &spBoard,
    Movement::Move move
) const
{
    Game::BitBoard copy(*spBoard);
    copy.MakeMove(move);

    return std::make_shared<Game::BitBoard>(copy);
}

//=============================================================================
bool MoveSelector::reachedEndState(
    const Game::value_type &depth,
    const Game::value_type &score
) const
{
    return ((depth <= 1) || (score == s_posInfinity) || (score == s_negInfinity) || (score == 0));
}

}
