#pragma once

#include "movement/move.h"

#include <array>
#include <list>
#include <vector>

namespace chessmate {

typedef std::vector<Move> MoveList;

/**
 * Class to calculate a list of all possible moves for every piece at every
 * possible location, regardless of board setup.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version September 13, 2015
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
    MoveList GetWhitePawnMoves(MoveList::size_type) const;

    /**
     * @return All possible moves a black pawn can make.
     */
    MoveList GetBlackPawnMoves(MoveList::size_type) const;

    /**
     * @return All possible moves a knight can make.
     */
    MoveList GetKnightMoves(MoveList::size_type) const;

    /**
     * @return All possible moves a bishop can make in a NE direction.
     */
    MoveList GetBishopMovesNE(MoveList::size_type) const;

    /**
     * @return All possible moves a bishop can make in a NW direction.
     */
    MoveList GetBishopMovesNW(MoveList::size_type) const;

    /**
     * @return All possible moves a bishop can make in a SE direction.
     */
    MoveList GetBishopMovesSE(MoveList::size_type) const;

    /**
     * @return All possible moves a bishop can make in a SW direction.
     */
    MoveList GetBishopMovesSW(MoveList::size_type) const;

    /**
     * @return All possible moves a rook can make in a N direction.
     */
    MoveList GetRookMovesN(MoveList::size_type) const;

    /**
     * @return All possible moves a rook can make in a S direction.
     */
    MoveList GetRookMovesS(MoveList::size_type) const;

    /**
     * @return All possible moves a rook can make in an E direction.
     */
    MoveList GetRookMovesE(MoveList::size_type) const;

    /**
     * @return All possible moves a rook can make in a W direction.
     */
    MoveList GetRookMovesW(MoveList::size_type) const;

    /**
     * @return All possible moves a queen can make in a N direction.
     */
    MoveList GetQueenMovesN(MoveList::size_type) const;

    /**
     * @return All possible moves a queen can make in a S direction.
     */
    MoveList GetQueenMovesS(MoveList::size_type) const;

    /**
     * @return All possible moves a queen can make in an E direction.
     */
    MoveList GetQueenMovesE(MoveList::size_type) const;

    /**
     * @return All possible moves a queen can make in a W direction.
     */
    MoveList GetQueenMovesW(MoveList::size_type) const;

    /**
     * @return All possible moves a queen can make in a NE direction.
     */
    MoveList GetQueenMovesNE(MoveList::size_type) const;

    /**
     * @return All possible moves a queen can make in a NW direction.
     */
    MoveList GetQueenMovesNW(MoveList::size_type) const;

    /**
     * @return All possible moves a queen can make in a SE direction.
     */
    MoveList GetQueenMovesSE(MoveList::size_type) const;

    /**
     * @return All possible moves a queen can make in a SW direction.
     */
    MoveList GetQueenMovesSW(MoveList::size_type) const;

    /**
     * @return All possible moves a king can make
     */
    MoveList GetKingMoves(MoveList::size_type) const;

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

    std::vector<MoveList> m_whitePawnMoves;
    std::vector<MoveList> m_blackPawnMoves;
    std::vector<MoveList> m_knightMoves;
    std::vector<MoveList> m_bishopMovesNE;
    std::vector<MoveList> m_bishopMovesNW;
    std::vector<MoveList> m_bishopMovesSE;
    std::vector<MoveList> m_bishopMovesSW;
    std::vector<MoveList> m_rookMovesN;
    std::vector<MoveList> m_rookMovesS;
    std::vector<MoveList> m_rookMovesE;
    std::vector<MoveList> m_rookMovesW;
    std::vector<MoveList> m_queenMovesN;
    std::vector<MoveList> m_queenMovesS;
    std::vector<MoveList> m_queenMovesE;
    std::vector<MoveList> m_queenMovesW;
    std::vector<MoveList> m_queenMovesNE;
    std::vector<MoveList> m_queenMovesNW;
    std::vector<MoveList> m_queenMovesSE;
    std::vector<MoveList> m_queenMovesSW;
    std::vector<MoveList> m_kingMoves;
};

} // namespace chessmate
