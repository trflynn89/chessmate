/**
 * Implementation of the ValidMoveSet interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
#include "ValidMoveSet.h"

using ChessMate::Game::BitBoard;
using ChessMate::Movement::MoveSet;

namespace ChessMate { namespace Movement {

// Anonymous namespace to store private members
namespace
{
	// Attacking piece values - better to attack with pawn than queen
	const value_t m_pawnValue	= 6;
	const value_t m_knightValue	= 3;
	const value_t m_bishopValue	= 3;
	const value_t m_rookValue	= 2;
	const value_t m_queenValue	= 1;
	const value_t m_kingValue	= 1;
}

//=============================================================================
ValidMoveSet::ValidMoveSet(const BitBoard &board) : m_board(board)
{
	for (square_t i=0; i<BOARD_SIZE; i++)
	{
		m_attackValue[i] = 0;
		m_defendValue[i] = 0;
	}
	
	std::fill(m_myValidMoves.begin(), m_myValidMoves.end(), Move());
	std::fill(m_oppValidMoves.begin(), m_oppValidMoves.end(), Move());

	m_numMyValidMoves = 0;
	m_numOppValidMoves = 0;
	generateValidMoves();
}
	
//=============================================================================
void ValidMoveSet::generateValidMoves()
{
	moveList2d_t wPawn = MoveSet::GetWhitePawnMoves();
	moveList2d_t bPawn = MoveSet::GetBlackPawnMoves();
	moveList2d_t knight = MoveSet::GetKnightMoves();
	moveList2d_t king = MoveSet::GetKingMoves();
	color_t playerInTurn = m_board.GetPlayerInTurn();

	for (square_t i=0; i<BOARD_SIZE; i++)
	{
		square_t rank = GET_RANK(i);
		square_t file = GET_FILE(i);
		color_t c = (m_board.IsWhite(rank, file) ? color_W : color_B);

		/***** EMPTY *****/

		if (m_board.IsEmpty(rank, file))
		{
			continue;
		}

		/***** PAWN *****/

		else if (m_board.IsPawn(rank, file))
		{

			// WHITE pawn
			if (c == color_W)
			{
				moveList_t moves = wPawn[i];

				for (value_t j=0; j<MoveSet::GetNumWhitePawnMoves(i); j++) 
				{
					Move m = moves[j];

					// Straight
					if (m.GetStartFile() == m.GetEndFile())
					{
						// 2 move start
						if (m.GetEndRank() - m.GetStartRank() == 2)
						{
							if (m_board.IsEmpty(m.GetStartRank()+1, m.GetStartFile()))
							{
								if (m_board.IsEmpty(m.GetEndRank(), m.GetEndFile()))
								{
									m.SetMovingPiece(piece_P);

									if (playerInTurn == color_W)
									{
										m_myValidMoves[m_numMyValidMoves++] = m;
									}
									else
									{
										m_oppValidMoves[m_numOppValidMoves++] = m;
									}
								}
							}
						} 
					
						// Single move forward
						else if (m_board.IsEmpty(m.GetEndRank(), m.GetEndFile()))
						{
							m.SetMovingPiece(piece_P);

							if (playerInTurn == color_W)
							{
								m_myValidMoves[m_numMyValidMoves++] = m;
							}
							else
							{
								m_oppValidMoves[m_numOppValidMoves++] = m;
							}
						}
					} 
				
					// Attack diagonally
					else
					{
						if (playerInTurn == color_W)
						{
							m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())] 
								+= m_pawnValue;
						}
						else
						{
							m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
								+= m_pawnValue;
						}

						if (m_board.IsBlack(m.GetEndRank(), m.GetEndFile()))
						{
							m.SetCapture();
							m.SetMovingPiece(piece_P);

							if (playerInTurn == color_W)
							{
								m_myValidMoves[m_numMyValidMoves++] = m;
							}
							else
							{
								m_oppValidMoves[m_numOppValidMoves++] = m;
							}
						}

						// En passant
						else if (m_board.GetEnPassantColor() == color_B)
						{
							square_t pos = m_board.GetEnPassantPosition();
							square_t r = GET_RANK(pos);
							square_t f = GET_FILE(pos);

							if (r == m.GetEndRank() && f == m.GetEndFile())
							{
								m.SetCapture();
								m.SetEnPassant();
								m.SetMovingPiece(piece_P);

								if (playerInTurn == color_W)
								{
									m_myValidMoves[m_numMyValidMoves++] = m;
								}
								else
								{
									m_oppValidMoves[m_numOppValidMoves++] = m;
								}
							}
						}
					}
				}
			} 
			
			// BLACK pawn
			else
			{
				moveList_t moves = bPawn[i];

				for (value_t j=0; j<MoveSet::GetNumBlackPawnMoves(i); j++)
				{
					Move m = moves[j];

					// Straight
					if (m.GetStartFile() == m.GetEndFile())
					{
						// 2 move start
						if (m.GetStartRank() - m.GetEndRank() == 2)
						{
							if (m_board.IsEmpty(m.GetStartRank()-1, m.GetStartFile()))
							{
								if (m_board.IsEmpty(m.GetEndRank(), m.GetEndFile()))
								{
									m.SetMovingPiece(piece_P);

									if (playerInTurn == color_B)
									{
										m_myValidMoves[m_numMyValidMoves++] = m;
									}
									else
									{
										m_oppValidMoves[m_numOppValidMoves++] = m;
									}
								}
							}
						} 
					
						// Single move forward
						else if (m_board.IsEmpty(m.GetEndRank(), m.GetEndFile()))
						{
							m.SetMovingPiece(piece_P);

							if (playerInTurn == color_B)
							{
								m_myValidMoves[m_numMyValidMoves++] = m;
							}
							else
							{
								m_oppValidMoves[m_numOppValidMoves++] = m;
							}
						}
					} 
				
					// Attack diagonally
					else
					{
						if (playerInTurn == color_B)
						{
							m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
								+= m_pawnValue;
						}
						else
						{
							m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
								+= m_pawnValue;
						}

						if (m_board.IsWhite(m.GetEndRank(), m.GetEndFile()))
						{
							m.SetCapture();
							m.SetMovingPiece(piece_P);

							if (playerInTurn == color_B)
							{
								m_myValidMoves[m_numMyValidMoves++] = m;
							} 
							else
							{
								m_oppValidMoves[m_numOppValidMoves++] = m;
							}
						}

						// En passant
						else if (m_board.GetEnPassantColor() == color_W)
						{
							square_t pos = m_board.GetEnPassantPosition();
							square_t r = GET_RANK(pos);
							square_t f = GET_FILE(pos);

							if (r == m.GetEndRank() && f == m.GetEndFile())
							{
								m.SetCapture();
								m.SetEnPassant();
								m.SetMovingPiece(piece_P);

								if (playerInTurn == color_B)
								{
									m_myValidMoves[m_numMyValidMoves++] = m;
								}
								else
								{
									m_oppValidMoves[m_numOppValidMoves++] = m;
								}
							}
						}
					}
				}
			}
		}

		/***** KNIGHT *****/

		if (m_board.IsKnight(rank, file))
		{
			moveList_t moves = knight[i];

			if (playerInTurn == c)
			{
				for (value_t j=0; j<MoveSet::GetNumKnightMoves(i); j++)
				{
					Move m = moves[j];

					if (m_board.IsEmpty(m.GetEndRank(), m.GetEndFile()))
					{
						m.SetMovingPiece(piece_N);

						m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
							+= m_knightValue;
						m_myValidMoves[m_numMyValidMoves++] = m;
					}
					else
					{
						color_t c2 = (m_board.IsWhite(m.GetEndRank(), m.GetEndFile())
							? color_W : color_B);
						if (c != c2)
						{
							m.SetCapture();
							m.SetMovingPiece(piece_N);

							m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
								+= m_knightValue;
							m_myValidMoves[m_numMyValidMoves++] = m;
						}
					}
				}
			}
			else
			{
				for (value_t j=0; j<MoveSet::GetNumKnightMoves(i); j++)
				{
					Move m = moves[j];
					
					if (m_board.IsEmpty(m.GetEndRank(), m.GetEndFile()))
					{
						m.SetMovingPiece(piece_N);

						m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
							+= m_knightValue;
						m_oppValidMoves[m_numOppValidMoves++] = m;
					}
					else
					{
						color_t c2 = (m_board.IsWhite(m.GetEndRank(), m.GetEndFile())
							? color_W : color_B);
						if (c != c2)
						{
							m.SetCapture();
							m.SetMovingPiece(piece_N);

							m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
								+= m_knightValue;
							m_oppValidMoves[m_numOppValidMoves++] = m;
						}
					}
				}
			}
		}

		/***** BISHOP *****/

		if (m_board.IsBishop(rank, file))
		{
			addSlidingMoveIfValid(c, piece_B, m_bishopValue,
				MoveSet::GetBishopMovesNE()[i], MoveSet::GetNumBishopMovesNE(i));
			addSlidingMoveIfValid(c, piece_B, m_bishopValue,
				MoveSet::GetBishopMovesNW()[i], MoveSet::GetNumBishopMovesNW(i));
			addSlidingMoveIfValid(c, piece_B, m_bishopValue, 
				MoveSet::GetBishopMovesSE()[i], MoveSet::GetNumBishopMovesSE(i));
			addSlidingMoveIfValid(c, piece_B, m_bishopValue, 
				MoveSet::GetBishopMovesSW()[i], MoveSet::GetNumBishopMovesSW(i));
		}

		/***** ROOK *****/

		else if (m_board.IsRook(rank, file))
		{
			addSlidingMoveIfValid(c, piece_R, m_rookValue, 
				MoveSet::GetRookMovesN()[i], MoveSet::GetNumRookMovesN(i));
			addSlidingMoveIfValid(c, piece_R, m_rookValue, 
				MoveSet::GetRookMovesS()[i], MoveSet::GetNumRookMovesS(i));
			addSlidingMoveIfValid(c, piece_R, m_rookValue, 
				MoveSet::GetRookMovesE()[i], MoveSet::GetNumRookMovesE(i));
			addSlidingMoveIfValid(c, piece_R, m_rookValue, 
				MoveSet::GetRookMovesW()[i], MoveSet::GetNumRookMovesW(i));
		}
				
		/***** QUEEN *****/

		else if (m_board.IsQueen(rank, file))
		{
			addSlidingMoveIfValid(c, piece_Q, m_queenValue, 
				MoveSet::GetQueenMovesN()[i], MoveSet::GetNumQueenMovesN(i));
			addSlidingMoveIfValid(c, piece_Q, m_queenValue, 
				MoveSet::GetQueenMovesS()[i], MoveSet::GetNumQueenMovesS(i));
			addSlidingMoveIfValid(c, piece_Q, m_queenValue, 
				MoveSet::GetQueenMovesE()[i], MoveSet::GetNumQueenMovesE(i));
			addSlidingMoveIfValid(c, piece_Q, m_queenValue, 
				MoveSet::GetQueenMovesW()[i], MoveSet::GetNumQueenMovesW(i));
			addSlidingMoveIfValid(c, piece_Q, m_queenValue, 
				MoveSet::GetQueenMovesNE()[i], MoveSet::GetNumQueenMovesNE(i));
			addSlidingMoveIfValid(c, piece_Q, m_queenValue, 
				MoveSet::GetQueenMovesNW()[i], MoveSet::GetNumQueenMovesNW(i));
			addSlidingMoveIfValid(c, piece_Q, m_queenValue, 
				MoveSet::GetQueenMovesSE()[i], MoveSet::GetNumQueenMovesSE(i));
			addSlidingMoveIfValid(c, piece_Q, m_queenValue, 
				MoveSet::GetQueenMovesSW()[i], MoveSet::GetNumQueenMovesSW(i));
		}

		/***** KING *****/

		if (m_board.IsKing(rank, file))
		{
			color_t c = (m_board.IsWhite(rank, file) ? color_W : color_B);
			color_t oppC = (c == color_W ? color_B : color_W);

			if (c == m_board.GetPlayerInTurn())
			{
				moveList_t moves = king[i];
			
				for (value_t j=0; j<MoveSet::GetNumKingMoves(i); j++)
				{
					Move m = moves[j];

					bool movedKing = (c == color_W ?
						m_board.HasWhiteMovedKing() :
						m_board.HasBlackMovedKing());
					bool movedKingsideRook = (c == color_W ?
						m_board.HasWhiteMovedKingsideRook() :
						m_board.HasBlackMovedKingsideRook());
					bool movedQueensideRook = (c == color_W ?
						m_board.HasWhiteMovedQueensideRook() :
						m_board.HasBlackMovedQueensideRook());
					bool inCheck = (c == color_W ?
						m_board.IsWhiteInCheck() :
						m_board.IsBlackInCheck());

					square_t diff = m.GetEndFile() - m.GetStartFile();

					// If the destination is empty
					if (m_board.IsEmpty(m.GetEndRank(), m.GetEndFile()))
					{

						// If the desitination is empty and we're not castling,
						// definitely valid
						if (diff > -2 && diff < 2) {
							m.SetMovingPiece(piece_K);
							m_myValidMoves[m_numMyValidMoves++] = m;
						}

						// Verify castling move
						else if (!movedKing && !inCheck)
						{
							// Kingside castle
							if (diff == 2)
							{
 								if (!movedKingsideRook &&
									m_board.IsEmpty(m.GetStartRank(), m.GetStartFile()+1))
								{
									if (m_board.IsUnderAttack(m.GetStartRank(),
										m.GetStartFile()+1, oppC) == 0)
									{
										m.SetKingsideCastle();
										m.SetMovingPiece(piece_K);
										m_myValidMoves[m_numMyValidMoves++] = m;
									}
								}
							}

							// Queenside castle
							else if (diff == -2)
							{ 
								if (!movedQueensideRook &&
									m_board.IsEmpty(m.GetStartRank(), m.GetStartFile()-1))
								{
									if (m_board.IsUnderAttack(m.GetStartRank(),
										m.GetStartFile()-1, oppC) == 0)
									{
										m.SetQueensideCastle();
										m.SetMovingPiece(piece_K);
										m_myValidMoves[m_numMyValidMoves++] = m;
									}
								}
							} 
						}
					} 

					// Or if we are white and destination is black
					else if ((c == color_W
						&& m_board.IsBlack(m.GetEndRank(), m.GetEndFile())))
					{
						if (diff > -2 && diff < 2)
						{
							m.SetCapture();
							m.SetMovingPiece(piece_K);
							m_myValidMoves[m_numMyValidMoves++] = m;
						}
					} 
					
					// Or if we are black and destination is white
					else if ((c == color_B
						&& m_board.IsWhite(m.GetEndRank(), m.GetEndFile())))
					{
						if (diff > -2 && diff < 2)
						{
							m.SetCapture();
							m.SetMovingPiece(piece_K);
							m_myValidMoves[m_numMyValidMoves++] = m;
						}
					}
				}
			}
		}
	}

	// Remove moves that leave player in check
	validMoveList_t newValidMoves;
	value_t sz = m_numMyValidMoves;
	m_numMyValidMoves = 0;

	for (value_t i=0; i<sz; i++)
	{
		BitBoard copy = BitBoard(m_board);
		Move m = m_myValidMoves[i];
		copy.MakeMove(m);

		// After making move, current player is opponent
		if (copy.GetPlayerInTurn() == color_W && copy.IsBlackInCheck())
		{
			// do nothing
		}
		else if (copy.GetPlayerInTurn() == color_B && copy.IsWhiteInCheck())
		{
			// do nothing
		}
		else
		{
			newValidMoves[m_numMyValidMoves++] = m;
		}
	}

	m_myValidMoves = newValidMoves;
}

//=============================================================================
validMoveList_t ValidMoveSet::GetMyValidMoves() const
{
	return m_myValidMoves;
}

//=============================================================================
validMoveList_t ValidMoveSet::GetOppValidMoves() const
{
	return m_oppValidMoves;
}

//=============================================================================
value_t ValidMoveSet::GetNumMyValidMoves() const
{
	return m_numMyValidMoves;
}

//=============================================================================
value_t ValidMoveSet::GetNumOppValidMoves() const
{
	return m_numOppValidMoves;
}

//=============================================================================
value_t ValidMoveSet::GetAttackValue(square_t rank, square_t file) const
{
	return m_attackValue[GET_SQUARE(rank, file)];
}

//=============================================================================
value_t ValidMoveSet::GetDefendValue(square_t rank, square_t file) const
{
	return m_defendValue[GET_SQUARE(rank, file)];
}

//=============================================================================
void ValidMoveSet::addSlidingMoveIfValid(color_t color, piece_t piece,
	value_t value, moveList_t moves, value_t size)
{
	color_t playerInTurn = m_board.GetPlayerInTurn();

	if (color == playerInTurn)
	{
		for (square_t i=0; i<size; i++)
		{
			Move m = moves[i];

			// If the square is empty, it's valid
			if (m_board.IsEmpty(m.GetEndRank(), m.GetEndFile()))
			{
				m.SetMovingPiece(piece);

				m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())] += value;
				m_myValidMoves[m_numMyValidMoves++] = m;
			} 
		
			// If the square is occupied by the opposing color, it's valid
			else
			{
				color_t c = (m_board.IsWhite(m.GetEndRank(), m.GetEndFile()) ? color_W : color_B);

				if (playerInTurn != c)
				{
					m.SetCapture();
					m.SetMovingPiece(piece);

					m_myValidMoves[m_numMyValidMoves++] = m;
				}
				else
				{
					m.SetCapture();
					m.SetMovingPiece(piece);

					m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())] += value;
				}
				
				break;
			}
		}
	}
	else
	{
		for (square_t i=0; i<size; i++)
		{
			Move m = moves[i];

			if (m_board.IsEmpty(m.GetEndRank(), m.GetEndFile()))
			{
				m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())] += value;
				m_oppValidMoves[m_numOppValidMoves++] = m;
			}
			else
			{
				color_t c = (m_board.IsWhite(m.GetEndRank(), m.GetEndFile()) ? color_W : color_B);

				if (playerInTurn == c)
				{
					m_oppValidMoves[m_numOppValidMoves++] = m;
				}
				else
				{
					m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())] += value;
				}

				break;
			}
		}
	}
}

}}
