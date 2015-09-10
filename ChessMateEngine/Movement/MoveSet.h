#pragma once

#include <array>

#include <Movement/Move.h>
#include <Game/BoardTypes.h>

typedef std::array<Game::value_type, Game::BOARD_SIZE> numMoves_t;

namespace Movement {

/**
 * This file calculates a list of all possible moves for every piece at every
 * possible location, regardless of board setup. The Initialize() method should
 * be called during the initialization process for the engine.
 *
 * Note: since performance is not an issue when the Initialize method is called,
 * the methods have not been very optimized. It is simply made to be correct.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version March 3, 2013
 */
class MoveSet
{
public:
    /**
     * Initialize the move set.
     */
    static void Initialize();

    /**
     * Destroy the move set.
     */
    static void Destroy();

    /**
     * @return All possible moves a white pawn can make.
     */
    static moveList2d_t GetWhitePawnMoves();

    /**
     * @return The number of moves a white pawn can make from a given square.
     */
    static int GetNumWhitePawnMoves(Game::square_type);

    /**
     * @return All possible moves a black pawn can make.
     */
    static moveList2d_t GetBlackPawnMoves();

    /**
     * @return The number of moves a black pawn can make from a given square.
     */
    static int GetNumBlackPawnMoves(Game::square_type);

    /**
     * @return All possible moves a knight can make.
     */
    static moveList2d_t GetKnightMoves();

    /**
     * @return The number of moves a knight can make from a given square.
     */
    static int GetNumKnightMoves(Game::square_type);

    /**
     * @return All possible moves a bishop can make in a NE direction.
     */
    static moveList2d_t GetBishopMovesNE();

    /**
     * @return The number of moves a bishop can make NE from a given square.
     */
    static int GetNumBishopMovesNE(Game::square_type);

    /**
     * @return All possible moves a bishop can make in a NW direction.
     */
    static moveList2d_t GetBishopMovesNW();

    /**
     * @return The number of moves a bishop can make NW from a given square.
     */
    static int GetNumBishopMovesNW(Game::square_type);

    /**
     * @return All possible moves a bishop can make in a SE direction.
     */
    static moveList2d_t GetBishopMovesSE();

    /**
     * @return The number of moves a bishop can make SE from a given square.
     */
    static int GetNumBishopMovesSE(Game::square_type);

    /**
     * @return All possible moves a bishop can make in a SW direction.
     */
    static moveList2d_t GetBishopMovesSW();

    /**
     * @return The number of moves a bishop can make SW from a given square.
     */
    static int GetNumBishopMovesSW(Game::square_type);

    /**
     * @return All possible moves a rook can make in a N direction.
     */
    static moveList2d_t GetRookMovesN();

    /**
     * @return The number of moves a rook can make N from a given square.
     */
    static int GetNumRookMovesN(Game::square_type);

    /**
     * @return All possible moves a rook can make in a S direction.
     */
    static moveList2d_t GetRookMovesS();

    /**
     * @return The number of moves a rook can make S from a given square.
     */
    static int GetNumRookMovesS(Game::square_type);

    /**
     * @return All possible moves a rook can make in an E direction.
     */
    static moveList2d_t GetRookMovesE();

    /**
     * @return The number of moves a rook can make E from a given square.
     */
    static int GetNumRookMovesE(Game::square_type);

    /**
     * @return All possible moves a rook can make in a W direction.
     */
    static moveList2d_t GetRookMovesW();

    /**
     * @return The number of moves a rook can make W from a given square.
     */
    static int GetNumRookMovesW(Game::square_type);

    /**
     * @return All possible moves a queen can make in a N direction.
     */
    static moveList2d_t GetQueenMovesN();

    /**
     * @return The number of moves a queen can make N from a given square.
     */
    static int GetNumQueenMovesN(Game::square_type);

    /**
     * @return All possible moves a queen can make in a S direction.
     */
    static moveList2d_t GetQueenMovesS();

    /**
     * @return The number of moves a queen can make S from a given square.
     */
    static int GetNumQueenMovesS(Game::square_type);

    /**
     * @return All possible moves a queen can make in an E direction.
     */
    static moveList2d_t GetQueenMovesE();

    /**
     * @return The number of moves a queen can make E from a given square.
     */
    static int GetNumQueenMovesE(Game::square_type);

    /**
     * @return All possible moves a queen can make in a W direction.
     */
    static moveList2d_t GetQueenMovesW();

    /**
     * @return The number of moves a queen can make W from a given square.
     */
    static int GetNumQueenMovesW(Game::square_type);

    /**
     * @return All possible moves a queen can make in a NE direction.
     */
    static moveList2d_t GetQueenMovesNE();

    /**
     * @return The number of moves a queen can make NE from a given square.
     */
    static int GetNumQueenMovesNE(Game::square_type);

    /**
     * @return All possible moves a queen can make in a NW direction.
     */
    static moveList2d_t GetQueenMovesNW();

    /**
     * @return The number of moves a queen can make NW from a given square.
     */
    static int GetNumQueenMovesNW(Game::square_type);

    /**
     * @return All possible moves a queen can make in a SE direction.
     */
    static moveList2d_t GetQueenMovesSE();

    /**
     * @return The number of moves a queen can make SE from a given square.
     */
    static int GetNumQueenMovesSE(Game::square_type);

    /**
     * @return All possible moves a queen can make in a SW direction.
     */
    static moveList2d_t GetQueenMovesSW();

    /**
     * @return The number of moves a queen can make SW from a given square.
     */
    static int GetNumQueenMovesSW(Game::square_type);

    /**
     * @return All possible moves a king can make
     */
    static moveList2d_t GetKingMoves();

    /**
     * @return The number of moves a king can make from a given square.
     */
    static int GetNumKingMoves(Game::square_type);

private:
    /**
     * Initialize the moves a white pawn can make.
     */
    static void initializeWhitePawnMoves();

    /**
     * Initialize the moves a black pawn can make.
     */
    static void initializeBlackPawnMoves();

    /**
     * Initialize the moves a knight can make.
     */
    static void initializeKnightMoves();

    /**
     * Initialize the moves a bishop can make.
     */
    static void initializeBishopMoves();

    /**
     * Initialize the moves a rook can make.
     */
    static void initializeRookMoves();

    /**
     * Initialize the moves a queen can make.
     */
    static void initializeQueenMoves();

    /**
     * Initialize the moves a king can make.
     */
    static void initializeKingMoves();

    // List of moves for each piece
    static moveList2d_t s_whitePawnMoves;
    static moveList2d_t s_blackPawnMoves;
    static moveList2d_t s_knightMoves;
    static moveList2d_t s_bishopMovesNE;
    static moveList2d_t s_bishopMovesNW;
    static moveList2d_t s_bishopMovesSE;
    static moveList2d_t s_bishopMovesSW;
    static moveList2d_t s_rookMovesN;
    static moveList2d_t s_rookMovesS;
    static moveList2d_t s_rookMovesE;
    static moveList2d_t s_rookMovesW;
    static moveList2d_t s_queenMovesN;
    static moveList2d_t s_queenMovesS;
    static moveList2d_t s_queenMovesE;
    static moveList2d_t s_queenMovesW;
    static moveList2d_t s_queenMovesNE;
    static moveList2d_t s_queenMovesNW;
    static moveList2d_t s_queenMovesSE;
    static moveList2d_t s_queenMovesSW;
    static moveList2d_t s_kingMoves;

    // Number of moves for each piece
    static numMoves_t s_numWhitePawnMoves;
    static numMoves_t s_numBlackPawnMoves;
    static numMoves_t s_numKnightMoves;
    static numMoves_t s_numBishopMovesNE;
    static numMoves_t s_numBishopMovesNW;
    static numMoves_t s_numBishopMovesSE;
    static numMoves_t s_numBishopMovesSW;
    static numMoves_t s_numRookMovesN;
    static numMoves_t s_numRookMovesS;
    static numMoves_t s_numRookMovesE;
    static numMoves_t s_numRookMovesW;
    static numMoves_t s_numQueenMovesN;
    static numMoves_t s_numQueenMovesS;
    static numMoves_t s_numQueenMovesE;
    static numMoves_t s_numQueenMovesW;
    static numMoves_t s_numQueenMovesNE;
    static numMoves_t s_numQueenMovesNW;
    static numMoves_t s_numQueenMovesSE;
    static numMoves_t s_numQueenMovesSW;
    static numMoves_t s_numKingMoves;
};

}
