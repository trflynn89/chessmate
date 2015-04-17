/**
 * Implementation of the BitBoard interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
#include "BitBoard.h"

using std::ostream;

using ChessMate::Movement::Move;

namespace ChessMate { namespace Game {

//=============================================================================
BitBoard::BitBoard()
{
	m_pawn		= 0x00FF00000000FF00;
	m_knight	= 0x4200000000000042;
	m_bishop	= 0x2400000000000024;
	m_rook		= 0x8100000000000081;
	m_queen		= 0x0800000000000008;
	m_king		= 0x1000000000000010;
	m_white		= 0x000000000000FFFF;
	m_black		= 0xFFFF000000000000;

	m_boardScore				= 0;
	m_playerInTurn				= color_W;
	m_whiteInCheck				= false;
	m_blackInCheck				= false;
	m_endGame					= false;
	m_whiteKingLocation			= GET_SQUARE(rank_1, file_E);
	m_blackKingLocation			= GET_SQUARE(rank_8, file_E);
	m_whiteMovedKing			= false;
	m_whiteMovedQueen			= false;
	m_whiteMovedKingsideRook	= false;
	m_whiteMovedQueensideRook	= false;
	m_blackMovedKing			= false;
	m_blackMovedQueen			= false;
	m_blackMovedKingsideRook	= false;
	m_blackMovedQueensideRook	= false;
	m_whiteCastled				= false;
	m_blackCastled				= false;
	m_fiftyMoveCount			= 0;
	m_repeatedMoveCount			= 0;
	m_enPassantColor			= color_N;
	m_enPassantPosition			= -1;
}

//=============================================================================
BitBoard::BitBoard(const BitBoard &board)
{
	m_pawn		= board.m_pawn;
	m_knight	= board.m_knight;
	m_bishop	= board.m_bishop;
	m_rook		= board.m_rook;
	m_queen		= board.m_queen;
	m_king		= board.m_king;
	m_white		= board.m_white;
	m_black		= board.m_black;

	m_attackedByWhite			= board.m_attackedByWhite;
	m_attackedByBlack			= board.m_attackedByBlack;
	m_boardScore				= board.m_boardScore;
	m_playerInTurn				= board.m_playerInTurn;
	m_lastMove					= board.m_lastMove;
	m_whiteInCheck				= board.m_whiteInCheck;
	m_blackInCheck				= board.m_blackInCheck;
	m_endGame					= board.m_endGame;
	m_whiteKingLocation			= board.m_whiteKingLocation;
	m_blackKingLocation			= board.m_blackKingLocation;
	m_whiteMovedKing			= board.m_whiteMovedKing;
	m_whiteMovedQueen			= board.m_whiteMovedQueen;
	m_whiteMovedKingsideRook	= board.m_whiteMovedKingsideRook;
	m_whiteMovedQueensideRook	= board.m_whiteMovedQueensideRook;
	m_blackMovedKing			= board.m_blackMovedKing;
	m_blackMovedQueen			= board.m_blackMovedQueen;
	m_blackMovedKingsideRook	= board.m_blackMovedKingsideRook;
	m_blackMovedQueensideRook	= board.m_blackMovedQueensideRook;
	m_whiteCastled				= board.m_whiteCastled;
	m_blackCastled				= board.m_blackCastled;
	m_fiftyMoveCount			= board.m_fiftyMoveCount;
	m_repeatedMoveCount			= board.m_repeatedMoveCount;
	m_enPassantColor			= board.m_enPassantColor;
	m_enPassantPosition			= board.m_enPassantPosition;
}

//=============================================================================
void BitBoard::MakeMove(Move &move)
{
	square_t sRank = move.GetStartRank();
	square_t sFile = move.GetStartFile();
	square_t eRank = move.GetEndRank();
	square_t eFile = move.GetEndFile();

	// Bits to set and erase
	bitboard_t eraseBit = ~(I64(1) << GET_SQUARE(sRank, sFile));
	bitboard_t setBit = (I64(1) << GET_SQUARE(eRank, eFile));

	// Increment the fifty move counter
	m_fiftyMoveCount++;

	// Piece color
	color_t color = (IsWhite(sRank, sFile) ? color_W : color_B);

	// Check for en passant
	if (move.IsEnPassant())
	{
		m_fiftyMoveCount = 0;

		square_t n = GET_SQUARE(sRank, eFile);
		bitboard_t epBit = (I64(1) << n);

		m_pawn &= ~epBit;
		m_white &= ~epBit; // Doesn't matter white or black
		m_black &= ~epBit;
	}

	// Check if we captured a piece
	else if (move.IsCapture())
	{
		m_fiftyMoveCount = 0;

		// Erase end piece
		if (IsPawn(eRank, eFile))
		{
			m_pawn &= ~setBit;
		}
		else if (IsKnight(eRank, eFile))
		{
			m_knight &= ~setBit;
		}
		else if (IsBishop(eRank, eFile))
		{
			m_bishop &= ~setBit;
		}
		else if (IsRook(eRank, eFile))
		{
			m_rook &= ~setBit;
		}
		else if (IsQueen(eRank, eFile))
		{
			m_queen &= ~setBit;
		}
		// King is never captured, so no need to check
	}

	// Reset en passant flags
	if (m_enPassantPosition > 0)
	{
		m_enPassantColor = color_N;
		m_enPassantPosition = -1;
	}

	// Erase start piece and set new end piece
	piece_t movingP = move.GetMovingPiece();
	if (movingP == piece_P)
	{
		m_fiftyMoveCount = 0;
		recordEnPassant(sRank, sFile, eRank, eFile);

		m_pawn &= eraseBit;
		m_pawn |= setBit;
	}
	else if (movingP == piece_N)
	{
		m_knight &= eraseBit;
		m_knight |= setBit;
	}
	else if (movingP == piece_B)
	{
		m_bishop &= eraseBit;
		m_bishop |= setBit;
	}
	else if (movingP == piece_R)
	{
		m_rook &= eraseBit;
		m_rook |= setBit;

		// Mark that the appropriate rook has moved
		if (sRank == rank_1)
		{
			if (sFile == file_A)
			{
				m_whiteMovedQueensideRook = true;
			}
			else if (sFile == file_H)
			{
				m_whiteMovedKingsideRook = true;
			}
		}
		else if (sRank == rank_8)
		{
			if (sFile == file_A)
			{
				m_blackMovedQueensideRook = true;
			}
			else if (sFile == file_H)
			{
				m_blackMovedKingsideRook = true;
			}
		}
	}
	else if (movingP == piece_Q)
	{
		m_queen &= eraseBit;
		m_queen |= setBit;

		// Mark that the appropriate queen has moved
		if (IsWhite(sRank, sFile))
		{
			m_whiteMovedQueen = true;
		}
		else if (IsBlack(sRank, sFile))
		{
			m_blackMovedQueen = true;
		}
	}
	else if (movingP == piece_K)
	{
		m_king &= eraseBit;
		m_king |= setBit;

		// Mark that the appropriate king has moved
		if (color == color_W)
		{
			m_whiteMovedKing = true;
			m_whiteKingLocation = GET_SQUARE(eRank, eFile);
		}
		else if (color == color_B)
		{
			m_blackMovedKing = true;
			m_blackKingLocation = GET_SQUARE(eRank, eFile);
		}
	}

	// Set colors
	if (color == color_W)
	{
		m_white &= eraseBit;
		m_white |= setBit;
		m_black &= ~setBit;
	}
	else if (color == color_B)
	{
		m_black &= eraseBit;
		m_black |= setBit;
		m_white &= ~setBit;
	}

	// Check for castling
	if (move.IsKingsideCastle())
	{
		if (color == color_W)
		{
			m_rook &= ~(I64(1) << 7);
			m_white &= ~(I64(1) << 7);
			m_rook |= (I64(1) << 5);
			m_white |= (I64(1) << 5);
		}
		else
		{
			m_rook &= ~(I64(1) << 63);
			m_black &= ~(I64(1) << 63);
			m_rook |= (I64(1) << 61);
			m_black |= (I64(1) << 61);
		}
	}
	else if (move.IsQueensideCastle())
	{
		if (color == color_W)
		{
			m_rook &= ~(I64(1) << 0);
			m_white &= ~(I64(1) << 0);
			m_rook |= (I64(1) << 3);
			m_white |= (I64(1) << 3);
		}
		else
		{
			m_rook &= ~(I64(1) << 56);
			m_black &= ~(I64(1) << 56);
			m_rook |= (I64(1) << 59);
			m_black |= (I64(1) << 59);
		}
	}

	// Check for pawn promotion
	piece_t promoPiece = move.GetPromotionPiece();
	if (promoPiece > piece_P)
	{
		square_t n = GET_SQUARE(eRank, eFile);
		bitboard_t promoBit = (I64(1) << n);

		// Set end piece
		if (promoPiece == piece_N)
		{
			m_knight |= promoBit;
		}
		else if (promoPiece == piece_B)
		{
			m_bishop |= promoBit;
		}
		else if (promoPiece == piece_R)
		{
			m_rook |= promoBit;
		}
		else if (promoPiece == piece_Q)
		{
			m_queen |= promoBit;
		}

		// Erase pawn
		m_pawn &= ~promoBit;
	}

	// Set the squares under attack
	generateAttackedSquares();

	// Set check flags
	SetCheckFlags();
	if (m_whiteInCheck || m_blackInCheck)
	{
		move.SetCheck();
	}

	// Set last move
	m_lastMove = move;

	// Change turn player
	m_playerInTurn = !m_playerInTurn;
}

//=============================================================================
bool BitBoard::IsPawn(square_t rank, square_t file) const
{
	square_t n = GET_SQUARE(rank, file);
	return (m_pawn >> n) & 0x1;
}

//=============================================================================
bool BitBoard::IsKnight(square_t rank, square_t file) const
{
	square_t n = GET_SQUARE(rank, file);
	return (m_knight >> n) & 0x1;
}

//=============================================================================
bool BitBoard::IsBishop(square_t rank, square_t file) const
{
	square_t n = GET_SQUARE(rank, file);
	return (m_bishop >> n) & 0x1;
}

//=============================================================================
bool BitBoard::IsRook(square_t rank, square_t file) const
{
	square_t n = GET_SQUARE(rank, file);
	return (m_rook >> n) & 0x1;
}

//=============================================================================
bool BitBoard::IsQueen(square_t rank, square_t file) const
{
	square_t n = GET_SQUARE(rank, file);
	return (m_queen >> n) & 0x1;
}

//=============================================================================
bool BitBoard::IsKing(square_t rank, square_t file) const
{
	square_t n = GET_SQUARE(rank, file);
	return (m_king >> n) & 0x1;
}

//=============================================================================
bool BitBoard::IsWhite(square_t rank, square_t file) const
{
	square_t n = GET_SQUARE(rank, file);
	return (m_white >> n) & 0x1;
}

//=============================================================================
bool BitBoard::IsBlack(square_t rank, square_t file) const
{
	square_t n = GET_SQUARE(rank, file);
	return (m_black >> n) & 0x1;
}

//=============================================================================
bool BitBoard::IsEmpty(square_t rank, square_t file) const
{
	return !(IsWhite(rank, file) || IsBlack(rank, file));
}

//=============================================================================
bool BitBoard::IsUnderAttack(square_t rank, square_t file, color_t color) const
{
	square_t n = GET_SQUARE(rank, file);

	if (color == color_W)
	{
		return (m_attackedByWhite >> n) & 0x1;
	}
	else
	{
		return (m_attackedByBlack >> n) & 0x1;
	}
}

//=============================================================================
void BitBoard::generateAttackedSquares()
{
	m_attackedByWhite = 0x0;
	m_attackedByBlack = 0x0;

	// White pieces
	bitboard_t pawnW = (m_pawn & m_white);
	bitboard_t knightW = (m_knight & m_white);
	bitboard_t bishopW = (m_bishop & m_white);
	bitboard_t rookW = (m_rook & m_white);
	bitboard_t queenW = (m_queen & m_white);
	bitboard_t kingW = (m_king & m_white);

	// Black pieces
	bitboard_t pawnB = (m_pawn & m_black);
	bitboard_t knightB = (m_knight & m_black);
	bitboard_t bishopB = (m_bishop & m_black);
	bitboard_t rookB = (m_rook & m_black);
	bitboard_t queenB = (m_queen & m_black);
	bitboard_t kingB = (m_king & m_black);

	bitboard_t allPieces = (m_white | m_black);
	bitboard_t allPiecesR90 = 0x0;
	//bitboard_t allPiecesR45 = 0x0;

	// Construct 90-degree rotated bitboards
	for (square_t i=0; i<BOARD_SIZE; i++)
	{
		square_t rank = GET_RANK(i);
		square_t file = GET_FILE(i);
		allPiecesR90 |= ((allPieces >> i) & 0x1) << GET_SQUARE(file, rank);
	}

	// PAWN
	bitboard_t notAFile = 0xfefefefefefefefe;
	bitboard_t notHFile = 0x7f7f7f7f7f7f7f7f;

	// White - Regular capture moves
	m_attackedByWhite |= (pawnW << I64(7)) & notHFile & ~m_white;
	m_attackedByWhite |= (pawnW << I64(9)) & notAFile & ~m_white;

	// White - En passant moves
	if (m_enPassantColor == color_B)
	{
		m_attackedByWhite |= (pawnW << I64(7)) & ((I64(1) << m_enPassantPosition));
		m_attackedByWhite |= (pawnW << I64(9)) & ((I64(1) << m_enPassantPosition));
	}

	// Black - Regular capture moves
	m_attackedByBlack |= (pawnB >> I64(7)) & notAFile & ~m_black;
	m_attackedByBlack |= (pawnB >> I64(9)) & notHFile & ~m_black;

	// Black - En passant moves
	if (m_enPassantColor == color_W)
	{
		m_attackedByBlack |= (pawnB >> I64(7)) & ((I64(1) << m_enPassantPosition));
		m_attackedByBlack |= (pawnB >> I64(9)) & ((I64(1) << m_enPassantPosition));
	}

	// KNIGHT - White
	m_attackedByWhite |= (knightW << I64(6)) & ~m_white;
	m_attackedByWhite |= (knightW << I64(10)) & ~m_white;
	m_attackedByWhite |= (knightW << I64(15)) & ~m_white;
	m_attackedByWhite |= (knightW << I64(17)) & ~m_white;
	m_attackedByWhite |= (knightW >> I64(6)) & ~m_white;
	m_attackedByWhite |= (knightW >> I64(10)) & ~m_white;
	m_attackedByWhite |= (knightW >> I64(15)) & ~m_white;
	m_attackedByWhite |= (knightW >> I64(17)) & ~m_white;

	// KNIGHT - Black
	m_attackedByBlack |= (knightB << I64(6)) & ~m_black;
	m_attackedByBlack |= (knightB << I64(10)) & ~m_black;
	m_attackedByBlack |= (knightB << I64(15)) & ~m_black;
	m_attackedByBlack |= (knightB << I64(17)) & ~m_black;
	m_attackedByBlack |= (knightB >> I64(6)) & ~m_black;
	m_attackedByBlack |= (knightB >> I64(10)) & ~m_black;
	m_attackedByBlack |= (knightB >> I64(15)) & ~m_black;
	m_attackedByBlack |= (knightB >> I64(17)) & ~m_black;

	// Sliding pieces
	for (square_t i=0; i<BOARD_SIZE; i++)
	{
		// BISHOP and QUEEN - White
		if (((bishopW >> i) & 0x1) || ((queenW >> i) & 0x1))
		{
			square_t rank = GET_RANK(i);
			square_t file = GET_FILE(i);

			// North-east
			for (square_t j=rank+1, k=file+1; j<NUM_RANKS && k<NUM_FILES; j++, k++)
			{
				square_t n = GET_SQUARE(j, k);
				if ((allPieces >> n) & 0x1)
				{
					m_attackedByWhite |= (I64(1) << n) & ~m_white;
					break;
				}
				else
				{
					m_attackedByWhite |= (I64(1) << n);
				}
			}

			// North-west
			for (square_t j=rank+1, k=file-1; j<NUM_RANKS && k>=0; j++, k--)
			{
				square_t n = GET_SQUARE(j, k);
				if ((allPieces >> n) & 0x1)
				{
					m_attackedByWhite |= (I64(1) << n) & ~m_white;
					break;
				}
				else
				{
					m_attackedByWhite |= (I64(1) << n);
				}
			}

			// South-east
			for (square_t j=rank-1, k=file+1; j>=0 && k<NUM_FILES; j--, k++)
			{
				square_t n = GET_SQUARE(j, k);
				if ((allPieces >> n) & 0x1)
				{
					m_attackedByWhite |= (I64(1) << n) & ~m_white;
					break;
				}
				else
				{
					m_attackedByWhite |= (I64(1) << n);
				}
			}

			// South-west
			for (square_t j=rank-1, k=file-1; j>=0 && k>=0; j--, k--)
			{
				square_t n = GET_SQUARE(j, k);
				if ((allPieces >> n) & 0x1)
				{
					m_attackedByWhite |= (I64(1) << n) & ~m_white;
					break;
				}
				else
				{
					m_attackedByWhite |= (I64(1) << n);
				}
			}
		}

		// BISHOP and QUEEN - Black
		else if (((bishopB >> i) & 0x1) || ((queenB >> i) & 0x1))
		{
			square_t rank = GET_RANK(i);
			square_t file = GET_FILE(i);

			// North-east
			for (square_t j=rank+1, k=file+1; j<NUM_RANKS && k<NUM_FILES; j++, k++)
			{
				square_t n = GET_SQUARE(j, k);
				if ((allPieces >> n) & 0x1)
				{
					m_attackedByBlack |= (I64(1) << n) & ~m_black;
					break;
				}
				else
				{
					m_attackedByBlack |= (I64(1) << n);
				}
			}

			// North-west
			for (square_t j=rank+1, k=file-1; j<NUM_RANKS && k>=0; j++, k--)
			{
				square_t n = GET_SQUARE(j, k);
				if ((allPieces >> n) & 0x1)
				{
					m_attackedByBlack |= (I64(1) << n) & ~m_black;
					break;
				}
				else
				{
					m_attackedByBlack |= (I64(1) << n);
				}
			}

			// South-east
			for (square_t j=rank-1, k=file+1; j>=0 && k<NUM_FILES; j--, k++)
			{
				square_t n = GET_SQUARE(j, k);
				if ((allPieces >> n) & 0x1)
				{
					m_attackedByBlack |= (I64(1) << n) & ~m_black;
					break;
				}
				else
				{
					m_attackedByBlack |= (I64(1) << n);
				}
			}

			// South-west
			for (square_t j=rank-1, k=file-1; j>=0 && k>=0; j--, k--)
			{
				square_t n = GET_SQUARE(j, k);
				if ((allPieces >> n) & 0x1)
				{
					m_attackedByBlack |= (I64(1) << n) & ~m_black;
					break;
				}
				else
				{
					m_attackedByBlack |= (I64(1) << n);
				}
			}
		}

		// ROOK and QUEEN - White
		if (((rookW >> i) & 0x1) || ((queenW >> i) & 0x1))
		{
			square_t rank = GET_RANK(i);
			square_t file = GET_FILE(i);
			square_t rankOcc = (allPieces >> (rank * 8)) & 0xff;
			square_t fileOcc = (allPiecesR90 >> (file * 8)) & 0xff;

			// North
			for (square_t j=rank+1; j<NUM_RANKS; j++)
			{
				if ((fileOcc >> j) & 0x1)
				{
					m_attackedByWhite |= (I64(1) << GET_SQUARE(j, file)) & ~m_white;
					break;
				}
				else
				{
					m_attackedByWhite |= (I64(1) << GET_SQUARE(j, file));
				}
			}

			// South
			for (square_t j=rank-1; j>=0; j--)
			{
				if ((fileOcc >> j) & 0x1)
				{
					m_attackedByWhite |= (I64(1) << GET_SQUARE(j, file)) & ~m_white;
					break;
				}
				else
				{
					m_attackedByWhite |= (I64(1) << GET_SQUARE(j, file));
				}
			}

			// East
			for (square_t j=file+1; j<NUM_FILES; j++)
			{
				if ((rankOcc >> j) & 0x1)
				{
					m_attackedByWhite |= (I64(1) << GET_SQUARE(rank, j)) & ~m_white;
					break;
				}
				else
				{
					m_attackedByWhite |= (I64(1) << GET_SQUARE(rank, j));
				}
			}

			// West
			for (square_t j=file-1; j>=0; j--)
			{
				if ((rankOcc >> j) & 0x1)
				{
					m_attackedByWhite |= (I64(1) << GET_SQUARE(rank, j)) & ~m_white;
					break;
				}
				else
				{
					m_attackedByWhite |= (I64(1) << GET_SQUARE(rank, j));
				}
			}
		}

		// ROOK and QUEEN - Black
		else if (((rookB >> i) & 0x1) || ((queenB >> i) & 0x1))
		{
			square_t rank = GET_RANK(i);
			square_t file = GET_FILE(i);
			square_t rankOcc = (allPieces >> (rank * 8)) & 0xff;
			square_t fileOcc = (allPiecesR90 >> (file * 8)) & 0xff;

			// North
			for (square_t j=rank+1; j<NUM_RANKS; j++)
			{
				if ((fileOcc >> j) & 0x1)
				{
					m_attackedByBlack |= (I64(1) << GET_SQUARE(j, file)) & ~m_black;
					break;
				}
				else
				{
					m_attackedByBlack |= (I64(1) << GET_SQUARE(j, file));
				}
			}

			// South
			for (square_t j=rank-1; j>=0; j--)
			{
				if ((fileOcc >> j) & 0x1)
				{
					m_attackedByBlack |= (I64(1) << GET_SQUARE(j, file)) & ~m_black;
					break;
				}
				else
				{
					m_attackedByBlack |= (I64(1) << GET_SQUARE(j, file));
				}
			}

			// East
			for (square_t j=file+1; j<NUM_FILES; j++)
			{
				if ((rankOcc >> j) & 0x1)
				{
					m_attackedByBlack |= (I64(1) << GET_SQUARE(rank, j)) & ~m_black;
					break;
				}
				else
				{
					m_attackedByBlack |= (I64(1) << GET_SQUARE(rank, j));
				}
			}

			// West
			for (square_t j=file-1; j>=0; j--)
			{
				if ((rankOcc >> j) & 0x1)
				{
					m_attackedByBlack |= (I64(1) << GET_SQUARE(rank, j)) & ~m_black;
					break;
				}
				else
				{
					m_attackedByBlack |= (I64(1) << GET_SQUARE(rank, j));
				}
			}
		}
	}

	// KING - White
	m_attackedByWhite |= (kingW << I64(1)) & ~m_white;
	m_attackedByWhite |= (kingW << I64(7)) & ~m_white;
	m_attackedByWhite |= (kingW << I64(8)) & ~m_white;
	m_attackedByWhite |= (kingW << I64(9)) & ~m_white;
	m_attackedByWhite |= (kingW >> I64(1)) & ~m_white;
	m_attackedByWhite |= (kingW >> I64(7)) & ~m_white;
	m_attackedByWhite |= (kingW >> I64(8)) & ~m_white;
	m_attackedByWhite |= (kingW >> I64(9)) & ~m_white;

	// KING - Black
	m_attackedByBlack |= (kingB << I64(1)) & ~m_black;
	m_attackedByBlack |= (kingB << I64(7)) & ~m_black;
	m_attackedByBlack |= (kingB << I64(8)) & ~m_black;
	m_attackedByBlack |= (kingB << I64(9)) & ~m_black;
	m_attackedByBlack |= (kingB >> I64(1)) & ~m_black;
	m_attackedByBlack |= (kingB >> I64(7)) & ~m_black;
	m_attackedByBlack |= (kingB >> I64(8)) & ~m_black;
	m_attackedByBlack |= (kingB >> I64(9)) & ~m_black;
}

//=============================================================================
bool BitBoard::IsWhiteInCheck() const
{
	return m_whiteInCheck;
}

//=============================================================================
bool BitBoard::IsBlackInCheck() const
{
	return m_blackInCheck;
}

//=============================================================================
void BitBoard::SetEndGame(bool endGame)
{
	m_endGame = endGame;
}

//=============================================================================
bool BitBoard::IsEndGame() const
{
	return m_endGame;
}

//=============================================================================
bool BitBoard::IsStalemateViaFiftyMoves() const
{
	return (m_fiftyMoveCount >= 50);
}

//=============================================================================
bool BitBoard::IsStalemateViaThreeMoveRep() const
{
	return (m_repeatedMoveCount >= 3);
}

//=============================================================================
color_t BitBoard::GetPlayerInTurn() const
{
	return m_playerInTurn;
}

//=============================================================================
color_t BitBoard::GetEnPassantColor() const
{
	return m_enPassantColor;
}

//=============================================================================
square_t BitBoard::GetEnPassantPosition() const
{
	return m_enPassantPosition;
}

//=============================================================================
square_t BitBoard::GetWhiteKingLocation() const
{
	return m_whiteKingLocation;
}

//=============================================================================
bool BitBoard::HasWhiteMovedKing() const
{
	return m_whiteMovedKing;
}

//=============================================================================
bool BitBoard::HasWhiteMovedQueen() const
{
	return m_whiteMovedQueen;
}

//=============================================================================
bool BitBoard::HasWhiteMovedKingsideRook() const
{
	return m_whiteMovedKingsideRook;
}

//=============================================================================
bool BitBoard::HasWhiteMovedQueensideRook() const
{
	return m_whiteMovedQueensideRook;
}

//=============================================================================
square_t BitBoard::GetBlackKingLocation() const
{
	return m_blackKingLocation;
}

//=============================================================================
bool BitBoard::HasBlackMovedKing() const
{
	return m_blackMovedKing;
}

//=============================================================================
bool BitBoard::HasBlackMovedQueen() const
{
	return m_blackMovedQueen;
}

//=============================================================================
bool BitBoard::HasBlackMovedKingsideRook() const
{
	return m_blackMovedKingsideRook;
}

//=============================================================================
bool BitBoard::HasBlackMovedQueensideRook() const
{
	return m_blackMovedQueensideRook;
}

//=============================================================================
bool BitBoard::HasWhiteCastled() const
{
	return m_whiteCastled;
}

//=============================================================================
bool BitBoard::HasBlackCastled() const
{
	return m_blackCastled;
}

//=============================================================================
void BitBoard::recordEnPassant(square_t sRank, square_t sFile, square_t eRank, square_t eFile)
{
	square_t n = GET_SQUARE(eRank, eFile);
	square_t diff = GET_SQUARE(sRank, sFile) - n;

	if (diff == 16 || diff == -16)
	{
		m_enPassantColor = (IsWhite(sRank, sFile) ? color_W : color_B);
		m_enPassantPosition = (n + (diff / 2));
	}
}

//=============================================================================
void BitBoard::SetCheckFlags()
{
	// White
	square_t rank = GET_RANK(m_whiteKingLocation);
	square_t file = GET_FILE(m_whiteKingLocation);
	m_whiteInCheck = IsUnderAttack(rank, file, color_B);

	// Black
	rank = GET_RANK(m_blackKingLocation);
	file = GET_FILE(m_blackKingLocation);
	m_blackInCheck = IsUnderAttack(rank, file, color_W);
}

//=============================================================================
ostream &operator<<(ostream &stream, BitBoard board)
{
	for (square_t i=NUM_RANKS-1; i>=0; i--)
	{
		for (square_t j=0; j<NUM_FILES; j++)
		{
			if (board.IsWhite(i, j))
			{
				stream << "w";
			}
			else if (board.IsBlack(i, j))
			{
				stream << "b";
			}
			else
			{
				stream << "-";
			}

			if (board.IsPawn(i,j))
			{
				stream << "p ";
			}
			else if (board.IsKnight(i,j))
			{
				stream << "k ";
			}
			else if (board.IsBishop(i,j))
			{
				stream << "b ";
			}
			else if (board.IsRook(i,j))
			{
				stream << "r ";
			}
			else if (board.IsQueen(i,j))
			{
				stream << "q ";
			}
			else if (board.IsKing(i,j))
			{
				stream << "K ";
			}
			else
			{
				stream << "- ";
			}
		}
		stream << "\n";
	}

	return stream;
}

}}
