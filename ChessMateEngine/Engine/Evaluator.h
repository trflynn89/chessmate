/**
 * Class to evaluate a given board. Scoring works as follows:
 * - The more positive the score is, the better the board is for the engine.
 * - The more negative the score is, the better the board is for the human.
 * - A score of 0 is a completely neutral board.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_

#include <ChessMateEngine.h>
#include <Game/BitBoard.h>
#include <Movement/Move.h>
#include <Movement/ValidMoveSet.h>

namespace ChessMate { namespace Engine {

class Evaluator 
{
public:
	/**
	 * Constructor.
	 *
	 * @param The engine color.
	 * @param The board to evaluate.
	 * @param All valid moves for the board.
	 */
	Evaluator(color_t, ChessMate::Game::BitBoard, ChessMate::Movement::ValidMoveSet);

	/**
	 * Evaluate the score of the whole board.
	 *
	 * @return The board's score.
	 */
	int EvaluateBoardScore();

private:
	/**
	 * Evaluate the score of a single piece.
	 *
	 * @param The rank of the piece.
	 * @param The file of the piece.
	 * @return The piece's score.
	 */
	int evaluateSinglePiece(square_t, square_t) const;
	
	color_t m_engineColor;
	ChessMate::Game::BitBoard m_board;
	ChessMate::Movement::ValidMoveSet m_vms;
	ChessMate::Movement::validMoveList_t m_myMoveSet;
	ChessMate::Movement::validMoveList_t m_oppMoveSet;
};

}}

#endif
