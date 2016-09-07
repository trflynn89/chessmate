#pragma once

#include <engine/evaluator.h>
#include <game/bit_board.h>
#include <game/board_types.h>
#include <movement/move.h>
#include <movement/move_set.h>

namespace chessmate {

/**
 * Class to select a move for the engine to play. Implements a depth-limited
 * min-max algorithm with alpha-beta pruning.
 *
 * @author Timothy Flynn
 * @version March 3, 2013
 */
class MoveSelector
{
public:
    /**
     * Constructor.
     *
     * @param MoveSetPtr The list of possible moves.
     * @param BitBoardPtr Shared pointer to the game's board.
     * @param color_type The engine color.
     */
    MoveSelector(const MoveSetPtr &, const BitBoardPtr &, const color_type &);

    /**
     * Use min-max to determine the best move that can be made.
     *
     * @param value_type The max depth to search.
     *
     * @return The best move.
     */
    Move GetBestMove(const value_type &) const;

private:
    /**
     * The algorithm to calculate the max value for the engine.
     *
     * @param BitBoardPtr Shared pointer to the current depth's board.
     * @param value_type The max depth to search.
     * @param value_type The alpha value.
     * @param value_type The beta value.
     *
     * @return The max utility value.
     */
    value_type maxValue(
        const BitBoardPtr &,
        const value_type &,
        value_type,
        value_type
    ) const;

    /**
     * The algorithm to calculate the min value for the human.
     *
     * @param BitBoardPtr Shared pointer to the current depth's board.
     * @param value_type The max depth to search.
     * @param value_type The alpha value.
     * @param value_type The beta value.
     *
     * @return The min utility value.
     */
    value_type minValue(
        const BitBoardPtr &,
        const value_type &,
        value_type,
        value_type
    ) const;

    /**
     * Create a copy of the given board, and make the given move on that copy.
     * Note: only promotes to queens for now.
     *
     * @param BitBoardPtr Shared pointer to the current depth's board.
     * @param Move The move to make on the board copy.
     *
     * @return The created copy after the move was made.
     */
    BitBoardPtr result(const BitBoardPtr &, Move) const;

    /**
     * Decide if the selector should stop searching.
     *
     * @param value_type The current depth.
     * @param value_type The current score.
     *
     * @return True if the selector should stop searching.
     */
    bool reachedEndState(const value_type &, const value_type &) const;

    MoveSetWPtr m_wpMoveSet;
    BitBoardWPtr m_wpBoard;
    color_type m_engineColor;

    Evaluator m_evaluator;

};

}
