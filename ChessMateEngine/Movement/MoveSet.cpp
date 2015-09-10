#include "MoveSet.h"

namespace Movement {

//=========================================================================
moveList2d_t MoveSet::s_whitePawnMoves;
moveList2d_t MoveSet::s_blackPawnMoves;
moveList2d_t MoveSet::s_knightMoves;
moveList2d_t MoveSet::s_bishopMovesNE;
moveList2d_t MoveSet::s_bishopMovesNW;
moveList2d_t MoveSet::s_bishopMovesSE;
moveList2d_t MoveSet::s_bishopMovesSW;
moveList2d_t MoveSet::s_rookMovesE;
moveList2d_t MoveSet::s_rookMovesN;
moveList2d_t MoveSet::s_rookMovesS;
moveList2d_t MoveSet::s_rookMovesW;
moveList2d_t MoveSet::s_queenMovesE;
moveList2d_t MoveSet::s_queenMovesN;
moveList2d_t MoveSet::s_queenMovesNE;
moveList2d_t MoveSet::s_queenMovesNW;
moveList2d_t MoveSet::s_queenMovesS;
moveList2d_t MoveSet::s_queenMovesSE;
moveList2d_t MoveSet::s_queenMovesSW;
moveList2d_t MoveSet::s_queenMovesW;
moveList2d_t MoveSet::s_kingMoves;

numMoves_t MoveSet::s_numWhitePawnMoves;
numMoves_t MoveSet::s_numBlackPawnMoves;
numMoves_t MoveSet::s_numKnightMoves;
numMoves_t MoveSet::s_numBishopMovesNE;
numMoves_t MoveSet::s_numBishopMovesNW;
numMoves_t MoveSet::s_numBishopMovesSE;
numMoves_t MoveSet::s_numBishopMovesSW;
numMoves_t MoveSet::s_numRookMovesN;
numMoves_t MoveSet::s_numRookMovesS;
numMoves_t MoveSet::s_numRookMovesE;
numMoves_t MoveSet::s_numRookMovesW;
numMoves_t MoveSet::s_numQueenMovesN;
numMoves_t MoveSet::s_numQueenMovesS;
numMoves_t MoveSet::s_numQueenMovesE;
numMoves_t MoveSet::s_numQueenMovesW;
numMoves_t MoveSet::s_numQueenMovesNE;
numMoves_t MoveSet::s_numQueenMovesNW;
numMoves_t MoveSet::s_numQueenMovesSE;
numMoves_t MoveSet::s_numQueenMovesSW;
numMoves_t MoveSet::s_numKingMoves;

//=========================================================================
void MoveSet::Initialize()
{
    // Set all moves to (-1, -1, -1, -1) and all sizes to 0
    for (Game::square_type i=0; i<Game::BOARD_SIZE; i++)
    {
        s_whitePawnMoves[i].fill(Move());

        s_blackPawnMoves[i].fill(Move());

        s_knightMoves[i].fill(Move());

        s_bishopMovesNE[i].fill(Move());
        s_bishopMovesNW[i].fill(Move());
        s_bishopMovesSE[i].fill(Move());
        s_bishopMovesSW[i].fill(Move());

        s_rookMovesE[i].fill(Move());
        s_rookMovesN[i].fill(Move());
        s_rookMovesS[i].fill(Move());
        s_rookMovesW[i].fill(Move());

        s_queenMovesE[i].fill(Move());
        s_queenMovesN[i].fill(Move());
        s_queenMovesNE[i].fill(Move());
        s_queenMovesNW[i].fill(Move());
        s_queenMovesS[i].fill(Move());
        s_queenMovesSE[i].fill(Move());
        s_queenMovesSW[i].fill(Move());
        s_queenMovesW[i].fill(Move());

        s_kingMoves[i].fill(Move());

        s_numWhitePawnMoves[i] = 0;
        s_numBlackPawnMoves[i] = 0;
        s_numKnightMoves[i] = 0;
        s_numBishopMovesNE[i] = 0;
        s_numBishopMovesNW[i] = 0;
        s_numBishopMovesSE[i] = 0;
        s_numBishopMovesSW[i] = 0;
        s_numRookMovesN[i] = 0;
        s_numRookMovesS[i] = 0;
        s_numRookMovesE[i] = 0;
        s_numRookMovesW[i] = 0;
        s_numQueenMovesN[i] = 0;
        s_numQueenMovesS[i] = 0;
        s_numQueenMovesE[i] = 0;
        s_numQueenMovesW[i] = 0;
        s_numQueenMovesNE[i] = 0;
        s_numQueenMovesNW[i] = 0;
        s_numQueenMovesSE[i] = 0;
        s_numQueenMovesSW[i] = 0;
        s_numKingMoves[i] = 0;
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
    return s_whitePawnMoves;
}

//=========================================================================
int MoveSet::GetNumWhitePawnMoves(Game::square_type i)
{
    return s_numWhitePawnMoves[i];
}

//=========================================================================
moveList2d_t MoveSet::GetBlackPawnMoves()
{
    return s_blackPawnMoves;
}

//=========================================================================
int MoveSet::GetNumBlackPawnMoves(Game::square_type i)
{
    return s_numBlackPawnMoves[i];
}

//=========================================================================
moveList2d_t MoveSet::GetKnightMoves()
{
    return s_knightMoves;
}

//=========================================================================
int MoveSet::GetNumKnightMoves(Game::square_type i)
{
    return s_numKnightMoves[i];
}

//=========================================================================
moveList2d_t MoveSet::GetBishopMovesNE()
{
    return s_bishopMovesNE;
}

//=========================================================================
int MoveSet::GetNumBishopMovesNE(Game::square_type i)
{
    return s_numBishopMovesNE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetBishopMovesNW()
{
    return s_bishopMovesNW;
}

//=========================================================================
int MoveSet::GetNumBishopMovesNW(Game::square_type i)
{
    return s_numBishopMovesNW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetBishopMovesSE()
{
    return s_bishopMovesSE;
}

//=========================================================================
int MoveSet::GetNumBishopMovesSE(Game::square_type i)
{
    return s_numBishopMovesSE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetBishopMovesSW()
{
    return s_bishopMovesSW;
}

//=========================================================================
int MoveSet::GetNumBishopMovesSW(Game::square_type i)
{
    return s_numBishopMovesSW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetRookMovesN()
{
    return s_rookMovesN;
}

//=========================================================================
int MoveSet::GetNumRookMovesN(Game::square_type i)
{
    return s_numRookMovesN[i];
}

//=========================================================================
moveList2d_t MoveSet::GetRookMovesS()
{
    return s_rookMovesS;
}

//=========================================================================
int MoveSet::GetNumRookMovesS(Game::square_type i)
{
    return s_numRookMovesS[i];
}

//=========================================================================
moveList2d_t MoveSet::GetRookMovesE()
{
    return s_rookMovesE;
}

//=========================================================================
int MoveSet::GetNumRookMovesE(Game::square_type i)
{
    return s_numRookMovesE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetRookMovesW()
{
    return s_rookMovesW;
}

//=========================================================================
int MoveSet::GetNumRookMovesW(Game::square_type i)
{
    return s_numRookMovesW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesN()
{
    return s_queenMovesN;
}

//=========================================================================
int MoveSet::GetNumQueenMovesN(Game::square_type i)
{
    return s_numQueenMovesN[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesS()
{
    return s_queenMovesS;
}

//=========================================================================
int MoveSet::GetNumQueenMovesS(Game::square_type i)
{
    return s_numQueenMovesS[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesE()
{
    return s_queenMovesE;
}

//=========================================================================
int MoveSet::GetNumQueenMovesE(Game::square_type i)
{
    return s_numQueenMovesE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesW()
{
    return s_queenMovesW;
}

//=========================================================================
int MoveSet::GetNumQueenMovesW(Game::square_type i)
{
    return s_numQueenMovesW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesNE()
{
    return s_queenMovesNE;
}

//=========================================================================
int MoveSet::GetNumQueenMovesNE(Game::square_type i)
{
    return s_numQueenMovesNE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesNW()
{
    return s_queenMovesNW;
}

//=========================================================================
int MoveSet::GetNumQueenMovesNW(Game::square_type i)
{
    return s_numQueenMovesNW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesSE()
{
    return s_queenMovesSE;
}

//=========================================================================
int MoveSet::GetNumQueenMovesSE(Game::square_type i)
{
    return s_numQueenMovesSE[i];
}

//=========================================================================
moveList2d_t MoveSet::GetQueenMovesSW()
{
    return s_queenMovesSW;
}

//=========================================================================
int MoveSet::GetNumQueenMovesSW(Game::square_type i)
{
    return s_numQueenMovesSW[i];
}

//=========================================================================
moveList2d_t MoveSet::GetKingMoves()
{
    return s_kingMoves;
}

//=========================================================================
int MoveSet::GetNumKingMoves(Game::square_type i)
{
    return s_numKingMoves[i];
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
            s_numWhitePawnMoves[i]++;
        }
        if (rank < Game::RANK_8 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank+1, file-1);
            s_numWhitePawnMoves[i]++;
        }

        // One move forward
        moves[n++] = Move(rank, file, rank+1, file);
        s_numWhitePawnMoves[i]++;

        // Two moves forward
        if (rank == Game::RANK_2)
        {
            moves[n++] = Move(rank, file, rank+2, file);
            s_numWhitePawnMoves[i]++;
        }

        s_whitePawnMoves[i] = moves;
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
            s_numBlackPawnMoves[i]++;
        }
        if (rank > Game::RANK_1 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank-1, file-1);
            s_numBlackPawnMoves[i]++;
        }

        // One move forward
        moves[n++] = Move(rank, file, rank-1, file);
        s_numBlackPawnMoves[i]++;

        // Two moves forward
        if (rank == Game::RANK_7)
        {
            moves[n++] = Move(rank, file, rank-2, file);
            s_numBlackPawnMoves[i]++;
        }

        s_blackPawnMoves[i] = moves;
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
            s_numKnightMoves[i]++;
        }

        // North 1, west 2
        if (rank < Game::RANK_8 && file > Game::FILE_B)
        {
            moves[n++] = Move(rank, file, rank+1, file-2);
            s_numKnightMoves[i]++;
        }

        // North 2, east 1
        if (rank < Game::RANK_7 && file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank+2, file+1);
            s_numKnightMoves[i]++;
        }

        // North 2, west 1
        if (rank < Game::RANK_7 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank+2, file-1);
            s_numKnightMoves[i]++;
        }

        // South 1, east 2
        if (rank > Game::RANK_1 && file < Game::FILE_G)
        {
            moves[n++] = Move(rank, file, rank-1, file+2);
            s_numKnightMoves[i]++;
        }

        // South 1, west 2
        if (rank > Game::RANK_1 && file > Game::FILE_B)
        {
            moves[n++] = Move(rank, file, rank-1, file-2);
            s_numKnightMoves[i]++;
        }

        // South 2, east 1
        if (rank > Game::RANK_2 && file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank-2, file+1);
            s_numKnightMoves[i]++;
        }

        // South 2, west 1
        if (rank > Game::RANK_2 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank-2, file-1);
            s_numKnightMoves[i]++;
        }

        s_knightMoves[i] = moves;
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
            s_bishopMovesNE[i][n++] = Move(rank, file, r, f);
            s_numBishopMovesNE[i]++;
        }

        // North-west
        r = rank;
        f = file;
        n = 0;
        while (r < Game::RANK_8 && f > Game::FILE_A)
        {
            r++; f--;
            s_bishopMovesNW[i][n++] = Move(rank, file, r, f);
            s_numBishopMovesNW[i]++;
        }

        // South-east
        r = rank;
        f = file;
        n = 0;
        while (r > Game::RANK_1 && f < Game::FILE_H)
        {
            r--; f++;
            s_bishopMovesSE[i][n++] = Move(rank, file, r, f);
            s_numBishopMovesSE[i]++;
        }

        // South-west
        r = rank;
        f = file;
        n = 0;
        while (r > Game::RANK_1 && f > Game::FILE_A)
        {
            r--; f--;
            s_bishopMovesSW[i][n++] = Move(rank, file, r, f);
            s_numBishopMovesSW[i]++;
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
            s_rookMovesN[i][n++] = Move(rank, file, r, file);
            s_numRookMovesN[i]++;
        }

        // South
        r = rank;
        n = 0;
        while (r > Game::RANK_1)
        {
            r--;
            s_rookMovesS[i][n++] = Move(rank, file, r, file);
            s_numRookMovesS[i]++;
        }

        // East
        Game::square_type f = file;
        n = 0;
        while (f < Game::FILE_H)
        {
            f++;
            s_rookMovesE[i][n++] = Move(rank, file, rank, f);
            s_numRookMovesE[i]++;
        }

        // West
        f = file;
        n = 0;
        while (f > Game::FILE_A)
        {
            f--;
            s_rookMovesW[i][n++] = Move(rank, file, rank, f);
            s_numRookMovesW[i]++;
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
            s_queenMovesN[i][n++] = Move(rank, file, r, file);
            s_numQueenMovesN[i]++;
        }

        // South
        r = rank;
        n = 0;
        while (r > Game::RANK_1)
        {
            r--;
            s_queenMovesS[i][n++] = Move(rank, file, r, file);
            s_numQueenMovesS[i]++;
        }

        // East
        Game::square_type f = file;
        n = 0;
        while (f < Game::FILE_H)
        {
            f++;
            s_queenMovesE[i][n++] = Move(rank, file, rank, f);
            s_numQueenMovesE[i]++;
        }

        // West
        f = file;
        n = 0;
        while (f > Game::FILE_A)
        {
            f--;
            s_queenMovesW[i][n++] = Move(rank, file, rank, f);
            s_numQueenMovesW[i]++;
        }

        // North-east
        r = rank;
        f = file;
        n = 0;
        while (r < Game::RANK_8 && f < Game::FILE_H)
        {
            r++; f++;
            s_queenMovesNE[i][n++] = Move(rank, file, r, f);
            s_numQueenMovesNE[i]++;
        }

        // North-west
        r = rank;
        f = file;
        n = 0;
        while (r < Game::RANK_8 && f > Game::FILE_A)
        {
            r++; f--;
            s_queenMovesNW[i][n++] = Move(rank, file, r, f);
            s_numQueenMovesNW[i]++;
        }

        // South-east
        r = rank;
        f = file;
        n = 0;
        while (r > Game::RANK_1 && f < Game::FILE_H)
        {
            r--; f++;
            s_queenMovesSE[i][n++] = Move(rank, file, r, f);
            s_numQueenMovesSE[i]++;
        }

        // South-west
        r = rank;
        f = file;
        n = 0;
        while (r > Game::RANK_1 && f > Game::FILE_A)
        {
            r--; f--;
            s_queenMovesSW[i][n++] = Move(rank, file, r, f);
            s_numQueenMovesSW[i]++;
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
            s_numKingMoves[i]++;
        }

        // South
        if (rank > Game::RANK_1)
        {
            moves[n++] = Move(rank, file, rank-1, file);
            s_numKingMoves[i]++;
        }

        // East
        if (file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank, file+1);
            s_numKingMoves[i]++;
        }

        // West
        if (file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank, file-1);
            s_numKingMoves[i]++;
        }

        // North-east
        if (rank < Game::RANK_8 && file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank+1, file+1);
            s_numKingMoves[i]++;
        }

        // North-west
        if (rank < Game::RANK_8 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank+1, file-1);
            s_numKingMoves[i]++;
        }

        // South-east
        if (rank > Game::RANK_1 && file < Game::FILE_H)
        {
            moves[n++] = Move(rank, file, rank-1, file+1);
            s_numKingMoves[i]++;
        }

        // South-west
        if (rank > Game::RANK_1 && file > Game::FILE_A)
        {
            moves[n++] = Move(rank, file, rank-1, file-1);
            s_numKingMoves[i]++;
        }

        // Castle
        if (file == Game::FILE_E && (rank == Game::RANK_1 || rank == Game::RANK_8))
        {
            moves[n++] = Move(rank, file, rank, file+2);
            moves[n++] = Move(rank, file, rank, file-2);
            s_numKingMoves[i] += 2;
        }

        s_kingMoves[i] = moves;
    }
}

}
