/**
 * Class to select a move for the engine to play. Implements a depth-limited
 * min-max algorithm with alpha-beta pruning.
 *
 * @author Timothy Flynn
 * @version March 3, 2013
 */
#ifndef _MOVE_SELECTOR_H_
#define _MOVE_SELECTOR_H_

#include <ChessMateEngine.h>
#include <Game/BitBoard.h>
#include <Movement/Move.h>

namespace ChessMate { namespace Engine {

class MoveSelector
{
public:
	/**
	 * Get the best move.
	 *
	 * @param The engine color.
	 * @param The board setup.
	 * @param The max depth to search.
	 * @return The best move.
	 */
	ChessMate::Movement::Move GetBestMove(color_t, ChessMate::Game::BitBoard *, value_t) const;

private:
	/**
	 * The algorithm to calculate the max value for the engine.
	 *
	 * @param The engine color.
	 * @param The board setup.
	 * @param The max depth to search.
	 * @param The alpha value.
	 * @param The beta value.
	 * @return The max utility value.
	 */
	value_t MaxValue(color_t, ChessMate::Game::BitBoard, value_t, value_t, value_t) const;

	/**
	 * The algorithm to calculate the min value for the human.
	 *
	 * @param The engine color.
	 * @param The board setup.
	 * @param The max depth to search.
	 * @param The alpha value.
	 * @param The beta value.
	 * @return The min utility value.
	 */
	value_t MinValue(color_t, ChessMate::Game::BitBoard, value_t, value_t, value_t) const;

	/**
	 * @return The result of making a move on a board.
	 *
	 * Note: only promotes to queens for now.
	 */
	ChessMate::Game::BitBoard Result(ChessMate::Game::BitBoard, ChessMate::Movement::Move) const;
};

}}

#endif
