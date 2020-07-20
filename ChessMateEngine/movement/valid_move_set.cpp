#include "valid_move_set.h"

namespace chessmate {

namespace {

    // Attacking piece values - better to attack with pawn than queen
    const value_type s_pawnValue = 6;
    const value_type s_knightValue = 3;
    const value_type s_bishopValue = 3;
    const value_type s_rookValue = 2;
    const value_type s_queenValue = 1;
    // const value_type s_kingValue = 1;

} // namespace

//==================================================================================================
ValidMoveSet::ValidMoveSet(
    const std::weak_ptr<MoveSet> &wpMoveSet,
    const std::shared_ptr<BitBoard> &spBoard)
{
    std::shared_ptr<MoveSet> spMoveSet = wpMoveSet.lock();

    for (MoveList::size_type i = 0; i < BOARD_SIZE; ++i)
    {
        m_attackValue[i] = 0;
        m_defendValue[i] = 0;
    }

    if (spMoveSet && spBoard)
    {
        generateValidMoves(spMoveSet, spBoard);
    }
}

//==================================================================================================
void ValidMoveSet::generateValidMoves(
    const std::shared_ptr<MoveSet> &spMoveSet,
    const std::shared_ptr<BitBoard> &spBoard)
{
    color_type playerInTurn = spBoard->GetPlayerInTurn();

    for (MoveList::size_type i = 0; i < BOARD_SIZE; ++i)
    {
        MoveList wPawn = spMoveSet->GetWhitePawnMoves(i);
        MoveList bPawn = spMoveSet->GetBlackPawnMoves(i);
        MoveList knight = spMoveSet->GetKnightMoves(i);
        MoveList king = spMoveSet->GetKingMoves(i);

        square_type rank = GET_RANK(i);
        square_type file = GET_FILE(i);
        color_type c = spBoard->GetOccupant(rank, file);

        /***** EMPTY *****/

        if (spBoard->IsEmpty(rank, file))
        {
            continue;
        }

        /***** PAWN *****/

        else if (spBoard->IsPawn(rank, file))
        {
            // WHITE pawn
            if (c == WHITE)
            {
                for (auto it = wPawn.begin(); it != wPawn.end(); ++it)
                {
                    // Straight
                    if (it->GetStartFile() == it->GetEndFile())
                    {
                        // 2 move start
                        if (it->GetEndRank() - it->GetStartRank() == 2)
                        {
                            if (spBoard->IsEmpty(it->GetStartRank() + 1, it->GetStartFile()))
                            {
                                if (spBoard->IsEmpty(it->GetEndRank(), it->GetEndFile()))
                                {
                                    it->SetMovingPiece(PAWN);

                                    if (playerInTurn == WHITE)
                                    {
                                        m_myValidMoves.push_back(*it);
                                    }
                                    else
                                    {
                                        m_oppValidMoves.push_back(*it);
                                    }
                                }
                            }
                        }

                        // Single move forward
                        else if (spBoard->IsEmpty(it->GetEndRank(), it->GetEndFile()))
                        {
                            it->SetMovingPiece(PAWN);

                            if (playerInTurn == WHITE)
                            {
                                m_myValidMoves.push_back(*it);
                            }
                            else
                            {
                                m_oppValidMoves.push_back(*it);
                            }
                        }
                    }

                    // Attack diagonally
                    else
                    {
                        if (playerInTurn == WHITE)
                        {
                            m_defendValue[GET_SQUARE(it->GetEndRank(), it->GetEndFile())] +=
                                s_pawnValue;
                        }
                        else
                        {
                            m_attackValue[GET_SQUARE(it->GetEndRank(), it->GetEndFile())] +=
                                s_pawnValue;
                        }

                        if (spBoard->IsBlack(it->GetEndRank(), it->GetEndFile()))
                        {
                            it->SetCapture();
                            it->SetMovingPiece(PAWN);

                            if (playerInTurn == WHITE)
                            {
                                m_myValidMoves.push_back(*it);
                            }
                            else
                            {
                                m_oppValidMoves.push_back(*it);
                            }
                        }

                        // En passant
                        else if (spBoard->GetEnPassantColor() == BLACK)
                        {
                            square_type pos = spBoard->GetEnPassantPosition();
                            square_type r = GET_RANK(pos);
                            square_type f = GET_FILE(pos);

                            if ((r == it->GetEndRank()) && (f == it->GetEndFile()))
                            {
                                it->SetCapture();
                                it->SetEnPassant();
                                it->SetMovingPiece(PAWN);

                                if (playerInTurn == WHITE)
                                {
                                    m_myValidMoves.push_back(*it);
                                }
                                else
                                {
                                    m_oppValidMoves.push_back(*it);
                                }
                            }
                        }
                    }
                }
            }

            // BLACK pawn
            else
            {
                for (auto it = bPawn.begin(); it != bPawn.end(); ++it)
                {
                    // Straight
                    if (it->GetStartFile() == it->GetEndFile())
                    {
                        // 2 move start
                        if (it->GetStartRank() - it->GetEndRank() == 2)
                        {
                            if (spBoard->IsEmpty(it->GetStartRank() - 1, it->GetStartFile()))
                            {
                                if (spBoard->IsEmpty(it->GetEndRank(), it->GetEndFile()))
                                {
                                    it->SetMovingPiece(PAWN);

                                    if (playerInTurn == BLACK)
                                    {
                                        m_myValidMoves.push_back(*it);
                                    }
                                    else
                                    {
                                        m_oppValidMoves.push_back(*it);
                                    }
                                }
                            }
                        }

                        // Single move forward
                        else if (spBoard->IsEmpty(it->GetEndRank(), it->GetEndFile()))
                        {
                            it->SetMovingPiece(PAWN);

                            if (playerInTurn == BLACK)
                            {
                                m_myValidMoves.push_back(*it);
                            }
                            else
                            {
                                m_oppValidMoves.push_back(*it);
                            }
                        }
                    }

                    // Attack diagonally
                    else
                    {
                        if (playerInTurn == BLACK)
                        {
                            m_defendValue[GET_SQUARE(it->GetEndRank(), it->GetEndFile())] +=
                                s_pawnValue;
                        }
                        else
                        {
                            m_attackValue[GET_SQUARE(it->GetEndRank(), it->GetEndFile())] +=
                                s_pawnValue;
                        }

                        if (spBoard->IsWhite(it->GetEndRank(), it->GetEndFile()))
                        {
                            it->SetCapture();
                            it->SetMovingPiece(PAWN);

                            if (playerInTurn == BLACK)
                            {
                                m_myValidMoves.push_back(*it);
                            }
                            else
                            {
                                m_oppValidMoves.push_back(*it);
                            }
                        }

                        // En passant
                        else if (spBoard->GetEnPassantColor() == WHITE)
                        {
                            square_type pos = spBoard->GetEnPassantPosition();
                            square_type r = GET_RANK(pos);
                            square_type f = GET_FILE(pos);

                            if ((r == it->GetEndRank()) && (f == it->GetEndFile()))
                            {
                                it->SetCapture();
                                it->SetEnPassant();
                                it->SetMovingPiece(PAWN);

                                if (playerInTurn == BLACK)
                                {
                                    m_myValidMoves.push_back(*it);
                                }
                                else
                                {
                                    m_oppValidMoves.push_back(*it);
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
            if (playerInTurn == c)
            {
                for (auto it = knight.begin(); it != knight.end(); ++it)
                {
                    square_type endSquare = GET_SQUARE(it->GetEndRank(), it->GetEndFile());

                    if (spBoard->IsEmpty(it->GetEndRank(), it->GetEndFile()))
                    {
                        it->SetMovingPiece(KNIGHT);

                        m_defendValue[endSquare] += s_knightValue;
                        m_myValidMoves.push_back(*it);
                    }
                    else if (c != spBoard->GetOccupant(it->GetEndRank(), it->GetEndFile()))
                    {
                        it->SetMovingPiece(KNIGHT);
                        it->SetCapture();

                        m_defendValue[endSquare] += s_knightValue;
                        m_myValidMoves.push_back(*it);
                    }
                }
            }
            else
            {
                for (auto it = knight.begin(); it != knight.end(); ++it)
                {
                    square_type endSquare = GET_SQUARE(it->GetEndRank(), it->GetEndFile());

                    if (spBoard->IsEmpty(it->GetEndRank(), it->GetEndFile()))
                    {
                        it->SetMovingPiece(KNIGHT);

                        m_attackValue[endSquare] += s_knightValue;
                        m_oppValidMoves.push_back(*it);
                    }
                    else if (c != spBoard->GetOccupant(it->GetEndRank(), it->GetEndFile()))
                    {
                        it->SetMovingPiece(KNIGHT);
                        it->SetCapture();

                        m_attackValue[endSquare] += s_knightValue;
                        m_oppValidMoves.push_back(*it);
                    }
                }
            }
        }

        /***** BISHOP *****/

        if (spBoard->IsBishop(rank, file))
        {
            addSlidingMoveIfValid(
                spBoard,
                BISHOP,
                c,
                s_bishopValue,
                spMoveSet->GetBishopMovesNE(i));
            addSlidingMoveIfValid(
                spBoard,
                BISHOP,
                c,
                s_bishopValue,
                spMoveSet->GetBishopMovesNW(i));
            addSlidingMoveIfValid(
                spBoard,
                BISHOP,
                c,
                s_bishopValue,
                spMoveSet->GetBishopMovesSE(i));
            addSlidingMoveIfValid(
                spBoard,
                BISHOP,
                c,
                s_bishopValue,
                spMoveSet->GetBishopMovesSW(i));
        }

        /***** ROOK *****/

        else if (spBoard->IsRook(rank, file))
        {
            addSlidingMoveIfValid(spBoard, ROOK, c, s_rookValue, spMoveSet->GetRookMovesN(i));
            addSlidingMoveIfValid(spBoard, ROOK, c, s_rookValue, spMoveSet->GetRookMovesS(i));
            addSlidingMoveIfValid(spBoard, ROOK, c, s_rookValue, spMoveSet->GetRookMovesE(i));
            addSlidingMoveIfValid(spBoard, ROOK, c, s_rookValue, spMoveSet->GetRookMovesW(i));
        }

        /***** QUEEN *****/

        else if (spBoard->IsQueen(rank, file))
        {
            addSlidingMoveIfValid(spBoard, QUEEN, c, s_queenValue, spMoveSet->GetQueenMovesN(i));
            addSlidingMoveIfValid(spBoard, QUEEN, c, s_queenValue, spMoveSet->GetQueenMovesS(i));
            addSlidingMoveIfValid(spBoard, QUEEN, c, s_queenValue, spMoveSet->GetQueenMovesE(i));
            addSlidingMoveIfValid(spBoard, QUEEN, c, s_queenValue, spMoveSet->GetQueenMovesW(i));
            addSlidingMoveIfValid(spBoard, QUEEN, c, s_queenValue, spMoveSet->GetQueenMovesNE(i));
            addSlidingMoveIfValid(spBoard, QUEEN, c, s_queenValue, spMoveSet->GetQueenMovesNW(i));
            addSlidingMoveIfValid(spBoard, QUEEN, c, s_queenValue, spMoveSet->GetQueenMovesSE(i));
            addSlidingMoveIfValid(spBoard, QUEEN, c, s_queenValue, spMoveSet->GetQueenMovesSW(i));
        }

        /***** KING *****/

        if (spBoard->IsKing(rank, file))
        {
            color_type oppC = (c == WHITE ? BLACK : WHITE);

            if (c == spBoard->GetPlayerInTurn())
            {
                for (auto it = king.begin(); it != king.end(); ++it)
                {
                    bool movedKing =
                        (c == WHITE ? spBoard->HasWhiteMovedKing() : spBoard->HasBlackMovedKing());
                    bool movedKingsideRook =
                        (c == WHITE ? spBoard->HasWhiteMovedKingsideRook() :
                                      spBoard->HasBlackMovedKingsideRook());
                    bool movedQueensideRook =
                        (c == WHITE ? spBoard->HasWhiteMovedQueensideRook() :
                                      spBoard->HasBlackMovedQueensideRook());
                    bool inCheck =
                        (c == WHITE ? spBoard->IsWhiteInCheck() : spBoard->IsBlackInCheck());

                    square_type diff = it->GetEndFile() - it->GetStartFile();

                    // If the destination is empty
                    if (spBoard->IsEmpty(it->GetEndRank(), it->GetEndFile()))
                    {
                        // If the desitination is empty and we're not castling,
                        // definitely valid
                        if ((diff > -2) && (diff < 2))
                        {
                            it->SetMovingPiece(KING);
                            m_myValidMoves.push_back(*it);
                        }

                        // Verify castling move
                        else if (!movedKing && !inCheck)
                        {
                            // Kingside castle
                            if (diff == 2)
                            {
                                if (!movedKingsideRook &&
                                    spBoard->IsEmpty(it->GetStartRank(), it->GetStartFile() + 1))
                                {
                                    if (spBoard->IsUnderAttack(
                                            it->GetStartRank(),
                                            it->GetStartFile() + 1,
                                            oppC) == 0)
                                    {
                                        it->SetKingsideCastle();
                                        it->SetMovingPiece(KING);
                                        m_myValidMoves.push_back(*it);
                                    }
                                }
                            }

                            // Queenside castle
                            else if (diff == -2)
                            {
                                if (!movedQueensideRook &&
                                    spBoard->IsEmpty(it->GetStartRank(), it->GetStartFile() - 1))
                                {
                                    if (spBoard->IsUnderAttack(
                                            it->GetStartRank(),
                                            it->GetStartFile() - 1,
                                            oppC) == 0)
                                    {
                                        it->SetQueensideCastle();
                                        it->SetMovingPiece(KING);
                                        m_myValidMoves.push_back(*it);
                                    }
                                }
                            }
                        }
                    }

                    // Or if we are white and destination is black
                    else if ((c == WHITE) && spBoard->IsBlack(it->GetEndRank(), it->GetEndFile()))
                    {
                        if ((diff > -2) && (diff < 2))
                        {
                            it->SetCapture();
                            it->SetMovingPiece(KING);
                            m_myValidMoves.push_back(*it);
                        }
                    }

                    // Or if we are black and destination is white
                    else if ((c == BLACK) && spBoard->IsWhite(it->GetEndRank(), it->GetEndFile()))
                    {
                        if ((diff > -2) && (diff < 2))
                        {
                            it->SetCapture();
                            it->SetMovingPiece(KING);
                            m_myValidMoves.push_back(*it);
                        }
                    }
                }
            }
        }
    }

    // Remove moves that leave player in check
    MoveList newValidMoves;

    for (auto it = m_myValidMoves.begin(); it != m_myValidMoves.end(); ++it)
    {
        BitBoard copy(*spBoard);
        copy.MakeMove(*it);

        // After making move, current player is opponent
        if ((copy.GetPlayerInTurn() == WHITE) && copy.IsBlackInCheck())
        {
            // do nothing
        }
        else if ((copy.GetPlayerInTurn() == BLACK) && copy.IsWhiteInCheck())
        {
            // do nothing
        }
        else
        {
            newValidMoves.push_back(*it);
        }
    }

    m_myValidMoves = newValidMoves;
}

//==================================================================================================
MoveList ValidMoveSet::GetMyValidMoves() const
{
    return m_myValidMoves;
}

//==================================================================================================
MoveList ValidMoveSet::GetOppValidMoves() const
{
    return m_oppValidMoves;
}

//==================================================================================================
value_type ValidMoveSet::GetAttackValue(square_type rank, square_type file) const
{
    return m_attackValue[GET_SQUARE(rank, file)];
}

//==================================================================================================
value_type ValidMoveSet::GetDefendValue(square_type rank, square_type file) const
{
    return m_defendValue[GET_SQUARE(rank, file)];
}

//==================================================================================================
void ValidMoveSet::addSlidingMoveIfValid(
    const std::shared_ptr<BitBoard> &spBoard,
    const piece_type &piece,
    const color_type &color,
    const value_type &value,
    MoveList moves)
{
    color_type playerInTurn = spBoard->GetPlayerInTurn();

    if (color == playerInTurn)
    {
        for (auto it = moves.begin(); it != moves.end(); ++it)
        {
            square_type endSquare = GET_SQUARE(it->GetEndRank(), it->GetEndFile());

            // If the square is empty, it's valid
            if (spBoard->IsEmpty(it->GetEndRank(), it->GetEndFile()))
            {
                it->SetMovingPiece(piece);

                m_defendValue[endSquare] += value;
                m_myValidMoves.push_back(*it);

                continue;
            }

            // If the square is occupied by the opposing color, it's valid
            else if (playerInTurn != spBoard->GetOccupant(it->GetEndRank(), it->GetEndFile()))
            {
                it->SetCapture();
                it->SetMovingPiece(piece);

                m_myValidMoves.push_back(*it);
            }

            else
            {
                m_defendValue[endSquare] += value;
            }

            break;
        }
    }
    else
    {
        for (auto it = moves.begin(); it != moves.end(); ++it)
        {
            square_type endSquare = GET_SQUARE(it->GetEndRank(), it->GetEndFile());

            if (spBoard->IsEmpty(it->GetEndRank(), it->GetEndFile()))
            {
                it->SetMovingPiece(piece);

                m_attackValue[endSquare] += value;
                m_oppValidMoves.push_back(*it);

                continue;
            }

            else if (playerInTurn == spBoard->GetOccupant(it->GetEndRank(), it->GetEndFile()))
            {
                it->SetCapture();
                it->SetMovingPiece(piece);

                m_oppValidMoves.push_back(*it);
            }

            else
            {
                m_attackValue[endSquare] += value;
            }

            break;
        }
    }
}

} // namespace chessmate
