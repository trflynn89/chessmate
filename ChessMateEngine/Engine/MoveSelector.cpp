/**
 * Implementation of the MoveSelector interface.
 *
 * @author Timothy Flynn(timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
#include <algorithm>

#include "MoveSelector.h"
#include <Movement/ValidMoveSet.h>
#include <Engine/Evaluator.h>

using std::max;
using std::min;

using ChessMate::Game::BitBoard;
using ChessMate::Engine::Evaluator;
using ChessMate::Movement::Move;
using ChessMate::Movement::ValidMoveSet;
using ChessMate::Movement::validMoveList_t;

namespace ChessMate { namespace Engine {

//=============================================================================
Move MoveSelector::GetBestMove(color_t engineColor, BitBoard *board, value_t maxDepth) const
{
	BitBoard b = *board;
	value_t val = -32767;

	// Get the list of possible moves
	ValidMoveSet vms = ValidMoveSet(b);
	validMoveList_t moves = vms.GetMyValidMoves();
	Move bestMove = moves[0];

	//DWORD start = GetTickCount();

	// Iterate through each possible move, one thread each
	for (value_t i=0; i<vms.GetNumMyValidMoves(); i++)
	{
		Move m = moves[i];

		value_t oldVal = val;
		value_t min = MinValue(engineColor, Result(b, m), maxDepth, -32767, 32767);
		val = max(val, min);

		// If the move is better, select it
		if (val > oldVal)
		{
			bestMove = m;
		}

		//std::cout << ".";
	}

	//DWORD end = GetTickCount();

	//std::cout << "\nTime: " << (end-start) << std::endl;

	return bestMove;
}

//=============================================================================
value_t MoveSelector::MaxValue(color_t engineColor, BitBoard board,
	value_t depth, value_t alpha, value_t beta) const
{
	ValidMoveSet vms = ValidMoveSet(board);
	value_t score = Evaluator(engineColor, board, vms).EvaluateBoardScore();
	
	// If in an end state, return the utility
	if (depth <= 1 || score == 32767 || score == -32767 || score == 0)
	{
		return score;
	}

	validMoveList_t moves = vms.GetMyValidMoves();
	value_t v = -32767;

	// For each possible action from the given board
	for (value_t i=0; i<vms.GetNumMyValidMoves(); i++)
	{
		Move m = moves[i];

		v = max(v, MinValue(engineColor, Result(board, m), depth-1, alpha, beta));

		// If the new value is better than beta, prune the branch
		if (score >= beta)
		{
			return v;
		}

		// Update alpha
		alpha = max(alpha, v);
	}

	return v;
}

//=============================================================================
value_t MoveSelector::MinValue(color_t engineColor, BitBoard board,
	value_t depth, value_t alpha, value_t beta) const
{
	ValidMoveSet vms = ValidMoveSet(board);
	value_t score = Evaluator(engineColor, board, vms).EvaluateBoardScore();
	
	// If in an end state, return the utility
	if (depth <= 1 || score == 32767 || score == -32767 || score == 0)
	{
		return score;
	}

	validMoveList_t moves = vms.GetMyValidMoves();
	value_t v = 32767;

	// For each possible action from the given board
	for (value_t i=0; i<vms.GetNumMyValidMoves(); i++)
	{
		Move m = moves[i];

		v = min(v, MaxValue(engineColor, Result(board, m), depth-1, alpha, beta));

		// If the new value is better than alpha, prune the branch
		if (score <= alpha)
		{
			return v;
		}

		// Update beta
		beta = min(beta, v);
	}

	return v;
}

//=============================================================================
BitBoard MoveSelector::Result(BitBoard board, Move move) const
{
	board.MakeMove(move);
	return board;
}

}}
