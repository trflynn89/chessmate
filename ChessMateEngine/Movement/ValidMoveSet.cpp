#include "ValidMoveSet.h"

namespace Movement {

namespace
{
    // Attacking piece values - better to attack with pawn than queen
    const Game::value_type m_pawnValue   = 6;
    const Game::value_type m_knightValue = 3;
    const Game::value_type m_bishopValue = 3;
    const Game::value_type m_rookValue   = 2;
    const Game::value_type m_queenValue  = 1;
    const Game::value_type m_kingValue   = 1;
}

//=============================================================================
ValidMoveSet::ValidMoveSet(const Game::BitBoardPtr &spBoard) :
    m_numMyValidMoves(0),
    m_numOppValidMoves(0)
{
    for (Game::square_type i = 0; i < Game::BOARD_SIZE; ++i)
    {
        m_attackValue[i] = 0;
        m_defendValue[i] = 0;
    }

    std::fill(m_myValidMoves.begin(), m_myValidMoves.end(), Move());
    std::fill(m_oppValidMoves.begin(), m_oppValidMoves.end(), Move());

    if (spBoard)
    {
        generateValidMoves(spBoard);
    }
}

//=============================================================================
void ValidMoveSet::generateValidMoves(const Game::BitBoardPtr &spBoard)
{
    Game::color_type playerInTurn = spBoard->GetPlayerInTurn();

    moveList2d_t wPawn = MoveSet::GetWhitePawnMoves();
    moveList2d_t bPawn = MoveSet::GetBlackPawnMoves();
    moveList2d_t knight = MoveSet::GetKnightMoves();
    moveList2d_t king = MoveSet::GetKingMoves();

    for (Game::square_type i=0; i<Game::BOARD_SIZE; i++)
    {
        Game::square_type rank = GET_RANK(i);
        Game::square_type file = GET_FILE(i);
        Game::color_type c = (spBoard->IsWhite(rank, file) ? Game::WHITE : Game::BLACK);

        /***** EMPTY *****/

        if (spBoard->IsEmpty(rank, file))
        {
            continue;
        }

        /***** PAWN *****/

        else if (spBoard->IsPawn(rank, file))
        {

            // WHITE pawn
            if (c == Game::WHITE)
            {
                moveList_t moves = wPawn[i];

                for (Game::value_type j=0; j<MoveSet::GetNumWhitePawnMoves(i); j++)
                {
                    Move m = moves[j];

                    // Straight
                    if (m.GetStartFile() == m.GetEndFile())
                    {
                        // 2 move start
                        if (m.GetEndRank() - m.GetStartRank() == 2)
                        {
                            if (spBoard->IsEmpty(m.GetStartRank()+1, m.GetStartFile()))
                            {
                                if (spBoard->IsEmpty(m.GetEndRank(), m.GetEndFile()))
                                {
                                    m.SetMovingPiece(Game::PAWN);

                                    if (playerInTurn == Game::WHITE)
                                    {
                                        m_myValidMoves[m_numMyValidMoves++] = m;
                                    }
                                    else
                                    {
                                        m_oppValidMoves[m_numOppValidMoves++] = m;
                                    }
                                }
                            }
                        }

                        // Single move forward
                        else if (spBoard->IsEmpty(m.GetEndRank(), m.GetEndFile()))
                        {
                            m.SetMovingPiece(Game::PAWN);

                            if (playerInTurn == Game::WHITE)
                            {
                                m_myValidMoves[m_numMyValidMoves++] = m;
                            }
                            else
                            {
                                m_oppValidMoves[m_numOppValidMoves++] = m;
                            }
                        }
                    }

                    // Attack diagonally
                    else
                    {
                        if (playerInTurn == Game::WHITE)
                        {
                            m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
                                += m_pawnValue;
                        }
                        else
                        {
                            m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
                                += m_pawnValue;
                        }

                        if (spBoard->IsBlack(m.GetEndRank(), m.GetEndFile()))
                        {
                            m.SetCapture();
                            m.SetMovingPiece(Game::PAWN);

                            if (playerInTurn == Game::WHITE)
                            {
                                m_myValidMoves[m_numMyValidMoves++] = m;
                            }
                            else
                            {
                                m_oppValidMoves[m_numOppValidMoves++] = m;
                            }
                        }

                        // En passant
                        else if (spBoard->GetEnPassantColor() == Game::BLACK)
                        {
                            Game::square_type pos = spBoard->GetEnPassantPosition();
                            Game::square_type r = GET_RANK(pos);
                            Game::square_type f = GET_FILE(pos);

                            if (r == m.GetEndRank() && f == m.GetEndFile())
                            {
                                m.SetCapture();
                                m.SetEnPassant();
                                m.SetMovingPiece(Game::PAWN);

                                if (playerInTurn == Game::WHITE)
                                {
                                    m_myValidMoves[m_numMyValidMoves++] = m;
                                }
                                else
                                {
                                    m_oppValidMoves[m_numOppValidMoves++] = m;
                                }
                            }
                        }
                    }
                }
            }

            // BLACK pawn
            else
            {
                moveList_t moves = bPawn[i];

                for (Game::value_type j=0; j<MoveSet::GetNumBlackPawnMoves(i); j++)
                {
                    Move m = moves[j];

                    // Straight
                    if (m.GetStartFile() == m.GetEndFile())
                    {
                        // 2 move start
                        if (m.GetStartRank() - m.GetEndRank() == 2)
                        {
                            if (spBoard->IsEmpty(m.GetStartRank()-1, m.GetStartFile()))
                            {
                                if (spBoard->IsEmpty(m.GetEndRank(), m.GetEndFile()))
                                {
                                    m.SetMovingPiece(Game::PAWN);

                                    if (playerInTurn == Game::BLACK)
                                    {
                                        m_myValidMoves[m_numMyValidMoves++] = m;
                                    }
                                    else
                                    {
                                        m_oppValidMoves[m_numOppValidMoves++] = m;
                                    }
                                }
                            }
                        }

                        // Single move forward
                        else if (spBoard->IsEmpty(m.GetEndRank(), m.GetEndFile()))
                        {
                            m.SetMovingPiece(Game::PAWN);

                            if (playerInTurn == Game::BLACK)
                            {
                                m_myValidMoves[m_numMyValidMoves++] = m;
                            }
                            else
                            {
                                m_oppValidMoves[m_numOppValidMoves++] = m;
                            }
                        }
                    }

                    // Attack diagonally
                    else
                    {
                        if (playerInTurn == Game::BLACK)
                        {
                            m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
                                += m_pawnValue;
                        }
                        else
                        {
                            m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
                                += m_pawnValue;
                        }

                        if (spBoard->IsWhite(m.GetEndRank(), m.GetEndFile()))
                        {
                            m.SetCapture();
                            m.SetMovingPiece(Game::PAWN);

                            if (playerInTurn == Game::BLACK)
                            {
                                m_myValidMoves[m_numMyValidMoves++] = m;
                            }
                            else
                            {
                                m_oppValidMoves[m_numOppValidMoves++] = m;
                            }
                        }

                        // En passant
                        else if (spBoard->GetEnPassantColor() == Game::WHITE)
                        {
                            Game::square_type pos = spBoard->GetEnPassantPosition();
                            Game::square_type r = GET_RANK(pos);
                            Game::square_type f = GET_FILE(pos);

                            if (r == m.GetEndRank() && f == m.GetEndFile())
                            {
                                m.SetCapture();
                                m.SetEnPassant();
                                m.SetMovingPiece(Game::PAWN);

                                if (playerInTurn == Game::BLACK)
                                {
                                    m_myValidMoves[m_numMyValidMoves++] = m;
                                }
                                else
                                {
                                    m_oppValidMoves[m_numOppValidMoves++] = m;
                                }
                            }
                        }
                    }
                }
            }
        }

        /***** KNIGHT *****/

        if (spBoard->IsKnight(rank, file))
        {
            moveList_t moves = knight[i];

            if (playerInTurn == c)
            {
                for (Game::value_type j=0; j<MoveSet::GetNumKnightMoves(i); j++)
                {
                    Move m = moves[j];

                    if (spBoard->IsEmpty(m.GetEndRank(), m.GetEndFile()))
                    {
                        m.SetMovingPiece(Game::KNIGHT);

                        m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
                            += m_knightValue;
                        m_myValidMoves[m_numMyValidMoves++] = m;
                    }
                    else
                    {
                        Game::color_type c2 = (spBoard->IsWhite(m.GetEndRank(), m.GetEndFile())
                            ? Game::WHITE : Game::BLACK);
                        if (c != c2)
                        {
                            m.SetCapture();
                            m.SetMovingPiece(Game::KNIGHT);

                            m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
                                += m_knightValue;
                            m_myValidMoves[m_numMyValidMoves++] = m;
                        }
                    }
                }
            }
            else
            {
                for (Game::value_type j=0; j<MoveSet::GetNumKnightMoves(i); j++)
                {
                    Move m = moves[j];

                    if (spBoard->IsEmpty(m.GetEndRank(), m.GetEndFile()))
                    {
                        m.SetMovingPiece(Game::KNIGHT);

                        m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
                            += m_knightValue;
                        m_oppValidMoves[m_numOppValidMoves++] = m;
                    }
                    else
                    {
                        Game::color_type c2 = (spBoard->IsWhite(m.GetEndRank(), m.GetEndFile())
                            ? Game::WHITE : Game::BLACK);
                        if (c != c2)
                        {
                            m.SetCapture();
                            m.SetMovingPiece(Game::KNIGHT);

                            m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())]
                                += m_knightValue;
                            m_oppValidMoves[m_numOppValidMoves++] = m;
                        }
                    }
                }
            }
        }

        /***** BISHOP *****/

        if (spBoard->IsBishop(rank, file))
        {
            addSlidingMoveIfValid(spBoard, Game::BISHOP, c, m_bishopValue,
                MoveSet::GetBishopMovesNE()[i], MoveSet::GetNumBishopMovesNE(i));
            addSlidingMoveIfValid(spBoard, Game::BISHOP, c, m_bishopValue,
                MoveSet::GetBishopMovesNW()[i], MoveSet::GetNumBishopMovesNW(i));
            addSlidingMoveIfValid(spBoard, Game::BISHOP, c, m_bishopValue,
                MoveSet::GetBishopMovesSE()[i], MoveSet::GetNumBishopMovesSE(i));
            addSlidingMoveIfValid(spBoard, Game::BISHOP, c, m_bishopValue,
                MoveSet::GetBishopMovesSW()[i], MoveSet::GetNumBishopMovesSW(i));
        }

        /***** ROOK *****/

        else if (spBoard->IsRook(rank, file))
        {
            addSlidingMoveIfValid(spBoard, Game::ROOK, c, m_rookValue,
                MoveSet::GetRookMovesN()[i], MoveSet::GetNumRookMovesN(i));
            addSlidingMoveIfValid(spBoard, Game::ROOK, c, m_rookValue,
                MoveSet::GetRookMovesS()[i], MoveSet::GetNumRookMovesS(i));
            addSlidingMoveIfValid(spBoard, Game::ROOK, c, m_rookValue,
                MoveSet::GetRookMovesE()[i], MoveSet::GetNumRookMovesE(i));
            addSlidingMoveIfValid(spBoard, Game::ROOK, c, m_rookValue,
                MoveSet::GetRookMovesW()[i], MoveSet::GetNumRookMovesW(i));
        }

        /***** QUEEN *****/

        else if (spBoard->IsQueen(rank, file))
        {
            addSlidingMoveIfValid(spBoard, Game::QUEEN, c, m_queenValue,
                MoveSet::GetQueenMovesN()[i], MoveSet::GetNumQueenMovesN(i));
            addSlidingMoveIfValid(spBoard, Game::QUEEN, c, m_queenValue,
                MoveSet::GetQueenMovesS()[i], MoveSet::GetNumQueenMovesS(i));
            addSlidingMoveIfValid(spBoard, Game::QUEEN, c, m_queenValue,
                MoveSet::GetQueenMovesE()[i], MoveSet::GetNumQueenMovesE(i));
            addSlidingMoveIfValid(spBoard, Game::QUEEN, c, m_queenValue,
                MoveSet::GetQueenMovesW()[i], MoveSet::GetNumQueenMovesW(i));
            addSlidingMoveIfValid(spBoard, Game::QUEEN, c, m_queenValue,
                MoveSet::GetQueenMovesNE()[i], MoveSet::GetNumQueenMovesNE(i));
            addSlidingMoveIfValid(spBoard, Game::QUEEN, c, m_queenValue,
                MoveSet::GetQueenMovesNW()[i], MoveSet::GetNumQueenMovesNW(i));
            addSlidingMoveIfValid(spBoard, Game::QUEEN, c, m_queenValue,
                MoveSet::GetQueenMovesSE()[i], MoveSet::GetNumQueenMovesSE(i));
            addSlidingMoveIfValid(spBoard, Game::QUEEN, c, m_queenValue,
                MoveSet::GetQueenMovesSW()[i], MoveSet::GetNumQueenMovesSW(i));
        }

        /***** KING *****/

        if (spBoard->IsKing(rank, file))
        {
            Game::color_type c = (spBoard->IsWhite(rank, file) ? Game::WHITE : Game::BLACK);
            Game::color_type oppC = (c == Game::WHITE ? Game::BLACK : Game::WHITE);

            if (c == spBoard->GetPlayerInTurn())
            {
                moveList_t moves = king[i];

                for (Game::value_type j=0; j<MoveSet::GetNumKingMoves(i); j++)
                {
                    Move m = moves[j];

                    bool movedKing = (c == Game::WHITE ?
                        spBoard->HasWhiteMovedKing() :
                        spBoard->HasBlackMovedKing());
                    bool movedKingsideRook = (c == Game::WHITE ?
                        spBoard->HasWhiteMovedKingsideRook() :
                        spBoard->HasBlackMovedKingsideRook());
                    bool movedQueensideRook = (c == Game::WHITE ?
                        spBoard->HasWhiteMovedQueensideRook() :
                        spBoard->HasBlackMovedQueensideRook());
                    bool inCheck = (c == Game::WHITE ?
                        spBoard->IsWhiteInCheck() :
                        spBoard->IsBlackInCheck());

                    Game::square_type diff = m.GetEndFile() - m.GetStartFile();

                    // If the destination is empty
                    if (spBoard->IsEmpty(m.GetEndRank(), m.GetEndFile()))
                    {

                        // If the desitination is empty and we're not castling,
                        // definitely valid
                        if (diff > -2 && diff < 2) {
                            m.SetMovingPiece(Game::KING);
                            m_myValidMoves[m_numMyValidMoves++] = m;
                        }

                        // Verify castling move
                        else if (!movedKing && !inCheck)
                        {
                            // Kingside castle
                            if (diff == 2)
                            {
                                if (!movedKingsideRook &&
                                    spBoard->IsEmpty(m.GetStartRank(), m.GetStartFile()+1))
                                {
                                    if (spBoard->IsUnderAttack(m.GetStartRank(),
                                        m.GetStartFile()+1, oppC) == 0)
                                    {
                                        m.SetKingsideCastle();
                                        m.SetMovingPiece(Game::KING);
                                        m_myValidMoves[m_numMyValidMoves++] = m;
                                    }
                                }
                            }

                            // Queenside castle
                            else if (diff == -2)
                            {
                                if (!movedQueensideRook &&
                                    spBoard->IsEmpty(m.GetStartRank(), m.GetStartFile()-1))
                                {
                                    if (spBoard->IsUnderAttack(m.GetStartRank(),
                                        m.GetStartFile()-1, oppC) == 0)
                                    {
                                        m.SetQueensideCastle();
                                        m.SetMovingPiece(Game::KING);
                                        m_myValidMoves[m_numMyValidMoves++] = m;
                                    }
                                }
                            }
                        }
                    }

                    // Or if we are white and destination is black
                    else if ((c == Game::WHITE
                        && spBoard->IsBlack(m.GetEndRank(), m.GetEndFile())))
                    {
                        if (diff > -2 && diff < 2)
                        {
                            m.SetCapture();
                            m.SetMovingPiece(Game::KING);
                            m_myValidMoves[m_numMyValidMoves++] = m;
                        }
                    }

                    // Or if we are black and destination is white
                    else if ((c == Game::BLACK
                        && spBoard->IsWhite(m.GetEndRank(), m.GetEndFile())))
                    {
                        if (diff > -2 && diff < 2)
                        {
                            m.SetCapture();
                            m.SetMovingPiece(Game::KING);
                            m_myValidMoves[m_numMyValidMoves++] = m;
                        }
                    }
                }
            }
        }
    }

    // Remove moves that leave player in check
    validMoveList_t newValidMoves;
    Game::value_type sz = m_numMyValidMoves;
    m_numMyValidMoves = 0;

    for (Game::value_type i=0; i<sz; i++)
    {
        Game::BitBoard copy(*spBoard);
        Move m = m_myValidMoves[i];
        copy.MakeMove(m);

        // After making move, current player is opponent
        if (copy.GetPlayerInTurn() == Game::WHITE && copy.IsBlackInCheck())
        {
            // do nothing
        }
        else if (copy.GetPlayerInTurn() == Game::BLACK && copy.IsWhiteInCheck())
        {
            // do nothing
        }
        else
        {
            newValidMoves[m_numMyValidMoves++] = m;
        }
    }

    m_myValidMoves = newValidMoves;
}

//=============================================================================
validMoveList_t ValidMoveSet::GetMyValidMoves() const
{
    return m_myValidMoves;
}

//=============================================================================
validMoveList_t ValidMoveSet::GetOppValidMoves() const
{
    return m_oppValidMoves;
}

//=============================================================================
Game::value_type ValidMoveSet::GetNumMyValidMoves() const
{
    return m_numMyValidMoves;
}

//=============================================================================
Game::value_type ValidMoveSet::GetNumOppValidMoves() const
{
    return m_numOppValidMoves;
}

//=============================================================================
Game::value_type ValidMoveSet::GetAttackValue(Game::square_type rank, Game::square_type file) const
{
    return m_attackValue[GET_SQUARE(rank, file)];
}

//=============================================================================
Game::value_type ValidMoveSet::GetDefendValue(Game::square_type rank, Game::square_type file) const
{
    return m_defendValue[GET_SQUARE(rank, file)];
}

//=============================================================================
void ValidMoveSet::addSlidingMoveIfValid(
    const Game::BitBoardPtr &spBoard,
    Game::piece_type piece,
    Game::color_type color,
    Game::value_type value,
    moveList_t moves,
    Game::value_type size)
{
    Game::color_type playerInTurn = spBoard->GetPlayerInTurn();

    if (color == playerInTurn)
    {
        for (Game::square_type i=0; i<size; i++)
        {
            Move m = moves[i];

            // If the square is empty, it's valid
            if (spBoard->IsEmpty(m.GetEndRank(), m.GetEndFile()))
            {
                m.SetMovingPiece(piece);

                m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())] += value;
                m_myValidMoves[m_numMyValidMoves++] = m;
            }

            // If the square is occupied by the opposing color, it's valid
            else
            {
                Game::color_type c = (spBoard->IsWhite(m.GetEndRank(), m.GetEndFile()) ? Game::WHITE : Game::BLACK);

                if (playerInTurn != c)
                {
                    m.SetCapture();
                    m.SetMovingPiece(piece);

                    m_myValidMoves[m_numMyValidMoves++] = m;
                }
                else
                {
                    m.SetCapture();
                    m.SetMovingPiece(piece);

                    m_defendValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())] += value;
                }

                break;
            }
        }
    }
    else
    {
        for (Game::square_type i=0; i<size; i++)
        {
            Move m = moves[i];

            if (spBoard->IsEmpty(m.GetEndRank(), m.GetEndFile()))
            {
                m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())] += value;
                m_oppValidMoves[m_numOppValidMoves++] = m;
            }
            else
            {
                Game::color_type c = (spBoard->IsWhite(m.GetEndRank(), m.GetEndFile()) ? Game::WHITE : Game::BLACK);

                if (playerInTurn == c)
                {
                    m_oppValidMoves[m_numOppValidMoves++] = m;
                }
                else
                {
                    m_attackValue[GET_SQUARE(m.GetEndRank(), m.GetEndFile())] += value;
                }

                break;
            }
        }
    }
}

}
