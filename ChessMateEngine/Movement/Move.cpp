/**
 * Implementation of the Move interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
#include "Move.h"

using std::string;

namespace ChessMate { namespace Movement {

//=============================================================================
Move::Move()
:
	m_sRank(-1), m_sFile(-1), m_eRank(-1), m_eFile(-1), m_movingPiece(-1),
	m_promotionPiece(-1), m_IsCheck(false), m_IsCheckmate(false), m_IsStalemate(false),
	m_IsCapture(false), m_IsKingsideCastle(false), m_IsQueensideCastle(false), m_IsEnPassant(false)
{
}

//=============================================================================
Move::Move(square_t sRank, square_t sFile, square_t eRank, square_t eFile) 
:
	m_sRank(sRank), m_sFile(sFile), m_eRank(eRank), m_eFile(eFile), m_movingPiece(-1),
	m_promotionPiece(-1), m_IsCheck(false), m_IsCheckmate(false), m_IsStalemate(false),
	m_IsCapture(false), m_IsKingsideCastle(false), m_IsQueensideCastle(false), m_IsEnPassant(false)
{
}

//=============================================================================
Move::Move(square_t sRank, square_t sFile, square_t eRank, square_t eFile, piece_t promotionPiece)
:
	m_sRank(sRank),	m_sFile(sFile), m_eRank(eRank), m_eFile(eFile), m_movingPiece(-1),
	m_promotionPiece(promotionPiece), m_IsCheck(false), m_IsCheckmate(false), m_IsStalemate(false),
	m_IsCapture(false), m_IsKingsideCastle(false), m_IsQueensideCastle(false), m_IsEnPassant(false)
{
}

//=============================================================================
Move::Move(const string move, color_t player)
:
	m_promotionPiece(piece_P), m_IsCheck(false), m_IsCheckmate(false), m_IsStalemate(false),
	m_IsCapture(false), m_IsKingsideCastle(false), m_IsQueensideCastle(false), m_IsEnPassant(false)
{
	// Parse for castle
	if (move.compare("O-O") == 0)
	{
		m_movingPiece = piece_K;
		m_IsKingsideCastle = true;

		m_sFile = file_E;
		m_eFile = file_G;

		if (player == color_W)
		{
			m_sRank = rank_1;
			m_eRank = rank_1;
		}
		else
		{
			m_sRank = rank_8;
			m_eRank = rank_8;
		}

		return;
	}
	else if (move.compare("O-O-O") == 0)
	{
		m_movingPiece = piece_K;
		m_IsQueensideCastle = true;

		m_sFile = file_E;
		m_eFile = file_C;

		if (player == color_W)
		{
			m_sRank = rank_1;
			m_eRank = rank_1;
		}
		else
		{
			m_sRank = rank_8;
			m_eRank = rank_8;
		}

		return;
	}

	unsigned int ix = 0;

	// Parse starting piece
	if (move[ix] == 'N')
	{
		ix++;
		m_movingPiece = piece_N;
	}
	else if (move[ix] == 'B')
	{
		ix++;
		m_movingPiece = piece_B;
	}
	else if (move[ix] == 'R')
	{
		ix++;
		m_movingPiece = piece_R;
	}
	else if (move[ix] == 'Q')
	{
		ix++;
		m_movingPiece = piece_Q;
	}
	else if (move[ix] == 'K')
	{
		ix++;
		m_movingPiece = piece_K;
	}
	else
	{
		m_movingPiece = piece_P;
	}

	// Parse starting position
	m_sFile = move[ix++] - 'a';
	m_sRank = move[ix++] - '1';

	// Parse for capture
	if (move[ix] == 'x')
	{
		ix++;
		m_IsCapture = true;
	}

	// Parse ending position
	m_eFile = move[ix++] - 'a';
	m_eRank = move[ix++] - '1';

	// Special moves
	while (ix < move.length())
	{
		if (move[ix] == '=')
		{
			ix++;

			if (move[ix] == 'N')
			{
				m_promotionPiece = piece_N;
			}
			else if (move[ix] == 'B')
			{
				m_promotionPiece = piece_B;
			}
			else if (move[ix] == 'R')
			{
				m_promotionPiece = piece_R;
			}
			else if (move[ix] == 'Q')
			{
				m_promotionPiece = piece_Q;
			}
			else
			{
				m_promotionPiece = piece_P;
			}
		}
		else if (move[ix] == 'e')
		{
			if (move[++ix] == 'p')
			{
				m_IsEnPassant = true;
			}
		}
		else if (move[ix] == '+')
		{
			m_IsCheck = true;
		}
		else if (move[ix] == '#')
		{
			m_IsCheckmate = true;
		}

		ix++;
	}
}

//=============================================================================
square_t Move::GetStartFile() const
{
	return m_sFile;
}

//=============================================================================
square_t Move::GetStartRank() const
{
	return m_sRank;
}

//=============================================================================
square_t Move::GetEndFile() const
{
	return m_eFile;
}

//=============================================================================
square_t Move::GetEndRank() const
{
	return m_eRank;
}

//=============================================================================
std::string Move::GetPGNString() const
{
	if (m_IsKingsideCastle)
	{
		return "O-O";
	}
	else if (m_IsQueensideCastle)
	{
		return "O-O-O";
	}

	string ret("");

	// Set the moving piece
	if (m_movingPiece == piece_N)
	{
		ret += "N";
	}
	else if (m_movingPiece == piece_B)
	{
		ret += "B";
	}
	else if (m_movingPiece == piece_R)
	{
		ret += "R";
	}
	else if (m_movingPiece == piece_Q)
	{
		ret += "Q";
	}
	else if (m_movingPiece == piece_K)
	{
		ret += "K";
	}

	// Append the start file and start rank
	ret += ('a' + m_sFile);
	ret += ('1' + m_sRank);

	// If move is capture, indicate such
	if (m_IsCapture)
	{
		ret += 'x';
	}

	// Append the end file and end rank
	ret += ('a' + m_eFile);
	ret += ('1' + m_eRank);

	// If move is a promotion, indicate the promotion piece
	if (m_promotionPiece == piece_N)
	{
		ret += "=N";
	}
	else if (m_promotionPiece == piece_B)
	{
		ret += "=B";
	}
	else if (m_promotionPiece == piece_R)
	{
		ret += "=R";
	}
	else if (m_promotionPiece == piece_Q)
	{
		ret += "=Q";
	}

	// If move is an enpassant, indicate such
	if (m_IsEnPassant)
	{
		ret += "ep";
	}

	// If move is check or checkmate, indicate such
	if (m_IsCheckmate)
	{
		ret += '#';
	}
	else if (m_IsCheck)
	{
		ret += '+';
	}

	return ret;
}

//=============================================================================
piece_t Move::GetMovingPiece() const
{
	return m_movingPiece;
}

//=============================================================================
void Move::SetMovingPiece(piece_t movingPiece)
{
	m_movingPiece = movingPiece;
}

//=============================================================================
piece_t Move::GetPromotionPiece() const
{
	return m_promotionPiece;
}

//=============================================================================
void Move::SetPromotionPiece(piece_t promotionPiece)
{
	m_promotionPiece = promotionPiece;
}

//=============================================================================
bool Move::IsCapture() const
{
	return m_IsCapture;
}

//=============================================================================
void Move::SetCapture()
{
	m_IsCapture = true;
}

//=============================================================================
bool Move::IsCheck() const
{
	return m_IsCheck;
}

//=============================================================================
void Move::SetCheck()
{
	m_IsCheck = true;
}

//=============================================================================
bool Move::IsCheckmate() const
{
	return m_IsCheckmate;
}

//=============================================================================
void Move::SetCheckmate()
{
	m_IsCheckmate = true;
}

//=============================================================================
bool Move::IsEnPassant() const
{
	return m_IsEnPassant;
}

//=============================================================================
void Move::SetEnPassant()
{
	m_IsEnPassant = true;
}

//=============================================================================
bool Move::IsKingsideCastle() const
{
	return m_IsKingsideCastle;
}

//=============================================================================
void Move::SetKingsideCastle()
{
	m_IsKingsideCastle = true;
}

//=============================================================================
bool Move::IsQueensideCastle() const
{
	return m_IsQueensideCastle;
}

//=============================================================================
void Move::SetQueensideCastle()
{
	m_IsQueensideCastle = true;
}

//=============================================================================
bool Move::operator==(const Move &move) const
{
	return
	(
		(m_sRank == move.m_sRank) &&
		(m_sFile == move.m_sFile) &&
		(m_eRank == move.m_eRank) &&
		(m_eFile == move.m_eFile)
	);
}

}}
