#pragma once

#include <array>
#include <iostream>
#include <string>

#include <game/board_types.h>

namespace Movement {

/**
 * Class to store data about a move.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version March 3, 2013
 */
class Move
{
public:
    /**
     * Default constructor.
     */
    Move();

    /**
     * Constructor to set the move data.
     *
     * @param square_type The starting rank of the move.
     * @param square_type The starting file of the move.
     * @param square_type The ending rank of the move.
     * @param square_type The ending file of the move.
     */
    Move(
        const Game::square_type &,
        const Game::square_type &,
        const Game::square_type &,
        const Game::square_type &
    );

    /**
     * Constructor to set the move data.
     *
     * @param square_type The starting rank of the move.
     * @param square_type The starting file of the move.
     * @param square_type The ending rank of the move.
     * @param square_type The ending file of the move.
     * @param piece_type The promotion piece.
     */
    Move(
        const Game::square_type &,
        const Game::square_type &,
        const Game::square_type &,
        const Game::square_type &,
        const Game::piece_type &
    );

    /**
     * Constructor to parse a move in PGN format.
     *
     * @param string The PGN formatted string.
     * @param color_type The color of the player who generated the move.
     */
    Move(const std::string &, Game::color_type);

    /**
     * @return The move's start file.
     */
    Game::square_type GetStartFile() const;

    /**
     * @return The move's start rank.
     */
    Game::square_type GetStartRank() const;

    /**
     * @return The move's end file.
     */
    Game::square_type GetEndFile() const;

    /**
     * @return The move's end rank.
     */
    Game::square_type GetEndRank() const;

    /**
     * @return The move formatted into a PGN string.
     */
    std::string GetPGNString() const;

    /**
     * @return The piece being moved.
     */
    Game::piece_type GetMovingPiece() const;

    /**
     * Set the piece being moved.
     */
    void SetMovingPiece(Game::piece_type);

    /**
     * @return This move's promotion piece.
     */
    Game::piece_type GetPromotionPiece() const;

    /**
     * Set this move's promotion piece.
     */
    void SetPromotionPiece(Game::piece_type);

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
     * @param Move The move to compare.
     *
     * @return True if the moves are identical, false otherwise.
     */
    bool operator == (const Move &move) const;

    /**
     * Overloaded operator << for printing the move.
     *
     * @param stream The stream to print to.
     * @param Move The move to print.
     */
    friend std::ostream &operator << (std::ostream &, const Move &);

private:
    Game::square_type m_startRank;
    Game::square_type m_startFile;
    Game::square_type m_endRank;
    Game::square_type m_endFile;

    Game::piece_type m_movingPiece;
    Game::piece_type m_promotionPiece;

    bool m_IsCheck;
    bool m_IsCheckmate;
    bool m_IsStalemate;
    bool m_IsCapture;
    bool m_IsKingsideCastle;
    bool m_IsQueensideCastle;
    bool m_IsEnPassant;
};

}
