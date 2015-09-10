#pragma once

#include <Game/BitBoard.h>
#include <Game/BoardTypes.h>
#include <Movement/Move.h>
#include <Movement/ValidMoveSet.h>

namespace Engine {

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
    Evaluator(const Game::color_type &);

    /**
     * Evaluate the score of the whole board.
     *
     * @param BitBoardPtr The board to evaluate.
     * @param ValidMoveSet All valid moves for the board.
     *
     * @return The board's score.
     */
    int Score(const Game::BitBoardPtr &, const Movement::ValidMoveSet &) const;

private:
    /**
     * Evaluate the score of a single piece on a board.
     *
     * @param BitBoardPtr The board to evaluate.
     * @param ValidMoveSet All valid moves for the board.
     * @param square_type The rank of the piece.
     * @param square_type The file of the piece.
     *
     * @return The piece's score.
     */
    int evaluateSinglePiece(
        const Game::BitBoardPtr &,
        const Movement::ValidMoveSet &,
        const Game::square_type &,
        const Game::square_type &
    ) const;

    Game::color_type m_engineColor;
};

}
