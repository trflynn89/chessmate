#include "MoveSet.h"

namespace Movement {

//=========================================================================
moveList2d_t MoveSet::m_whitePawnMoves;
moveList2d_t MoveSet::m_blackPawnMoves;
moveList2d_t MoveSet::m_knightMoves;
moveList2d_t MoveSet::m_bishopMovesNE;
moveList2d_t MoveSet::m_bishopMovesNW;
moveList2d_t MoveSet::m_bishopMovesSE;
moveList2d_t MoveSet::m_bishopMovesSW;
moveList2d_t MoveSet::m_rookMovesE;
moveList2d_t MoveSet::m_rookMovesN;
moveList2d_t MoveSet::m_rookMovesS;
moveList2d_t MoveSet::m_rookMovesW;
moveList2d_t MoveSet::m_queenMovesE;
moveList2d_t MoveSet::m_queenMovesN;
moveList2d_t MoveSet::m_queenMovesNE;
moveList2d_t MoveSet::m_queenMovesNW;
moveList2d_t MoveSet::m_queenMovesS;
moveList2d_t MoveSet::m_queenMovesSE;
moveList2d_t MoveSet::m_queenMovesSW;
moveList2d_t MoveSet::m_queenMovesW;
moveList2d_t MoveSet::m_kingMoves;

numMoves_t MoveSet::m_numWhitePawnMoves;
numMoves_t MoveSet::m_numBlackPawnMoves;
numMoves_t MoveSet::m_numKnightMoves;
numMoves_t MoveSet::m_numBishopMovesNE;
numMoves_t MoveSet::m_numBishopMovesNW;
numMoves_t MoveSet::m_numBishopMovesSE;
numMoves_t MoveSet::m_numBishopMovesSW;
numMoves_t MoveSet::m_numRookMovesN;
numMoves_t MoveSet::m_numRookMovesS;
numMoves_t MoveSet::m_numRookMovesE;
numMoves_t MoveSet::m_numRookMovesW;
numMoves_t MoveSet::m_numQueenMovesN;
numMoves_t MoveSet::m_numQueenMovesS;
numMoves_t MoveSet::m_numQueenMovesE;
numMoves_t MoveSet::m_numQueenMovesW;
numMoves_t MoveSet::m_numQueenMovesNE;
numMoves_t MoveSet::m_numQueenMovesNW;
numMoves_t MoveSet::m_numQueenMovesSE;
numMoves_t MoveSet::m_numQueenMovesSW;
numMoves_t MoveSet::m_numKingMoves;

//=========================================================================
void MoveSet::Initialize()
{
    // Set all moves to (-1, -1, -1, -1) and all sizes to 0
    for (Game::square_type i=0; i<Game::BOARD_SIZE; i++)
    {
        m_whitePawnMoves[i].fill(Move());

        m_blackPawnMoves[i].fill(Move());

        m_knightMoves[i].fill(Move());

        m_bishopMovesNE[i].fill(Move());
        m_bishopMovesNW[i].fill(Move());
        m_bishopMovesSE[i].fill(Move());
        m_bishopMovesSW[i].fill(Move());

        m_rookMovesE[i].fill(Move());
        m_rookMovesN[i].fill(Move());
        m_rookMovesS[i].fill(Move());
        m_rookMovesW[i].fill(Move());

        m_queenMovesE[i].fill(Move());
        m_queenMovesN[i].fill(Move());
        m_queenMovesNE[i].fill(Move());
        m_queenMovesNW[i].fill(Move());
        m_queenMovesS[i].fill(Move());
        m_queenMovesSE[i].fill(Move());
        m_queenMovesSW[i].fill(Move());
        m_queenMovesW[i].fill(Move());

        m_kingMoves[i].fill(Move());

        m_numWhitePawnMoves[i] = 0;
        m_numBlackPawnMoves[i] = 0;
        m_numKnightMoves[i] = 0;
        m_numBishopMovesNE[i] = 0;
        m_numBishopMovesNW[i] = 0;
        m_numBishopMovesSE[i] = 0;
        m_numBishopMovesSW[i] = 0;
        m_numRookMovesN[i] = 0;
        m_numRookMovesS[i] = 0;
        m_numRookMovesE[i] = 0;
        m_numRookMovesW[i] = 0;
        m_numQueenMovesN[i] = 0;
        m_numQueenMovesS[i] = 0;
        m_numQueenMovesE[i] = 0;
        m_numQueenMovesW[i] = 0;
        m_numQueenMovesNE[i] = 0;
        m_numQueenMovesNW[i] = 0;
        m_numQueenMovesSE[i] = 0;
        m_numQueenMovesSW[i] = 0;
        m_numKingMoves[i] = 0;
    }

    initializeWhitePawnMoves();
    initializeBlackPawnMoves();
    initializeKnightMoves();
    initializeBishopMoves();
    initializeRookMoves();
    initializeQueenMoves();
    initializeKingMoves();
}

//=========================================================================
void MoveSet::Destroy()
{
    // TODO
}

//=========================================================================
moveList2d_t MoveSet::GetWhitePawnMoves()
{
    return m_whitePawnMoves;
}

//=========================================================================
int MoveSet::GetNumWhitePawnMoves(Game::square_type i)
{
    return m_numWhitePawnMoves[i];
}

//=========================================================================
moveList2d_t MoveSet::GetBlackPawnMoves()
{
    return m_blackPawnMoves;
}

//=========================================================================
int MoveSet::GetNumBlackPawnMoves(Game::square_type i)
{
    return m_numBlackPawnMoves[i];
}

//=========================================================================
moveList2d_t MoveSet::GetKnightMoves()
{
    return m_knightMoves;
}

//=========================================================================
int MoveSet::GetNumKnightMoves(Game::square_type i)
{
    return m_numKnightMoves[i];
}

//=========================================================================
moveList2d_t MoveSet::GetBishopMovesNE()
{
    return m_bishopMovesNE;
}

//=========================================================================
int MoveSet::GetNumBishopMovesNE(Game::square_type i)
{
    return m_numBishopMovesNE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetBishopMovesNW()
{
    return m_bishopMovesNW;
}

//=========================================================================
int MoveSet::GetNumBishopMovesNW(Game::square_type i)
{
    return m_numBishopMovesNW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetBishopMovesSE()
{
    return m_bishopMovesSE;
}

//=========================================================================
int MoveSet::GetNumBishopMovesSE(Game::square_type i)
{
    return m_numBishopMovesSE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetBishopMovesSW()
{
    return m_bishopMovesSW;
}

//=========================================================================
int MoveSet::GetNumBishopMovesSW(Game::square_type i)
{
    return m_numBishopMovesSW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetRookMovesN()
{
    return m_rookMovesN;
}

//=========================================================================
int MoveSet::GetNumRookMovesN(Game::square_type i)
{
    return m_numRookMovesN[i];
}

//=========================================================================
moveList2d_t MoveSet::GetRookMovesS()
{
    return m_rookMovesS;
}

//=========================================================================
int MoveSet::GetNumRookMovesS(Game::square_type i)
{
    return m_numRookMovesS[i];
}

//=========================================================================
moveList2d_t MoveSet::GetRookMovesE()
{
    return m_rookMovesE;
}

//=========================================================================
int MoveSet::GetNumRookMovesE(Game::square_type i)
{
    return m_numRookMovesE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetRookMovesW()
{
    return m_rookMovesW;
}

//=========================================================================
int MoveSet::GetNumRookMovesW(Game::square_type i)
{
    return m_numRookMovesW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesN()
{
    return m_queenMovesN;
}

//=========================================================================
int MoveSet::GetNumQueenMovesN(Game::square_type i)
{
    return m_numQueenMovesN[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesS()
{
    return m_queenMovesS;
}

//=========================================================================
int MoveSet::GetNumQueenMovesS(Game::square_type i)
{
    return m_numQueenMovesS[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesE()
{
    return m_queenMovesE;
}

//=========================================================================
int MoveSet::GetNumQueenMovesE(Game::square_type i)
{
    return m_numQueenMovesE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesW()
{
    return m_queenMovesW;
}

//=========================================================================
int MoveSet::GetNumQueenMovesW(Game::square_type i)
{
    return m_numQueenMovesW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesNE()
{
    return m_queenMovesNE;
}

//=========================================================================
int MoveSet::GetNumQueenMovesNE(Game::square_type i)
{
    return m_numQueenMovesNE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesNW()
{
    return m_queenMovesNW;
}

//=========================================================================
int MoveSet::GetNumQueenMovesNW(Game::square_type i)
{
    return m_numQueenMovesNW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesSE()
{
    return m_queenMovesSE;
}

//=========================================================================
int MoveSet::GetNumQueenMovesSE(Game::square_type i)
{
    return m_numQueenMovesSE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesSW()
{
    return m_queenMovesSW;
}

//=========================================================================
int MoveSet::GetNumQueenMovesSW(Game::square_type i)
{
    return m_numQueenMovesSW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetKingMoves()
{
    return m_kingMoves;
}

//=========================================================================
int MoveSet::GetNumKingMoves(Game::square_type i)
{
    return m_numKingMoves[i];
}

//=========================================================================
void MoveSet::initializeWhitePawnMoves()
{
    for (Game::square_type i=8; i<56; i++)
    {
        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        moveList_t moves;
        int n = 0;

        // Diagonal capture
        if (rank < Game::RANK_8 && file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank+1, file+1);
            m_numWhitePawnMoves[i]++;
        }
        if (rank < Game::RANK_8 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank+1, file-1);
            m_numWhitePawnMoves[i]++;
        }

        // One move forward
        moves[n++] = Move(rank, file, rank+1, file);
        m_numWhitePawnMoves[i]++;

        // Two moves forward
        if (rank == Game::RANK_2)
        {
            moves[n++] = Move(rank, file, rank+2, file);
            m_numWhitePawnMoves[i]++;
        }

        m_whitePawnMoves[i] = moves;
    }
}

//=========================================================================
void MoveSet::initializeBlackPawnMoves()
{
    for (Game::square_type i=8; i<56; i++)
    {
        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        moveList_t moves;
        int n = 0;

        // Diagonal capture
        if (rank > Game::RANK_1 && file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank-1, file+1);
            m_numBlackPawnMoves[i]++;
        }
        if (rank > Game::RANK_1 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank-1, file-1);
            m_numBlackPawnMoves[i]++;
        }

        // One move forward
        moves[n++] = Move(rank, file, rank-1, file);
        m_numBlackPawnMoves[i]++;

        // Two moves forward
        if (rank == Game::RANK_7)
        {
            moves[n++] = Move(rank, file, rank-2, file);
            m_numBlackPawnMoves[i]++;
        }

        m_blackPawnMoves[i] = moves;
    }
}

//=========================================================================
void MoveSet::initializeKnightMoves()
{
    for (Game::square_type i=0; i<Game::BOARD_SIZE; i++)
    {
        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        moveList_t moves;
        int n = 0;

        // North 1, east 2
        if (rank < Game::RANK_8  && file < Game::FILE_G)
        {
            moves[n++] = Move(rank, file, rank+1, file+2);
            m_numKnightMoves[i]++;
        }

        // North 1, west 2
        if (rank < Game::RANK_8 && file > Game::FILE_B)
        {
            moves[n++] = Move(rank, file, rank+1, file-2);
            m_numKnightMoves[i]++;
        }

        // North 2, east 1
        if (rank < Game::RANK_7 && file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank+2, file+1);
            m_numKnightMoves[i]++;
        }

        // North 2, west 1
        if (rank < Game::RANK_7 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank+2, file-1);
            m_numKnightMoves[i]++;
        }

        // South 1, east 2
        if (rank > Game::RANK_1 && file < Game::FILE_G)
        {
            moves[n++] = Move(rank, file, rank-1, file+2);
            m_numKnightMoves[i]++;
        }

        // South 1, west 2
        if (rank > Game::RANK_1 && file > Game::FILE_B)
        {
            moves[n++] = Move(rank, file, rank-1, file-2);
            m_numKnightMoves[i]++;
        }

        // South 2, east 1
        if (rank > Game::RANK_2 && file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank-2, file+1);
            m_numKnightMoves[i]++;
        }

        // South 2, west 1
        if (rank > Game::RANK_2 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank-2, file-1);
            m_numKnightMoves[i]++;
        }

        m_knightMoves[i] = moves;
    }
}

//=========================================================================
void MoveSet::initializeBishopMoves()
{
    for (Game::square_type i=0; i<Game::BOARD_SIZE; i++)
    {
        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        // North-east
        Game::square_type r = rank;
        Game::square_type f = file;
        int n = 0;
        while (r < Game::RANK_8 && f < Game::FILE_H)
        {
            r++; f++;
            m_bishopMovesNE[i][n++] = Move(rank, file, r, f);
            m_numBishopMovesNE[i]++;
        }

        // North-west
        r = rank;
        f = file;
        n = 0;
        while (r < Game::RANK_8 && f > Game::FILE_A)
        {
            r++; f--;
            m_bishopMovesNW[i][n++] = Move(rank, file, r, f);
            m_numBishopMovesNW[i]++;
        }

        // South-east
        r = rank;
        f = file;
        n = 0;
        while (r > Game::RANK_1 && f < Game::FILE_H)
        {
            r--; f++;
            m_bishopMovesSE[i][n++] = Move(rank, file, r, f);
            m_numBishopMovesSE[i]++;
        }

        // South-west
        r = rank;
        f = file;
        n = 0;
        while (r > Game::RANK_1 && f > Game::FILE_A)
        {
            r--; f--;
            m_bishopMovesSW[i][n++] = Move(rank, file, r, f);
            m_numBishopMovesSW[i]++;
        }
    }
}

//=========================================================================
void MoveSet::initializeRookMoves()
{
    for (Game::square_type i=0; i<Game::BOARD_SIZE; i++)
    {
        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        // North
        Game::square_type r = rank;
        int n = 0;
        while (r < Game::RANK_8)
        {
            r++;
            m_rookMovesN[i][n++] = Move(rank, file, r, file);
            m_numRookMovesN[i]++;
        }

        // South
        r = rank;
        n = 0;
        while (r > Game::RANK_1)
        {
            r--;
            m_rookMovesS[i][n++] = Move(rank, file, r, file);
            m_numRookMovesS[i]++;
        }

        // East
        Game::square_type f = file;
        n = 0;
        while (f < Game::FILE_H)
        {
            f++;
            m_rookMovesE[i][n++] = Move(rank, file, rank, f);
            m_numRookMovesE[i]++;
        }

        // West
        f = file;
        n = 0;
        while (f > Game::FILE_A)
        {
            f--;
            m_rookMovesW[i][n++] = Move(rank, file, rank, f);
            m_numRookMovesW[i]++;
        }
    }
}

//=========================================================================
void MoveSet::initializeQueenMoves()
{
    for (Game::square_type i=0; i<Game::BOARD_SIZE; i++)
    {
        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        // North
        Game::square_type r = rank;
        int n = 0;
        while (r < Game::RANK_8)
        {
            r++;
            m_queenMovesN[i][n++] = Move(rank, file, r, file);
            m_numQueenMovesN[i]++;
        }

        // South
        r = rank;
        n = 0;
        while (r > Game::RANK_1)
        {
            r--;
            m_queenMovesS[i][n++] = Move(rank, file, r, file);
            m_numQueenMovesS[i]++;
        }

        // East
        Game::square_type f = file;
        n = 0;
        while (f < Game::FILE_H)
        {
            f++;
            m_queenMovesE[i][n++] = Move(rank, file, rank, f);
            m_numQueenMovesE[i]++;
        }

        // West
        f = file;
        n = 0;
        while (f > Game::FILE_A)
        {
            f--;
            m_queenMovesW[i][n++] = Move(rank, file, rank, f);
            m_numQueenMovesW[i]++;
        }

        // North-east
        r = rank;
        f = file;
        n = 0;
        while (r < Game::RANK_8 && f < Game::FILE_H)
        {
            r++; f++;
            m_queenMovesNE[i][n++] = Move(rank, file, r, f);
            m_numQueenMovesNE[i]++;
        }

        // North-west
        r = rank;
        f = file;
        n = 0;
        while (r < Game::RANK_8 && f > Game::FILE_A)
        {
            r++; f--;
            m_queenMovesNW[i][n++] = Move(rank, file, r, f);
            m_numQueenMovesNW[i]++;
        }

        // South-east
        r = rank;
        f = file;
        n = 0;
        while (r > Game::RANK_1 && f < Game::FILE_H)
        {
            r--; f++;
            m_queenMovesSE[i][n++] = Move(rank, file, r, f);
            m_numQueenMovesSE[i]++;
        }

        // South-west
        r = rank;
        f = file;
        n = 0;
        while (r > Game::RANK_1 && f > Game::FILE_A)
        {
            r--; f--;
            m_queenMovesSW[i][n++] = Move(rank, file, r, f);
            m_numQueenMovesSW[i]++;
        }
    }
}

//=========================================================================
void MoveSet::initializeKingMoves()
{
    for (Game::square_type i=0; i<Game::BOARD_SIZE; i++)
    {
        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);

        moveList_t moves;
        int n = 0;

        // North
        if (rank < Game::RANK_8)
        {
            moves[n++] = Move(rank, file, rank+1, file);
            m_numKingMoves[i]++;
        }

        // South
        if (rank > Game::RANK_1)
        {
            moves[n++] = Move(rank, file, rank-1, file);
            m_numKingMoves[i]++;
        }

        // East
        if (file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank, file+1);
            m_numKingMoves[i]++;
        }

        // West
        if (file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank, file-1);
            m_numKingMoves[i]++;
        }

        // North-east
        if (rank < Game::RANK_8 && file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank+1, file+1);
            m_numKingMoves[i]++;
        }

        // North-west
        if (rank < Game::RANK_8 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank+1, file-1);
            m_numKingMoves[i]++;
        }

        // South-east
        if (rank > Game::RANK_1 && file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank-1, file+1);
            m_numKingMoves[i]++;
        }

        // South-west
        if (rank > Game::RANK_1 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank-1, file-1);
            m_numKingMoves[i]++;
        }

        // Castle
        if (file == Game::FILE_E && (rank == Game::RANK_1 || rank == Game::RANK_8))
        {
            moves[n++] = Move(rank, file, rank, file+2);
            moves[n++] = Move(rank, file, rank, file-2);
            m_numKingMoves[i] += 2;
        }

        m_kingMoves[i] = moves;
    }
}

}
