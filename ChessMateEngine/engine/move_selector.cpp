#include "move_selector.h"

#include "movement/valid_move_set.h"

#include <fly/fly.hpp>

#include <algorithm>

namespace chessmate {

namespace {
    static const int s_posInfinity = 32767;
    static const int s_negInfinity = -32767;
} // namespace

//==================================================================================================
MoveSelector::MoveSelector(
    const std::shared_ptr<MoveSet> &spMoveSet,
    const std::shared_ptr<BitBoard> &spBoard,
    const color_type &engineColor) :
    m_wpMoveSet(spMoveSet),
    m_wpBoard(spBoard),
    m_engineColor(engineColor),
    m_evaluator(engineColor)
{
    FLY_UNUSED(m_engineColor);
}

//==================================================================================================
Move MoveSelector::GetBestMove(const value_type &maxDepth) const
{
    std::shared_ptr<BitBoard> spBoard = m_wpBoard.lock();

    ValidMoveSet vms(m_wpMoveSet, spBoard);
    MoveList moves = vms.GetMyValidMoves();

    value_type bestValue = s_negInfinity;
    Move bestMove;

    for (auto it = moves.begin(); it != moves.end(); ++it)
    {
        std::shared_ptr<BitBoard> spResult = result(spBoard, *it);

        value_type oldVal = bestValue;
        value_type min = minValue(spResult, maxDepth, s_negInfinity, s_posInfinity);
        bestValue = std::max(bestValue, min);

        if (bestValue > oldVal)
        {
            bestMove = *it;
        }
    }

    return bestMove;
}

//==================================================================================================
value_type MoveSelector::maxValue(
    const std::shared_ptr<BitBoard> &spBoard,
    const value_type &depth,
    value_type alpha,
    value_type beta) const
{
    ValidMoveSet vms(m_wpMoveSet, spBoard);
    value_type score = m_evaluator.Score(spBoard, vms);

    if (reachedEndState(depth, score))
    {
        return score;
    }

    MoveList moves = vms.GetMyValidMoves();
    value_type v = s_negInfinity;

    for (auto it = moves.begin(); it != moves.end(); ++it)
    {
        std::shared_ptr<BitBoard> spResult = result(spBoard, *it);
        v = std::max(v, minValue(spResult, depth - 1, alpha, beta));

        if (score >= beta)
        {
            return v;
        }

        alpha = std::max(alpha, v);
    }

    return v;
}

//==================================================================================================
value_type MoveSelector::minValue(
    const std::shared_ptr<BitBoard> &spBoard,
    const value_type &depth,
    value_type alpha,
    value_type beta) const
{
    ValidMoveSet vms(m_wpMoveSet, spBoard);
    value_type score = m_evaluator.Score(spBoard, vms);

    if (reachedEndState(depth, score))
    {
        return score;
    }

    MoveList moves = vms.GetMyValidMoves();
    value_type v = s_posInfinity;

    for (auto it = moves.begin(); it != moves.end(); ++it)
    {
        std::shared_ptr<BitBoard> spResult = result(spBoard, *it);
        v = std::min(v, maxValue(spResult, depth - 1, alpha, beta));

        if (score <= alpha)
        {
            return v;
        }

        beta = std::min(beta, v);
    }

    return v;
}

//==================================================================================================
std::shared_ptr<BitBoard>
MoveSelector::result(const std::shared_ptr<BitBoard> &spBoard, Move move) const
{
    BitBoard copy(*spBoard);
    copy.MakeMove(move);

    return std::make_shared<BitBoard>(copy);
}

//==================================================================================================
bool MoveSelector::reachedEndState(const value_type &depth, const value_type &score) const
{
    return ((depth <= 1) || (score == s_posInfinity) || (score == s_negInfinity) || (score == 0));
}

} // namespace chessmate
