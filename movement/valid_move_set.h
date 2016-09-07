#pragma once

#include <game/bit_board.h>
#include <movement/move.h>
#include <movement/move_set.h>

namespace chessmate {

/**
 * This class uses the set of all moves calculated at the beginning of the game
 * to generate a single list of actually possible moves given a board config.
 * Unlike the MoveSet file, this should be called after every move.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version March 3, 2013
 */
class ValidMoveSet
{
public:
    /**
     * Default constructor.
     *
     * @param MoveSetPtr The list of possible moves.
     * @param BitBoardPtr The board to generate moves for.
     */
    ValidMoveSet(const MoveSetWPtr &, const BitBoardPtr &);

    /**
     * @return The list of the turn player's valid moves.
     */
    MoveList GetMyValidMoves() const;

    /**
     * @return The list of the opponent's valid moves.
     */
    MoveList GetOppValidMoves() const;

    /**
     * @return A given square's attack value.
     */
    value_type GetAttackValue(square_type, square_type) const;

    /**
     * @return A given square's defense value.
     */
    value_type GetDefendValue(square_type, square_type) const;

private:
    /**
     * Generate a list of moves all pieces can make.
     *
     * @param MoveSetPtr The list of possible moves.
     * @param BitBoardPtr The board to generate moves for.
     */
    void generateValidMoves(const MoveSetPtr &, const BitBoardPtr &);

    /**
     * Given a list of sliding moves, add the move to the list if it is valid.
     *
     * @param BitBoardPtr The board to generate moves for.
     * @param piece_type The sliding piece.
     * @param color_type The sliding piece's color.
     * @param value_type The sliding piece's value.
     * @param MoveList The list of moves to check.
     */
    void addSlidingMoveIfValid(
        const BitBoardPtr &,
        const piece_type &,
        const color_type &,
        const value_type &,
        MoveList);

    MoveList m_myValidMoves;
    MoveList m_oppValidMoves;

    value_type m_attackValue[BOARD_SIZE];
    value_type m_defendValue[BOARD_SIZE];
};

}
