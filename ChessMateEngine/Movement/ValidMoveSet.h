/**
 * This class uses the set of all moves calculated at the beginning of the game
 * to generate a single list of actually possible moves given a board config.
 * Unlike the MoveSet file, this should be called after every move.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
#ifndef _VALID_MOVE_SET_H_
#define _VALID_MOVE_SET_H_

#include <ChessMateEngine.h>
#include <Game/BitBoard.h>
#include <Movement/Move.h>
#include <Movement/MoveSet.h>

namespace ChessMate { namespace Movement {

class ValidMoveSet
{
public:
	/**
	 * Default constructor.
	 *
	 * @param The board to generate moves for.
	 */
	ValidMoveSet(const ChessMate::Game::BitBoard &);

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
	value_t GetNumMyValidMoves() const;

	/**
	 * @return The number of the opponent's valid moves.
	 */
	value_t GetNumOppValidMoves() const;

	/**
	 * @return A given square's attack value.
	 */
	value_t GetAttackValue(square_t, square_t) const;

	/**
	 * @return A given square's defense value.
	 */
	value_t GetDefendValue(square_t, square_t) const;

private:
	/**
	 * Generate a list of moves all pieces can make.
	 */ 
	void generateValidMoves();
	
	/**
	 * Given a list of sliding moves, add the move to the list if it is valid.
	 *
	 * @param The color of the player in turn.
	 * @param The sliding piece.
	 * @param The sliding piece's value.
	 * @param The list of moves to check.
	 * @param The size of the move list to check.
	 */ 
	void addSlidingMoveIfValid(color_t, piece_t, value_t, moveList_t, value_t);

	validMoveList_t m_myValidMoves;
	validMoveList_t m_oppValidMoves;

	value_t m_numMyValidMoves;
	value_t m_numOppValidMoves;

	value_t m_attackValue[BOARD_SIZE];
	value_t m_defendValue[BOARD_SIZE];

	ChessMate::Game::BitBoard m_board;
};

}}

#endif
