#pragma once

#include <Game/BitBoard.h>
#include <Movement/Move.h>
#include <Movement/MoveSet.h>

namespace Movement {

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
    ValidMoveSet(const MoveSetWPtr &, const Game::BitBoardPtr &);

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
    Game::value_type GetAttackValue(Game::square_type, Game::square_type) const;

    /**
     * @return A given square's defense value.
     */
    Game::value_type GetDefendValue(Game::square_type, Game::square_type) const;

private:
    /**
     * Generate a list of moves all pieces can make.
     *
     * @param MoveSetPtr The list of possible moves.
     * @param BitBoardPtr The board to generate moves for.
     */
    void generateValidMoves(const MoveSetPtr &, const Game::BitBoardPtr &);

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
        const Game::BitBoardPtr &,
        const Game::piece_type &,
        const Game::color_type &,
        const Game::value_type &,
        MoveList);

    MoveList m_myValidMoves;
    MoveList m_oppValidMoves;

    Game::value_type m_attackValue[Game::BOARD_SIZE];
    Game::value_type m_defendValue[Game::BOARD_SIZE];
};

}
