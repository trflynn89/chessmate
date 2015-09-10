#pragma once

#include <array>
#include <vector>
#include <list>

#include <Movement/Move.h>
#include <Game/BoardTypes.h>

namespace Movement {

typedef std::vector<Move> MoveList;
typedef std::vector<MoveList> MoveLists;

/**
 * Class to calculate a list of all possible moves for every piece at every
 * possible location, regardless of board setup.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version September 9, 2015
 */
class MoveSet
{
public:
    /**
     * Initialize the move sets.
     */
    MoveSet();

    /**
     * @return All possible moves a white pawn can make.
     */
    MoveLists GetWhitePawnMoves() const;

    /**
     * @return All possible moves a black pawn can make.
     */
    MoveLists GetBlackPawnMoves() const;

    /**
     * @return All possible moves a knight can make.
     */
    MoveLists GetKnightMoves() const;

    /**
     * @return All possible moves a bishop can make in a NE direction.
     */
    MoveLists GetBishopMovesNE() const;

    /**
     * @return All possible moves a bishop can make in a NW direction.
     */
    MoveLists GetBishopMovesNW() const;

    /**
     * @return All possible moves a bishop can make in a SE direction.
     */
    MoveLists GetBishopMovesSE() const;

    /**
     * @return All possible moves a bishop can make in a SW direction.
     */
    MoveLists GetBishopMovesSW() const;

    /**
     * @return All possible moves a rook can make in a N direction.
     */
    MoveLists GetRookMovesN() const;

    /**
     * @return All possible moves a rook can make in a S direction.
     */
    MoveLists GetRookMovesS() const;

    /**
     * @return All possible moves a rook can make in an E direction.
     */
    MoveLists GetRookMovesE() const;

    /**
     * @return All possible moves a rook can make in a W direction.
     */
    MoveLists GetRookMovesW() const;

    /**
     * @return All possible moves a queen can make in a N direction.
     */
    MoveLists GetQueenMovesN() const;

    /**
     * @return All possible moves a queen can make in a S direction.
     */
    MoveLists GetQueenMovesS() const;

    /**
     * @return All possible moves a queen can make in an E direction.
     */
    MoveLists GetQueenMovesE() const;

    /**
     * @return All possible moves a queen can make in a W direction.
     */
    MoveLists GetQueenMovesW() const;

    /**
     * @return All possible moves a queen can make in a NE direction.
     */
    MoveLists GetQueenMovesNE() const;

    /**
     * @return All possible moves a queen can make in a NW direction.
     */
    MoveLists GetQueenMovesNW() const;

    /**
     * @return All possible moves a queen can make in a SE direction.
     */
    MoveLists GetQueenMovesSE() const;

    /**
     * @return All possible moves a queen can make in a SW direction.
     */
    MoveLists GetQueenMovesSW() const;

    /**
     * @return All possible moves a king can make
     */
    MoveLists GetKingMoves() const;

private:
    /**
     * Initialize the moves a white pawn can make.
     */
    void initializeWhitePawnMoves();

    /**
     * Initialize the moves a black pawn can make.
     */
    void initializeBlackPawnMoves();

    /**
     * Initialize the moves a knight can make.
     */
    void initializeKnightMoves();

    /**
     * Initialize the moves a bishop can make.
     */
    void initializeBishopMoves();

    /**
     * Initialize the moves a rook can make.
     */
    void initializeRookMoves();

    /**
     * Initialize the moves a queen can make.
     */
    void initializeQueenMoves();

    /**
     * Initialize the moves a king can make.
     */
    void initializeKingMoves();

    MoveLists m_whitePawnMoves;
    MoveLists m_blackPawnMoves;
    MoveLists m_knightMoves;
    MoveLists m_bishopMovesNE;
    MoveLists m_bishopMovesNW;
    MoveLists m_bishopMovesSE;
    MoveLists m_bishopMovesSW;
    MoveLists m_rookMovesN;
    MoveLists m_rookMovesS;
    MoveLists m_rookMovesE;
    MoveLists m_rookMovesW;
    MoveLists m_queenMovesN;
    MoveLists m_queenMovesS;
    MoveLists m_queenMovesE;
    MoveLists m_queenMovesW;
    MoveLists m_queenMovesNE;
    MoveLists m_queenMovesNW;
    MoveLists m_queenMovesSE;
    MoveLists m_queenMovesSW;
    MoveLists m_kingMoves;
};

}
