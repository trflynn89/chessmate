#pragma once

#include <fly/fly.h>

#include <game/board_types.h>
#include <movement/move.h>

namespace chessmate {

DEFINE_CLASS_PTRS(BitBoard);

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
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version March 3, 2013
 */
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
     * @param BitBoard The board to copy.
     */
    BitBoard(const BitBoard &);

    /**
     * Move a piece on the board.
     * Note: no validation is performed here. The piece will be moved blindly.
     *
     * @param Move The move to make.
     */
    void MakeMove(Move &);

    /**
     * Determine if a piece is a pawn.
     *
     * @param square_type The rank of the piece to check.
     * @param square_type The file of the piece to check.
     *
     * @return True if the piece is a pawn.
     */
    bool IsPawn(const square_type &, const square_type &) const;

    /**
     * Determine if a piece is a knight.
     *
     * @param square_type The rank of the piece to check.
     * @param square_type The file of the piece to check.
     *
     * @return True if the piece is a knight.
     */
    bool IsKnight(const square_type &, const square_type &) const;

    /**
     * Determine if a piece is a bishop.
     *
     * @param square_type The rank of the piece to check.
     * @param square_type The file of the piece to check.
     *
     * @return True if the piece is a bishop.
     */
    bool IsBishop(const square_type &, const square_type &) const;

    /**
     * Determine if a piece is a rook.
     *
     * @param square_type The rank of the piece to check.
     * @param square_type The file of the piece to check.
     *
     * @return True if the piece is a rook.
     */
    bool IsRook(const square_type &, const square_type &) const;

    /**
     * Determine if a piece is a queen.
     *
     * @param square_type The rank of the piece to check.
     * @param square_type The file of the piece to check.
     *
     * @return True if the piece is a queen.
     */
    bool IsQueen(const square_type &, const square_type &) const;

    /**
     * Determine if a piece is a king.
     *
     * @param square_type The rank of the piece to check.
     * @param square_type The file of the piece to check.
     *
     * @return True if the piece is a king.
     */
    bool IsKing(const square_type &, const square_type &) const;

    /**
     * Determine if a piece is a white piece.
     *
     * @param square_type The rank of the piece to check.
     * @param square_type The file of the piece to check.
     *
     * @return True if the piece is a white piece.
     */
    bool IsWhite(const square_type &, const square_type &) const;

    /**
     * Determine if a piece is a black piece.
     *
     * @param square_type The rank of the piece to check.
     * @param square_type The file of the piece to check.
     *
     * @return True if the piece is a black piece.
     */
    bool IsBlack(const square_type &, const square_type &) const;

    /**
     * Determine if a location is empty.
     *
     * @param square_type The rank of the piece to check.
     * @param square_type The file of the piece to check.
     *
     * @return True if the piece is a black piece.
     */
    bool IsEmpty(const square_type &, const square_type &) const;

    /**
     * Get the color of the piece that is occupying the given location.
     *
     * @param square_type The rank of the piece to check.
     * @param square_type The file of the piece to check.
     *
     * @return The color of the piece, or NONE.
     */
    color_type GetOccupant(const square_type &, const square_type &) const;

    /**
     * Determine if a square if under attack by a certain color.
     *
     * @param square_type The rank of the square to check.
     * @param square_type The file of the piece to check.
     * @param color_type The color of the attacking player.
     *
     * @return True if the attacking player is attacking the square.
     */
    bool IsUnderAttack(const square_type &, const square_type &, const color_type &) const;

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
     * @param bool True if the game is in the end game, false otherwise.
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
    bool IsStalemateViaRepetition() const;

    /**
     * @return The current player in turn (WHITE or BLACK).
     */
    color_type GetPlayerInTurn() const;

    /**
     * @return The color of the pawn that set the en passant flag, or color_N.
     */
    color_type GetEnPassantColor() const;

    /**
     * @return The location of the pawn that set the en passant flag, or -1.
     */
    square_type GetEnPassantPosition() const;

    /**
     * @return The location of the white king.
     */
    square_type GetWhiteKingLocation() const;

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
    square_type GetBlackKingLocation() const;

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
     * @param ostream The stream to print to.
     * @param BitBoard The board to print.
     */
    friend std::ostream &operator << (std::ostream &, const BitBoard &);

private:
    /**
     * Set which squares are under attack by both sides.
     */
    void generateAttackedSquares();

    /**
     * Set the en passant flags if one is possible.
     *
     * @param square_type The starting rank of the move.
     * @param square_type The starting file of the move.
     * @param square_type The ending rank of the move.
     * @param square_type The ending file of the move.
     */
    void recordEnPassant(const square_type &, const square_type &, const square_type &, const square_type &);

    /**
     * Set the flags for whether either side is in check.
     */
    void setCheckFlags();

    // Piece locations
    board_type m_pawn;
    board_type m_knight;
    board_type m_bishop;
    board_type m_rook;
    board_type m_queen;
    board_type m_king;
    board_type m_white;
    board_type m_black;

    // Squares under attack
    board_type m_attackedByWhite;
    board_type m_attackedByBlack;

    // Score of the board used for evaluation. The score is relative - positive
    // is good for white, negative is good for black
    int m_boardScore;

    // Current player in turn
    color_type m_playerInTurn;

    // Last move that has occured
    Move m_lastMove;

    // Locations of the king
    square_type m_whiteKingLocation;
    square_type m_blackKingLocation;

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
    color_type m_enPassantColor;
    square_type m_enPassantPosition; // 0-63. Position where a pawn would move to.
};

}
