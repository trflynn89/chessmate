#pragma once

#include <array>
#include <iostream>
#include <string>

#include <Game/BoardTypes.h>

#define MAX_NUM_MOVES       8
#define MAX_NUM_MOVES_TOT   512 // TODO determine best size

namespace Movement {

class Move;
typedef std::array<Move, MAX_NUM_MOVES>     moveList_t;
typedef std::array<moveList_t, Game::BOARD_SIZE>  moveList2d_t;
typedef std::array<Move, MAX_NUM_MOVES_TOT> validMoveList_t;

/**
 * Class to store data about a move.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
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
    bool operator==(const Move &move) const;

    /**
     * Overloaded operator << for printing the move.
     *
     * @param stream The stream to print to.
     * @param Move The move to print.
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
    Game::square_type m_sRank;
    Game::square_type m_sFile;
    Game::square_type m_eRank;
    Game::square_type m_eFile;

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
