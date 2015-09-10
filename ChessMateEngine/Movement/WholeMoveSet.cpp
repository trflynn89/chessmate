#include "WholeMoveSet.h"

#include <Game/BoardTypes.h>

namespace Movement {

//=========================================================================
WholeMoveSet::WholeMoveSet() :
    m_whitePawnMoves(Game::BOARD_SIZE),
    m_blackPawnMoves(Game::BOARD_SIZE),
    m_knightMoves(Game::BOARD_SIZE),
    m_bishopMovesNE(Game::BOARD_SIZE),
    m_bishopMovesNW(Game::BOARD_SIZE),
    m_bishopMovesSE(Game::BOARD_SIZE),
    m_bishopMovesSW(Game::BOARD_SIZE),
    m_rookMovesN(Game::BOARD_SIZE),
    m_rookMovesS(Game::BOARD_SIZE),
    m_rookMovesE(Game::BOARD_SIZE),
    m_rookMovesW(Game::BOARD_SIZE),
    m_queenMovesN(Game::BOARD_SIZE),
    m_queenMovesS(Game::BOARD_SIZE),
    m_queenMovesE(Game::BOARD_SIZE),
    m_queenMovesW(Game::BOARD_SIZE),
    m_queenMovesNE(Game::BOARD_SIZE),
    m_queenMovesNW(Game::BOARD_SIZE),
    m_queenMovesSE(Game::BOARD_SIZE),
    m_queenMovesSW(Game::BOARD_SIZE),
    m_kingMoves(Game::BOARD_SIZE)
{
    initializeWhitePawnMoves();
    initializeBlackPawnMoves();
    initializeKnightMoves();
    initializeBishopMoves();
    initializeRookMoves();
    initializeQueenMoves();
    initializeKingMoves();
}

//=========================================================================
MoveLists WholeMoveSet::GetWhitePawnMoves() const
{
    return m_whitePawnMoves;
}

//=========================================================================
MoveLists WholeMoveSet::GetBlackPawnMoves() const
{
    return m_blackPawnMoves;
}

//=========================================================================
MoveLists WholeMoveSet::GetKnightMoves() const
{
    return m_knightMoves;
}

//=========================================================================
MoveLists WholeMoveSet::GetBishopMovesNE() const
{
    return m_bishopMovesNE;
}

//=========================================================================
MoveLists WholeMoveSet::GetBishopMovesNW() const
{
    return m_bishopMovesNW;
}

//=========================================================================
MoveLists WholeMoveSet::GetBishopMovesSE() const
{
    return m_bishopMovesSE;
}

//=========================================================================
MoveLists WholeMoveSet::GetBishopMovesSW() const
{
    return m_bishopMovesSW;
}

//=========================================================================
MoveLists WholeMoveSet::GetRookMovesN() const
{
    return m_rookMovesN;
}

//=========================================================================
MoveLists WholeMoveSet::GetRookMovesS() const
{
    return m_rookMovesS;
}

//=========================================================================
MoveLists WholeMoveSet::GetRookMovesE() const
{
    return m_rookMovesE;
}

//=========================================================================
MoveLists WholeMoveSet::GetRookMovesW() const
{
    return m_rookMovesW;
}

//=========================================================================
MoveLists WholeMoveSet::GetQueenMovesN() const
{
    return m_queenMovesN;
}

//=========================================================================
MoveLists WholeMoveSet::GetQueenMovesS() const
{
    return m_queenMovesS;
}

//=========================================================================
MoveLists WholeMoveSet::GetQueenMovesE() const
{
    return m_queenMovesE;
}

//=========================================================================
MoveLists WholeMoveSet::GetQueenMovesW() const
{
    return m_queenMovesW;
}

//=========================================================================
MoveLists WholeMoveSet::GetQueenMovesNE() const
{
    return m_queenMovesNE;
}

//=========================================================================
MoveLists WholeMoveSet::GetQueenMovesNW() const
{
    return m_queenMovesNW;
}

//=========================================================================
MoveLists WholeMoveSet::GetQueenMovesSE() const
{
    return m_queenMovesSE;
}

//=========================================================================
MoveLists WholeMoveSet::GetQueenMovesSW() const
{
    return m_queenMovesSW;
}

//=========================================================================
MoveLists WholeMoveSet::GetKingMoves() const
{
    return m_kingMoves;
}

//=========================================================================
void WholeMoveSet::initializeWhitePawnMoves()
{
    for (Game::square_type i = 8; i < 56; ++i)
    {
        MoveList &moves = m_whitePawnMoves[i];

        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        // Diagonal capture
        if ((rank < Game::RANK_8) && (file < Game::FILE_H))
        {
            moves.push_back(Move(rank, file, rank+1, file+1));
        }
        if ((rank < Game::RANK_8) && (file > Game::FILE_A))
        {
            moves.push_back(Move(rank, file, rank+1, file-1));
        }

        // One move forward
        moves.push_back(Move(rank, file, rank+1, file));

        // Two moves forward
        if (rank == Game::RANK_2)
        {
            moves.push_back(Move(rank, file, rank+2, file));
        }
    }
}

//=========================================================================
void WholeMoveSet::initializeBlackPawnMoves()
{
    for (Game::square_type i = 8; i < 56; ++i)
    {
        MoveList &moves = m_blackPawnMoves[i];

        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        // Diagonal capture
        if ((rank > Game::RANK_1) && (file < Game::FILE_H))
        {
            moves.push_back(Move(rank, file, rank-1, file+1));
        }
        if ((rank > Game::RANK_1) && (file > Game::FILE_A))
        {
            moves.push_back(Move(rank, file, rank-1, file-1));
        }

        // One move forward
        moves.push_back(Move(rank, file, rank-1, file));

        // Two moves forward
        if (rank == Game::RANK_7)
        {
            moves.push_back(Move(rank, file, rank-2, file));
        }
    }
}

//=========================================================================
void WholeMoveSet::initializeKnightMoves()
{
    for (Game::square_type i = 0; i < Game::BOARD_SIZE; ++i)
    {
        MoveList &moves = m_knightMoves[i];

        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        // North 1, east 2
        if ((rank < Game::RANK_8) && (file < Game::FILE_G))
        {
            moves.push_back(Move(rank, file, rank+1, file+2));
        }

        // North 1, west 2
        if ((rank < Game::RANK_8) && (file > Game::FILE_B))
        {
            moves.push_back(Move(rank, file, rank+1, file-2));
        }

        // North 2, east 1
        if ((rank < Game::RANK_7) && (file < Game::FILE_H))
        {
            moves.push_back(Move(rank, file, rank+2, file+1));
        }

        // North 2, west 1
        if ((rank < Game::RANK_7) && (file > Game::FILE_A))
        {
            moves.push_back(Move(rank, file, rank+2, file-1));
        }

        // South 1, east 2
        if ((rank > Game::RANK_1) && (file < Game::FILE_G))
        {
            moves.push_back(Move(rank, file, rank-1, file+2));
        }

        // South 1, west 2
        if ((rank > Game::RANK_1) && (file > Game::FILE_B))
        {
            moves.push_back(Move(rank, file, rank-1, file-2));
        }

        // South 2, east 1
        if ((rank > Game::RANK_2) && (file < Game::FILE_H))
        {
            moves.push_back(Move(rank, file, rank-2, file+1));
        }

        // South 2, west 1
        if ((rank > Game::RANK_2) && (file > Game::FILE_A))
        {
            moves.push_back(Move(rank, file, rank-2, file-1));
        }
    }
}

//=========================================================================
void WholeMoveSet::initializeBishopMoves()
{
    for (Game::square_type i = 0; i < Game::BOARD_SIZE; ++i)
    {
        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        // North-east
        Game::square_type r = rank;
        Game::square_type f = file;
        while ((r < Game::RANK_8) && (f < Game::FILE_H))
        {
            m_bishopMovesNE[i].push_back(Move(rank, file, ++r, ++f));
        }

        // North-west
        r = rank;
        f = file;
        while ((r < Game::RANK_8) && (f > Game::FILE_A))
        {
            m_bishopMovesNW[i].push_back(Move(rank, file, ++r, --f));
        }

        // South-east
        r = rank;
        f = file;
        while ((r > Game::RANK_1) && (f < Game::FILE_H))
        {
            m_bishopMovesSE[i].push_back(Move(rank, file, --r, ++f));
        }

        // South-west
        r = rank;
        f = file;
        while ((r > Game::RANK_1) && (f > Game::FILE_A))
        {
            m_bishopMovesSW[i].push_back(Move(rank, file, --r, --f));
        }
    }
}

//=========================================================================
void WholeMoveSet::initializeRookMoves()
{
    for (Game::square_type i = 0; i < Game::BOARD_SIZE;++ i)
    {
        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        // North
        Game::square_type r = rank;
        while (r < Game::RANK_8)
        {
            m_rookMovesN[i].push_back(Move(rank, file, ++r, file));
        }

        // South
        r = rank;
        while (r > Game::RANK_1)
        {
            m_rookMovesS[i].push_back(Move(rank, file, --r, file));
        }

        // East
        Game::square_type f = file;
        while (f < Game::FILE_H)
        {
            m_rookMovesE[i].push_back(Move(rank, file, rank, ++f));
        }

        // West
        f = file;
        while (f > Game::FILE_A)
        {
            m_rookMovesW[i].push_back(Move(rank, file, rank, --f));
        }
    }
}

//=========================================================================
void WholeMoveSet::initializeQueenMoves()
{
    for (Game::square_type i = 0; i < Game::BOARD_SIZE;++ i)
    {
        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        // North
        Game::square_type r = rank;
        while (r < Game::RANK_8)
        {
            m_queenMovesN[i].push_back(Move(rank, file, ++r, file));
        }

        // South
        r = rank;
        while (r > Game::RANK_1)
        {
            m_queenMovesS[i].push_back(Move(rank, file, --r, file));
        }

        // East
        Game::square_type f = file;
        while (f < Game::FILE_H)
        {
            m_queenMovesE[i].push_back(Move(rank, file, rank, ++f));
        }

        // West
        f = file;
        while (f > Game::FILE_A)
        {
            m_queenMovesW[i].push_back(Move(rank, file, rank, --f));
        }

        // North-east
        r = rank;
        f = file;
        while (r < Game::RANK_8 && f < Game::FILE_H)
        {
            m_queenMovesNE[i].push_back(Move(rank, file, ++r, ++f));
        }

        // North-west
        r = rank;
        f = file;
        while (r < Game::RANK_8 && f > Game::FILE_A)
        {
            m_queenMovesNW[i].push_back(Move(rank, file, ++r, --f));
        }

        // South-east
        r = rank;
        f = file;
        while (r > Game::RANK_1 && f < Game::FILE_H)
        {
            m_queenMovesSE[i].push_back(Move(rank, file, --r, ++f));
        }

        // South-west
        r = rank;
        f = file;
        while (r > Game::RANK_1 && f > Game::FILE_A)
        {
            m_queenMovesSW[i].push_back(Move(rank, file, --r, --f));
        }
    }
}

//=========================================================================
void WholeMoveSet::initializeKingMoves()
{
    for (Game::square_type i = 0; i < Game::BOARD_SIZE; ++i)
    {
        MoveList &moves = m_kingMoves[i];

        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        // North
        if (rank < Game::RANK_8)
        {
            moves.push_back(Move(rank, file, rank+1, file));
        }

        // South
        if (rank > Game::RANK_1)
        {
            moves.push_back(Move(rank, file, rank-1, file));
        }

        // East
        if (file < Game::FILE_H)
        {
            moves.push_back(Move(rank, file, rank, file+1));
        }

        // West
        if (file > Game::FILE_A)
        {
            moves.push_back(Move(rank, file, rank, file-1));
        }

        // North-east
        if ((rank < Game::RANK_8) && (file < Game::FILE_H))
        {
            moves.push_back(Move(rank, file, rank+1, file+1));
        }

        // North-west
        if ((rank < Game::RANK_8) && (file > Game::FILE_A))
        {
            moves.push_back(Move(rank, file, rank+1, file-1));
        }

        // South-east
        if ((rank > Game::RANK_1) && (file < Game::FILE_H))
        {
            moves.push_back(Move(rank, file, rank-1, file+1));
        }

        // South-west
        if ((rank > Game::RANK_1) && (file > Game::FILE_A))
        {
            moves.push_back(Move(rank, file, rank-1, file-1));
        }

        // Castle
        if ((file == Game::FILE_E) && (rank == Game::RANK_1 || rank == Game::RANK_8))
        {
            moves.push_back(Move(rank, file, rank, file+2));
            moves.push_back(Move(rank, file, rank, file-2));
        }
    }
}

}
