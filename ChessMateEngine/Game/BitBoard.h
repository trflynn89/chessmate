/**
 * Class to represent a chess board. Uses a bit board representation. There are
 * 8 64-bit data types stored and maintained in the class - one for each piece
 * and one for each color. Each bit corresponds to one square on the board, and
 * is set high or low - high if that square can be represented by that piece or
 * color. The lowest bit corresponds to A1, and the highest bit corresponds to
 * H8.
 *
 * This class also stores information about the state of the board, such as its
 * score (according to board evaluation), if anyone is in check, if stalemate 
 * has occurred, the last move to have occurred, if en passant is possible, 
 * which side can castle, and counters for game termination (via the 50 move
 * rule or 3 move repetition rule).
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
#ifndef _BIT_BOARD_H_
#define _BIT_BOARD_H_

#include <Movement/Move.h>
#include <ChessMateEngine.h>

namespace ChessMate { namespace Game {

class BitBoard 
{
public:
	/**
	 * Default constructor.
	 */
	BitBoard();

	/**
	 * Copy constructor.
	 *
	 * @param The board to copy.
	 */
	BitBoard(const BitBoard &);

	/**
	 * Move a piece on the board. 
	 * Note: no validation is performed here. The piece will be moved blindly.
	 *
	 * @param The move to make.
	 */
	void MakeMove(ChessMate::Movement::Move &);

	/**
	 * Determine if a piece is a pawn.
	 *
	 * @param The rank of the piece to check.
	 * @param The file of the piece to check.
	 * @return True if the piece is a pawn.
	 */
	bool IsPawn(square_t, square_t) const;

	/**
	 * Determine if a piece is a knight.
	 *
	 * @param The rank of the piece to check.
	 * @param The file of the piece to check.
	 * @return True if the piece is a knight.
	 */
	bool IsKnight(square_t, square_t) const;

	/**
	 * Determine if a piece is a bishop.
	 *
	 * @param The rank of the piece to check.
	 * @param The file of the piece to check.
	 * @return True if the piece is a bishop.
	 */
	bool IsBishop(square_t, square_t) const;

	/**
	 * Determine if a piece is a rook.
	 *
	 * @param The rank of the piece to check.
	 * @param The file of the piece to check.
	 * @return True if the piece is a rook.
	 */
	bool IsRook(square_t, square_t) const;

	/**
	 * Determine if a piece is a queen.
	 *
	 * @param The rank of the piece to check.
	 * @param The file of the piece to check.
	 * @return True if the piece is a queen.
	 */
	bool IsQueen(square_t, square_t) const;

	/**
	 * Determine if a piece is a king.
	 *
	 * @param The rank of the piece to check.
	 * @param The file of the piece to check.
	 * @return True if the piece is a king.
	 */
	bool IsKing(square_t, square_t) const;

	/**
	 * Determine if a piece is a white piece.
	 *
	 * @param The rank of the piece to check.
	 * @param The file of the piece to check.
	 * @return True if the piece is a white piece.
	 */
	bool IsWhite(square_t, square_t) const;

	/**
	 * Determine if a piece is a black piece.
	 *
	 * @param The rank of the piece to check.
	 * @param The file of the piece to check.
	 * @return True if the piece is a black piece.
	 */
	bool IsBlack(square_t, square_t) const;

	/**
	 * Determine if a location is empty.
	 *
	 * @param The rank of the piece to check.
	 * @param The file of the piece to check.
	 * @return True if the piece is a black piece.
	 */
	bool IsEmpty(square_t, square_t) const;

	/**
	 * Determine if a square if under attack by a certain color.
	 *
	 * @param The rank of the square to check.
	 * @param The file of the piece to check.
	 * @param The color of the attacking player.
	 * @return True if the attacking player is attacking the square.
	 */
	bool IsUnderAttack(square_t, square_t, color_t) const;

	/**
	 * @return True if white is in check after the last move.
	 */
	bool IsWhiteInCheck() const;

	/**
	 * @return True if black is in check after the last move.
	 */
	bool IsBlackInCheck() const;

	/**
	 * Set the end game flag.
	 *
	 * @param True if the game is in the end game, false otherwise.
	 */
	void SetEndGame(bool);

	/**
	 * @return True if the game is in the end game, false otherwise.
	 */
	bool IsEndGame() const;

	/**
	 * @return True if the game is in stalemate from fifty move rule.
	 */
	bool IsStalemateViaFiftyMoves() const;

	/**
	 * @return True if the game is in stalemate from three move repetition rule.
	 */
	bool IsStalemateViaThreeMoveRep() const;

	/**
	 * @return The current player in turn (color_W or color_B).
	 */
	color_t GetPlayerInTurn() const;

	/**
	 * @return The color of the pawn that set the en passant flag, or color_N.
	 */
	color_t GetEnPassantColor() const;

	/**
	 * @return The location of the pawn that set the en passant flag, or -1.
	 */
	square_t GetEnPassantPosition() const;
	
	/**
	 * @return The location of the white king.
	 */
	square_t GetWhiteKingLocation() const;

	/**
	 * @return True if the white king has moved.
	 */
	bool HasWhiteMovedKing() const;

	/**
	 * @return True if the white queen has moved.
	 */
	bool HasWhiteMovedQueen() const;

	/**
	 * @return True if the white kingside rook has moved.
	 */
	bool HasWhiteMovedKingsideRook() const;

	/**
	 * @return True if the white queenside rook has moved.
	 */
	bool HasWhiteMovedQueensideRook() const;

	/**
	 * @return The location of the black king.
	 */
	square_t GetBlackKingLocation() const;

	/**
	 * @return True if the black king has moved.
	 */
	bool HasBlackMovedKing() const;

	/**
	 * @return True if the black queen has moved.
	 */
	bool HasBlackMovedQueen() const;

	/**
	 * @return True if the black kingside rook has moved.
	 */
	bool HasBlackMovedKingsideRook() const;

	/**
	 * @return True if the black queenside rook has moved.
	 */
	bool HasBlackMovedQueensideRook() const;

	/**
	 * @return True if white has castled.
	 */
	bool HasWhiteCastled() const;

	/**
	 * @return True if black has castled.
	 */
	bool HasBlackCastled() const;

	/**
	 * Overloaded operator << for printing the board.
	 *
	 * @param The stream to print to.
	 * @param The board to print.
	 */
	friend std::ostream &operator<<(std::ostream &, BitBoard);

private:
	/** Piece locations **/
	bitboard_t m_pawn;
	bitboard_t m_knight;
	bitboard_t m_bishop;
	bitboard_t m_rook;
	bitboard_t m_queen;
	bitboard_t m_king;
	bitboard_t m_white;
	bitboard_t m_black;

	/** Board state **/

	// Squares under attack
	bitboard_t m_attackedByWhite;
	bitboard_t m_attackedByBlack;

	// Score of the board used for evaluation. The score is relative - positive
	// is good for white, negative is good for black
	int m_boardScore; 

	// Current player in turn
	color_t m_playerInTurn;

	// Last move that has occured
	ChessMate::Movement::Move m_lastMove;

	// Locations of the king
	square_t m_whiteKingLocation;
	square_t m_blackKingLocation;

	// Flags for check and stale mate
	bool m_whiteInCheck;
	bool m_blackInCheck;

	// Keep track of whether the game is in the end game phase
	bool m_endGame;

	// Flags for whether specific pieces have moved
	bool m_whiteMovedKing;
	bool m_whiteMovedQueen;
	bool m_whiteMovedKingsideRook;
	bool m_whiteMovedQueensideRook;
	bool m_blackMovedKing;
	bool m_blackMovedQueen;
	bool m_blackMovedKingsideRook;
	bool m_blackMovedQueensideRook;

	// Flags for castling
	bool m_whiteCastled;
	bool m_blackCastled;

	// Counters for game-termination rules
	short m_fiftyMoveCount;
	short m_repeatedMoveCount;

	// En passant flags
	color_t m_enPassantColor;
	square_t m_enPassantPosition; // 0-63. Position where a pawn would move to.

	/** Helper methods **/

	/**
	 * Set which squares are under attack by both sides.
	 */
	void generateAttackedSquares();

	/**
	 * Set the en passant flags if one is possible.
	 *
	 * @param The starting rank of the move.
	 * @param The starting file of the move.
	 * @param The ending rank of the move.
	 * @param The ending file of the move.
	 */
	void recordEnPassant(square_t, square_t, square_t, square_t);

	/**
	 * Set the flags for whether either side is in check.
	 */
	void SetCheckFlags();
};

}}

#endif
