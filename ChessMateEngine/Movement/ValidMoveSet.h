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
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
class ValidMoveSet
{
public:
    /**
     * Default constructor.
     *
     * @param BitBoardPtr The board to generate moves for.
     */
    ValidMoveSet(const Game::BitBoardPtr &);

    /**
     * @return The list of the turn player's valid moves.
     */
    validMoveList_t GetMyValidMoves() const;

    /**
     * @return The list of the opponent's valid moves.
     */
    validMoveList_t GetOppValidMoves() const;

    /**
     * @return The number of the turn player's valid moves.
     */
    Game::value_type GetNumMyValidMoves() const;

    /**
     * @return The number of the opponent's valid moves.
     */
    Game::value_type GetNumOppValidMoves() const;

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
     * @param BitBoardPtr The board to generate moves for.
     */
    void generateValidMoves(const Game::BitBoardPtr &);

    /**
     * Given a list of sliding moves, add the move to the list if it is valid.
     *
     * @param BitBoardPtr The board to generate moves for.
     * @param piece_type The sliding piece.
     * @param color_type The sliding piece's color.
     * @param value_type The sliding piece's value.
     * @param moveList_t The list of moves to check.
     * @param value_type The size of the move list to check.
     */
    void addSlidingMoveIfValid(
        const Game::BitBoardPtr &,
        Game::piece_type,
        Game::color_type,
        Game::value_type,
        moveList_t,
        Game::value_type);

    validMoveList_t m_myValidMoves;
    validMoveList_t m_oppValidMoves;

    Game::value_type m_numMyValidMoves;
    Game::value_type m_numOppValidMoves;

    Game::value_type m_attackValue[Game::BOARD_SIZE];
    Game::value_type m_defendValue[Game::BOARD_SIZE];
};

}
