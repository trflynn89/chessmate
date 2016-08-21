#include "move_set.h"

#include <game/board_types.h>

namespace chessmate {

//==============================================================================
MoveSet::MoveSet() :
    m_whitePawnMoves(BOARD_SIZE),
    m_blackPawnMoves(BOARD_SIZE),
    m_knightMoves(BOARD_SIZE),
    m_bishopMovesNE(BOARD_SIZE),
    m_bishopMovesNW(BOARD_SIZE),
    m_bishopMovesSE(BOARD_SIZE),
    m_bishopMovesSW(BOARD_SIZE),
    m_rookMovesN(BOARD_SIZE),
    m_rookMovesS(BOARD_SIZE),
    m_rookMovesE(BOARD_SIZE),
    m_rookMovesW(BOARD_SIZE),
    m_queenMovesN(BOARD_SIZE),
    m_queenMovesS(BOARD_SIZE),
    m_queenMovesE(BOARD_SIZE),
    m_queenMovesW(BOARD_SIZE),
    m_queenMovesNE(BOARD_SIZE),
    m_queenMovesNW(BOARD_SIZE),
    m_queenMovesSE(BOARD_SIZE),
    m_queenMovesSW(BOARD_SIZE),
    m_kingMoves(BOARD_SIZE)
{
    initializeWhitePawnMoves();
    initializeBlackPawnMoves();
    initializeKnightMoves();
    initializeBishopMoves();
    initializeRookMoves();
    initializeQueenMoves();
    initializeKingMoves();
}

//==============================================================================
MoveList MoveSet::GetWhitePawnMoves(int i) const
{
    return m_whitePawnMoves[i];
}

//==============================================================================
MoveList MoveSet::GetBlackPawnMoves(int i) const
{
    return m_blackPawnMoves[i];
}

//==============================================================================
MoveList MoveSet::GetKnightMoves(int i) const
{
    return m_knightMoves[i];
}

//==============================================================================
MoveList MoveSet::GetBishopMovesNE(int i) const
{
    return m_bishopMovesNE[i];
}

//==============================================================================
MoveList MoveSet::GetBishopMovesNW(int i) const
{
    return m_bishopMovesNW[i];
}

//==============================================================================
MoveList MoveSet::GetBishopMovesSE(int i) const
{
    return m_bishopMovesSE[i];
}

//==============================================================================
MoveList MoveSet::GetBishopMovesSW(int i) const
{
    return m_bishopMovesSW[i];
}

//==============================================================================
MoveList MoveSet::GetRookMovesN(int i) const
{
    return m_rookMovesN[i];
}

//==============================================================================
MoveList MoveSet::GetRookMovesS(int i) const
{
    return m_rookMovesS[i];
}

//==============================================================================
MoveList MoveSet::GetRookMovesE(int i) const
{
    return m_rookMovesE[i];
}

//==============================================================================
MoveList MoveSet::GetRookMovesW(int i) const
{
    return m_rookMovesW[i];
}

//==============================================================================
MoveList MoveSet::GetQueenMovesN(int i) const
{
    return m_queenMovesN[i];
}

//==============================================================================
MoveList MoveSet::GetQueenMovesS(int i) const
{
    return m_queenMovesS[i];
}

//==============================================================================
MoveList MoveSet::GetQueenMovesE(int i) const
{
    return m_queenMovesE[i];
}

//==============================================================================
MoveList MoveSet::GetQueenMovesW(int i) const
{
    return m_queenMovesW[i];
}

//==============================================================================
MoveList MoveSet::GetQueenMovesNE(int i) const
{
    return m_queenMovesNE[i];
}

//==============================================================================
MoveList MoveSet::GetQueenMovesNW(int i) const
{
    return m_queenMovesNW[i];
}

//==============================================================================
MoveList MoveSet::GetQueenMovesSE(int i) const
{
    return m_queenMovesSE[i];
}

//==============================================================================
MoveList MoveSet::GetQueenMovesSW(int i) const
{
    return m_queenMovesSW[i];
}

//==============================================================================
MoveList MoveSet::GetKingMoves(int i) const
{
    return m_kingMoves[i];
}

//==============================================================================
void MoveSet::initializeWhitePawnMoves()
{
    for (square_type i = 8; i < 56; ++i)
    {
        MoveList &moves = m_whitePawnMoves[i];

        square_type rank = GET_RANK(i);
        square_type file = GET_FILE(i);

        // Diagonal capture
        if ((rank < RANK_8) && (file < FILE_H))
        {
            moves.push_back(Move(rank, file, rank+1, file+1));
        }
        if ((rank < RANK_8) && (file > FILE_A))
        {
            moves.push_back(Move(rank, file, rank+1, file-1));
        }

        // One move forward
        moves.push_back(Move(rank, file, rank+1, file));

        // Two moves forward
        if (rank == RANK_2)
        {
            moves.push_back(Move(rank, file, rank+2, file));
        }
    }
}

//==============================================================================
void MoveSet::initializeBlackPawnMoves()
{
    for (square_type i = 8; i < 56; ++i)
    {
        MoveList &moves = m_blackPawnMoves[i];

        square_type rank = GET_RANK(i);
        square_type file = GET_FILE(i);

        // Diagonal capture
        if ((rank > RANK_1) && (file < FILE_H))
        {
            moves.push_back(Move(rank, file, rank-1, file+1));
        }
        if ((rank > RANK_1) && (file > FILE_A))
        {
            moves.push_back(Move(rank, file, rank-1, file-1));
        }

        // One move forward
        moves.push_back(Move(rank, file, rank-1, file));

        // Two moves forward
        if (rank == RANK_7)
        {
            moves.push_back(Move(rank, file, rank-2, file));
        }
    }
}

//==============================================================================
void MoveSet::initializeKnightMoves()
{
    for (square_type i = 0; i < BOARD_SIZE; ++i)
    {
        MoveList &moves = m_knightMoves[i];

        square_type rank = GET_RANK(i);
        square_type file = GET_FILE(i);

        // North 1, east 2
        if ((rank < RANK_8) && (file < FILE_G))
        {
            moves.push_back(Move(rank, file, rank+1, file+2));
        }

        // North 1, west 2
        if ((rank < RANK_8) && (file > FILE_B))
        {
            moves.push_back(Move(rank, file, rank+1, file-2));
        }

        // North 2, east 1
        if ((rank < RANK_7) && (file < FILE_H))
        {
            moves.push_back(Move(rank, file, rank+2, file+1));
        }

        // North 2, west 1
        if ((rank < RANK_7) && (file > FILE_A))
        {
            moves.push_back(Move(rank, file, rank+2, file-1));
        }

        // South 1, east 2
        if ((rank > RANK_1) && (file < FILE_G))
        {
            moves.push_back(Move(rank, file, rank-1, file+2));
        }

        // South 1, west 2
        if ((rank > RANK_1) && (file > FILE_B))
        {
            moves.push_back(Move(rank, file, rank-1, file-2));
        }

        // South 2, east 1
        if ((rank > RANK_2) && (file < FILE_H))
        {
            moves.push_back(Move(rank, file, rank-2, file+1));
        }

        // South 2, west 1
        if ((rank > RANK_2) && (file > FILE_A))
        {
            moves.push_back(Move(rank, file, rank-2, file-1));
        }
    }
}

//==============================================================================
void MoveSet::initializeBishopMoves()
{
    for (square_type i = 0; i < BOARD_SIZE; ++i)
    {
        square_type rank = GET_RANK(i);
        square_type file = GET_FILE(i);

        // North-east
        square_type r = rank;
        square_type f = file;
        while ((r < RANK_8) && (f < FILE_H))
        {
            m_bishopMovesNE[i].push_back(Move(rank, file, ++r, ++f));
        }

        // North-west
        r = rank;
        f = file;
        while ((r < RANK_8) && (f > FILE_A))
        {
            m_bishopMovesNW[i].push_back(Move(rank, file, ++r, --f));
        }

        // South-east
        r = rank;
        f = file;
        while ((r > RANK_1) && (f < FILE_H))
        {
            m_bishopMovesSE[i].push_back(Move(rank, file, --r, ++f));
        }

        // South-west
        r = rank;
        f = file;
        while ((r > RANK_1) && (f > FILE_A))
        {
            m_bishopMovesSW[i].push_back(Move(rank, file, --r, --f));
        }
    }
}

//==============================================================================
void MoveSet::initializeRookMoves()
{
    for (square_type i = 0; i < BOARD_SIZE;++ i)
    {
        square_type rank = GET_RANK(i);
        square_type file = GET_FILE(i);

        // North
        square_type r = rank;
        while (r < RANK_8)
        {
            m_rookMovesN[i].push_back(Move(rank, file, ++r, file));
        }

        // South
        r = rank;
        while (r > RANK_1)
        {
            m_rookMovesS[i].push_back(Move(rank, file, --r, file));
        }

        // East
        square_type f = file;
        while (f < FILE_H)
        {
            m_rookMovesE[i].push_back(Move(rank, file, rank, ++f));
        }

        // West
        f = file;
        while (f > FILE_A)
        {
            m_rookMovesW[i].push_back(Move(rank, file, rank, --f));
        }
    }
}

//==============================================================================
void MoveSet::initializeQueenMoves()
{
    for (square_type i = 0; i < BOARD_SIZE;++ i)
    {
        square_type rank = GET_RANK(i);
        square_type file = GET_FILE(i);

        // North
        square_type r = rank;
        while (r < RANK_8)
        {
            m_queenMovesN[i].push_back(Move(rank, file, ++r, file));
        }

        // South
        r = rank;
        while (r > RANK_1)
        {
            m_queenMovesS[i].push_back(Move(rank, file, --r, file));
        }

        // East
        square_type f = file;
        while (f < FILE_H)
        {
            m_queenMovesE[i].push_back(Move(rank, file, rank, ++f));
        }

        // West
        f = file;
        while (f > FILE_A)
        {
            m_queenMovesW[i].push_back(Move(rank, file, rank, --f));
        }

        // North-east
        r = rank;
        f = file;
        while (r < RANK_8 && f < FILE_H)
        {
            m_queenMovesNE[i].push_back(Move(rank, file, ++r, ++f));
        }

        // North-west
        r = rank;
        f = file;
        while (r < RANK_8 && f > FILE_A)
        {
            m_queenMovesNW[i].push_back(Move(rank, file, ++r, --f));
        }

        // South-east
        r = rank;
        f = file;
        while (r > RANK_1 && f < FILE_H)
        {
            m_queenMovesSE[i].push_back(Move(rank, file, --r, ++f));
        }

        // South-west
        r = rank;
        f = file;
        while (r > RANK_1 && f > FILE_A)
        {
            m_queenMovesSW[i].push_back(Move(rank, file, --r, --f));
        }
    }
}

//==============================================================================
void MoveSet::initializeKingMoves()
{
    for (square_type i = 0; i < BOARD_SIZE; ++i)
    {
        MoveList &moves = m_kingMoves[i];

        square_type rank = GET_RANK(i);
        square_type file = GET_FILE(i);

        // North
        if (rank < RANK_8)
        {
            moves.push_back(Move(rank, file, rank+1, file));
        }

        // South
        if (rank > RANK_1)
        {
            moves.push_back(Move(rank, file, rank-1, file));
        }

        // East
        if (file < FILE_H)
        {
            moves.push_back(Move(rank, file, rank, file+1));
        }

        // West
        if (file > FILE_A)
        {
            moves.push_back(Move(rank, file, rank, file-1));
        }

        // North-east
        if ((rank < RANK_8) && (file < FILE_H))
        {
            moves.push_back(Move(rank, file, rank+1, file+1));
        }

        // North-west
        if ((rank < RANK_8) && (file > FILE_A))
        {
            moves.push_back(Move(rank, file, rank+1, file-1));
        }

        // South-east
        if ((rank > RANK_1) && (file < FILE_H))
        {
            moves.push_back(Move(rank, file, rank-1, file+1));
        }

        // South-west
        if ((rank > RANK_1) && (file > FILE_A))
        {
            moves.push_back(Move(rank, file, rank-1, file-1));
        }

        // Castle
        if ((file == FILE_E) && (rank == RANK_1 || rank == RANK_8))
        {
            moves.push_back(Move(rank, file, rank, file+2));
            moves.push_back(Move(rank, file, rank, file-2));
        }
    }
}

}
