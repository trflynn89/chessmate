/**
 * Class to store data about a move.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
#ifndef _MOVE_H_
#define _MOVE_H_

#include <array>
#include <iostream>
#include <string>

#include <ChessMateEngine.h>

namespace ChessMate { namespace Movement {

// Move lists
class Move;
typedef std::array<Move, MAX_NUM_MOVES>		moveList_t;
typedef std::array<moveList_t, BOARD_SIZE>	moveList2d_t;
typedef std::array<Move, MAX_NUM_MOVES_TOT>	validMoveList_t;

class Move {
public:
	/**
	 * Default constructor.
	 */
	Move();

	/**
	 * Constructor to set the move data.
	 *
	 * @param The starting rank of the move.
	 * @param The starting file of the move.
	 * @param The ending rank of the move.
	 * @param The ending file of the move.
	 */
	Move(square_t, square_t, square_t, square_t);

	/**
	 * Constructor to set the move data.
	 *
	 * @param The starting rank of the move.
	 * @param The starting file of the move.
	 * @param The ending rank of the move.
	 * @param The ending file of the move.
	 * @param The promotion piece.
	 */
	Move(square_t, square_t, square_t, square_t, square_t);

	/**
	 * Constructor to parse a move in PGN format.
	 *
	 * @param The PGN formatted string.
	 * @param The color of the player who generated the move.
	 */
	Move(const std::string, color_t);

	/**
	 * @return The move's start file.
	 */
	square_t GetStartFile() const;

	/**
	 * @return The move's start rank.
	 */
	square_t GetStartRank() const;

	/**
	 * @return The move's end file.
	 */
	square_t GetEndFile() const;

	/**
	 * @return The move's end rank.
	 */
	square_t GetEndRank() const;

	/**
	 * @return The move formatted into a PGN string.
	 */
	std::string GetPGNString() const;

	/**
	 * @return The piece being moved.
	 */
	piece_t GetMovingPiece() const;

	/**
	 * Set the piece being moved.
	 */
	void SetMovingPiece(piece_t);

	/**
	 * @return This move's promotion piece.
	 */
	piece_t GetPromotionPiece() const;

	/**
	 * Set this move's promotion piece.
	 */
	void SetPromotionPiece(piece_t);

	/**
	 * @return True if this move captures a piece.
	 */
	bool IsCapture() const;

	/**
	 * Set that this move captures a piece.
	 */
	void SetCapture();

	/**
	 * @return True if this puts a player in check.
	 */
	bool IsCheck() const;

	/**
	 * Set that this move puts a player in check.
	 */
	void SetCheck();

	/**
	 * @return True if this puts a player in checkmate.
	 */
	bool IsCheckmate() const;

	/**
	 * Set that this move puts a player in checkmate.
	 */
	void SetCheckmate();

	/**
	 * @return True if this move is an en passant.
	 */
	bool IsEnPassant() const;

	/**
	 * Set that this move is an en passant.
	 */
	void SetEnPassant();

	/**
	 * @return True if this move is a kingside castle.
	 */
	bool IsKingsideCastle() const;

	/**
	 * Set that this move is a kingside castle.
	 */
	void SetKingsideCastle();

	/**
	 * @return True if this move is a queenside castle.
	 */
	bool IsQueensideCastle() const;

	/**
	 * Set that this move is a queenside castle.
	 */
	void SetQueensideCastle();

	/**
	 * Overloaded operator == for comparing moves.
	 *
	 * @param The move to compare.
	 * @return True if the moves are identical, false otherwise.
	 */
	bool operator==(const Move &move) const;

	/**
	 * Overloaded operator << for printing the move.
	 *
	 * @param The stream to print to.
	 * @param The move to print.
	 */
	friend std::ostream &operator<<(std::ostream &stream, Move move)
	{
		stream << move.m_sRank << " ";
		stream << move.m_sFile << " ";
		stream << move.m_eRank << " ";
		stream << move.m_eFile;

		return stream;
	}

private:
	square_t m_sRank;
	square_t m_sFile;
	square_t m_eRank;
	square_t m_eFile;

	piece_t m_movingPiece;
	piece_t m_promotionPiece;

	bool m_IsCheck;
	bool m_IsCheckmate;
	bool m_IsStalemate;
	bool m_IsCapture;
	bool m_IsKingsideCastle;
	bool m_IsQueensideCastle;
	bool m_IsEnPassant;
};

}}

#endif
