/**
 * Implementation of the Evaluator interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
#include "Evaluator.h"

using ChessMate::Game::BitBoard;
using ChessMate::Movement::Move;
using ChessMate::Movement::ValidMoveSet;
using ChessMate::Movement::validMoveList_t;

namespace ChessMate { namespace Engine {

// Anonymous namespace to store private members
namespace 
{
	// Piece values
	const value_t m_pawnValue	= 100;
	const value_t m_knightValue	= 320;
	const value_t m_bishopValue	= 325;
	const value_t m_rookValue	= 500;
	const value_t m_queenValue	= 975;
	const value_t m_kingValue	= 32767;

	// Array to store the "meaning" of a pawn in a file
	// i.e. account for isolated and doubled pawns
	value_t m_whitePawnFileValue[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	value_t m_blackPawnFileValue[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	// Count the number of bishops
	value_t m_whiteBishopCount = 0;
	value_t m_blackBishopCount = 0;

	// Tables go from 0th index = A1, to 63rd index = H8

	const value_t m_pawnTable[] =
	{
		0,  0,  0,  0,  0,  0,  0,  0,
		5, 10, 10,-25,-25, 10, 10,  5,
		5, -5,-10,  0,  0,-10, -5,  5,
		0,  0,  0, 25, 25,  0,  0,  0,
		5,  5, 10, 27, 27, 10,  5,  5,
		10, 10, 20, 30, 30, 20, 10, 10,
		50, 50, 50, 50, 50, 50, 50, 50,
		0,  0,  0,  0,  0,  0,  0,  0
	};

	const value_t m_knightTable[] =
	{
		-50,-40,-20,-30,-30,-20,-40,-50,
		-40,-20,  0,  5,  5,  0,-20,-40,
		-30,  5, 10, 15, 15, 10,  5,-30,
		-30,  0, 15, 20, 20, 15,  0,-30,
		-30,  5, 15, 20, 20, 15,  5,-30,
		-30,  0, 10, 15, 15, 10,  0,-30,
		-40,-20,  0,  0,  0,  0,-20,-40,
		-50,-40,-30,-30,-30,-30,-40,-50
	};

	const value_t m_bishopTable[] =
	{
		-20,-10,-40,-10,-10,-40,-10,-20,
		-10,  5,  0,  0,  0,  0,  5,-10,
		-10, 10, 10, 10, 10, 10, 10,-10,
		-10,  0, 10, 10, 10, 10,  0,-10,
		-10,  5,  5, 10, 10,  5,  5,-10,
		-10,  0,  5, 10, 10,  5,  0,-10,
		-10,  0,  0,  0,  0,  0,  0,-10,
		-20,-10,-10,-10,-10,-10,-10,-20
	};

	const value_t m_kingTable[] =
	{
		20, 30, 10,  0,  0, 10, 30, 20,
		20, 20,  0,  0,  0,  0, 20, 20,
		-10,-20,-20,-20,-20,-20,-20,-10, 
		-20,-30,-30,-40,-40,-30,-30,-20,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30
	};

	const value_t m_kingTableEndGame[] =
	{
		-50,-30,-30,-30,-30,-30,-30,-50,
		-30,-30,  0,  0,  0,  0,-30,-30,
		-30,-10, 20, 30, 30, 20,-10,-30,
		-30,-10, 30, 40, 40, 30,-10,-30,
		-30,-10, 30, 40, 40, 30,-10,-30,
		-30,-10, 20, 30, 30, 20,-10,-30,
		-30,-20,-10,  0,  0,-10,-20,-30,
		-50,-40,-30,-20,-20,-30,-40,-50
	};
}

//=============================================================================
Evaluator::Evaluator(color_t engineColor, BitBoard board, ValidMoveSet vms)
: 
	m_engineColor(engineColor), m_board(board), m_vms(vms),
	m_myMoveSet(vms.GetMyValidMoves()), m_oppMoveSet(vms.GetOppValidMoves())
{ 
}

//=============================================================================
int Evaluator::EvaluateBoardScore() 
{
	value_t remainingPieces = 0;
	int score = 0;

	// Reinitialize values
	for (square_t i=file_A; i<=file_H; i++)
	{
		m_whitePawnFileValue[i] = 0;
		m_blackPawnFileValue[i] = 0;
	}
	m_whiteBishopCount = 0;
	m_blackBishopCount = 0;

	// Check for game over
	if (m_vms.GetNumMyValidMoves() == 0)
	{
		if (m_board.IsWhiteInCheck())
		{
			if (m_engineColor == color_W)
			{
				return (-m_kingValue);
			}
			else
			{
				return m_kingValue;
			}
		}
		else if (m_board.IsBlackInCheck())
		{
			if (m_engineColor == color_W)
			{
				return m_kingValue;
			}
			else
			{
				return (-m_kingValue);
			}
		} 
		else
		{
			return 0;
		}
	}
	else if (m_board.IsStalemateViaFiftyMoves())
	{
		return 0;
	}
	else if (m_board.IsStalemateViaThreeMoveRep())
	{
		return 0;
	}

	// Check for checks
	if (m_board.IsWhiteInCheck())
	{
		score -= 75;
		if (m_board.IsEndGame())
		{
			score -= 20;
		}
	}
	else if (m_board.IsBlackInCheck())
	{
		score += 75;
		if (m_board.IsEndGame())
		{
			score += 20;
		}
	}

	// Add for tempo
	if (m_board.GetPlayerInTurn() == color_W)
	{
		score += 10;
	}
	else
	{
		score -= 10;
	}

	// Try to prevent opponent from castling
	if (m_board.HasWhiteCastled())
	{
		score += 40;
	} 
	if (m_board.HasBlackCastled())
	{
		score -= 40;
	}

	// Loop through all pieces
	for (square_t s=0; s<BOARD_SIZE; s++)
	{
		square_t rank = GET_RANK(s);
		square_t file = GET_FILE(s);

		if (!m_board.IsEmpty(rank, file))
		{
			remainingPieces++;

			// Evaluate the piece
			if (m_board.IsWhite(rank, file))
			{
				score += evaluateSinglePiece(rank, file);
			}
			else
			{
				score -= evaluateSinglePiece(rank, file);
			}
		}
	}

	// Check for end game phase
	if (remainingPieces < 10)
	{
		m_board.SetEndGame(true);
	}

	// Check for white isolated pawns
	for (square_t i=file_A; i<=file_H; i++)
	{
		if (m_whitePawnFileValue[i] > 0)
		{
			if ((i>file_A && m_whitePawnFileValue[i-1] == 0) 
				|| (i<file_H && m_whitePawnFileValue[i+1] == 0))
			{
				score -= 15;
			}
		}
	}

	// Check for black isolated pawns
	for (square_t i=file_A; i<=file_H; i++)
	{
		if (m_blackPawnFileValue[i] > 0)
		{
			if ((i>file_A && m_blackPawnFileValue[i-1] == 0) 
				|| (i<file_H && m_blackPawnFileValue[i+1] == 0))
			{
				score += 15;
			}
		}
	}

	// Check for white passed pawns
	for (square_t i=file_A; i<=file_H; i++)
	{
		if (m_whitePawnFileValue[i] > 0 && m_blackPawnFileValue[i] == 0)
		{
			score += m_whitePawnFileValue[i];
		}
	}

	// Check for black passed pawns
	for (square_t i=file_A; i<=file_H; i++)
	{
		if (m_blackPawnFileValue[i] > 0 && m_whitePawnFileValue[i] == 0)
		{
			score += m_blackPawnFileValue[i];
		}
	}

	if (m_engineColor == color_W)
	{
		return score;
	}
	else
	{
		return -score;
	}
}

//=============================================================================
int Evaluator::evaluateSinglePiece(square_t rank, square_t file) const
{
	square_t loc = GET_SQUARE(rank, file);
	color_t pieceColor = (m_board.IsWhite(rank, file) ? color_W : color_B);
	int score = 0;

	if (pieceColor == color_B)
	{
		loc = BOARD_SIZE - loc - 1;
	}

	// Account for the attacked/defended value of the piece
	value_t defendedValue = 0;
	value_t attackedValue = 0;
	bool useMyMoves = true;

	// Get correct attack/defense values
	if (pieceColor == m_board.GetPlayerInTurn())
	{
		defendedValue = m_vms.GetDefendValue(rank, file);
		attackedValue = m_vms.GetAttackValue(rank, file);
	}
	else
	{
		defendedValue = m_vms.GetAttackValue(rank, file);
		attackedValue = m_vms.GetDefendValue(rank, file);
		useMyMoves = false;
	}

	score += defendedValue;
	score -= attackedValue;

	// Penalize further for situations where we will lose the piece
	if (defendedValue < attackedValue)
	{
		score -= (attackedValue - defendedValue) * 10;
	}

	// Add points for mobility
	if (useMyMoves)
	{
		for (value_t i=0; i<m_vms.GetNumMyValidMoves(); i++)
		{
			Move m = m_myMoveSet[i];
			if (m.GetStartRank() == rank && m.GetStartFile() == file)
			{
				score++;
			}
		}
	}
	else
	{
		for (value_t i=0; i<m_vms.GetNumOppValidMoves(); i++)
		{
			Move m = m_oppMoveSet[i];
			if (m.GetStartRank() == rank && m.GetStartFile() == file)
			{
				score++;
			}
		}
	}

	// Evaluate depending on piece

	// Pawn
	if (m_board.IsPawn(rank, file))
	{
		score += m_pawnValue;
		score += m_pawnTable[loc];

		// Rook-file pawns worth less - can only attack in one direction
		if (file==file_A || file == file_H)
		{
			score -= 15;
		}

		// White pawn
		if (pieceColor == color_W)
		{
			// If the value isn't 0, we have a doubled pawn
			if (m_whitePawnFileValue[file] > 0)
			{
				score -= 16;
			}

			// Good to advance pawn to second rank
			if (rank==rank_2)
			{
				if (attackedValue == 0)
				{
					m_whitePawnFileValue[file] += 200;

					if (defendedValue > 0)
					{
						m_whitePawnFileValue[file] += 50;
					}
				}
			} 
			// Good to advance to third rank
			else if (rank==rank_3)
			{
				if (attackedValue == 0)
				{
					m_whitePawnFileValue[file] += 100;

					if (defendedValue > 0)
					{
						m_whitePawnFileValue[file] += 25;
					}
				}
			}

			// Mark that there is a pawn in this file
			m_whitePawnFileValue[file] += 10;
		}

		// Black pawn
		else if (pieceColor == color_B)
		{
			// If the value isn't 0, we have a doubled pawn
			if (m_blackPawnFileValue[file] > 0)
			{
				score -= 16;
			}

			// Good to advance pawn to seventh rank
			if (rank==rank_7)
			{
				if (attackedValue == 0)
				{
					m_blackPawnFileValue[file] += 200;

					if (defendedValue > 0)
					{
						m_blackPawnFileValue[file] += 50;
					}
				}
			} 
			// Good to advance to sixth rank
			else if (rank==rank_6)
			{
				if (attackedValue == 0)
				{
					m_blackPawnFileValue[file] += 100;

					if (defendedValue > 0)
					{
						m_blackPawnFileValue[file] += 25;
					}
				}
			}

			// Mark that there is a pawn in this file
			m_blackPawnFileValue[file] += 10;
		}
	} 
	
	// Knight
	else if (m_board.IsKnight(rank, file))
	{
		score += m_knightValue;
		score += m_knightTable[loc];

		// Knights are worth less in the end game phase
		if (m_board.IsEndGame())
		{
			score -= 10;
		}
	} 
	
	// Bishop
	else if (m_board.IsBishop(rank, file))
	{
		score += m_bishopValue;
		score += m_bishopTable[loc];

		// Bishops are worth more in the end game phase
		if (m_board.IsEndGame())
		{
			score += 10;
		}

		// Bishops are better if we have more than one
		if (pieceColor == color_W)
		{
			m_whiteBishopCount++;
			if (m_whiteBishopCount >= 2)
			{
				score += 10;
			}
		}
		else if (pieceColor == color_B)
		{
			m_blackBishopCount++;
			if (m_blackBishopCount >= 2)
			{
				score += 10;
			}
		}
	} 
	
	// Rook
	else if (m_board.IsRook(rank, file))
	{
		score += m_rookValue;

		// Encourage rooks not to move until we have castled
		if (pieceColor == color_W)
		{
			if (!m_board.HasWhiteCastled())
			{
				if (rank != rank_1)
				{
					score -= 10;
				}
				else if (file != file_A || file != file_H)
				{
					score -= 10;
				}
			}
		}
		else if (pieceColor == color_B)
		{
			if (!m_board.HasBlackCastled())
			{
				if (rank != rank_8)
				{
					score -= 10;
				}
				else if (file != file_A || file != file_H)
				{
					score -= 10;
				}
			}
		}
	} 
	
	// Queen
	else if (m_board.IsQueen(rank, file))
	{
		score += m_queenValue;

		// Discourage queen from moving too early
		if (pieceColor == color_W)
		{
			if (m_board.HasWhiteMovedQueen() && !m_board.IsEndGame())
			{
				score -= 10;
			}
		} 
		else if (pieceColor == color_B)
		{
			if (m_board.HasBlackMovedQueen() && !m_board.IsEndGame())
			{
				score -= 10;
			}
		}
	} 
	
	// King
	else if (m_board.IsKing(rank, file))
	{
		score += m_kingValue;

		// Keep king mobile
		value_t numberOfKingMoves = 0;
		for (value_t i=0; i<m_vms.GetNumMyValidMoves(); i++)
		{
			Move m = m_myMoveSet[i];
			if (m.GetStartRank() == rank && m.GetStartFile() == file)
			{
				numberOfKingMoves++;
			}
		}
		if (numberOfKingMoves < 2)
		{
			score -= 5;
		}

		if (m_board.IsEndGame())
		{
			score += m_kingTableEndGame[loc];
		} 
		else 
		{
			score += m_kingTable[loc];

			// Encourage castling
			if (pieceColor == color_W) 
			{
				if (m_board.HasWhiteMovedKing() && 
					!(m_board.HasWhiteMovedKingsideRook() || 
						m_board.HasWhiteMovedQueensideRook())) 
				{
					score -= 30;
				}
			} 
			else if (pieceColor == color_B) 
			{
				if (m_board.HasBlackMovedKing() && 
					!(m_board.HasBlackMovedKingsideRook() || 
						m_board.HasBlackMovedQueensideRook()))
				{
					score -= 30;
				}
			}
		}
	}

	return score;
}

}}
