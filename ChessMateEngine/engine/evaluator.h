#pragma once

#include "game/bit_board.h"
#include "game/board_types.h"
#include "movement/move.h"
#include "movement/valid_move_set.h"

namespace chessmate {

/**
 * Class to evaluate a given board. Scoring works as follows:
 * - The more positive the score is, the better the board is for the engine.
 * - The more negative the score is, the better the board is for the human.
 * - A score of 0 is a draw.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version March 3, 2013
 */
class Evaluator
{
public:
    /**
     * Constructor.
     *
     * @param color_type The engine color.
     */
    Evaluator(const color_type &);

    /**
     * Evaluate the score of the whole board.
     *
     * @param std::shared_ptr<BitBoard> The board to evaluate.
     * @param ValidMoveSet All valid moves for the board.
     *
     * @return The board's score.
     */
    int Score(const std::shared_ptr<BitBoard> &, const ValidMoveSet &) const;

private:
    /**
     * Evaluate the score of a single piece on a board.
     *
     * @param std::shared_ptr<BitBoard> The board to evaluate.
     * @param ValidMoveSet All valid moves for the board.
     * @param square_type The rank of the piece.
     * @param square_type The file of the piece.
     *
     * @return The piece's score.
     */
    int evaluateSinglePiece(
        const std::shared_ptr<BitBoard> &,
        const ValidMoveSet &,
        const square_type &,
        const square_type &) const;

    color_type m_engineColor;
};

} // namespace chessmate
