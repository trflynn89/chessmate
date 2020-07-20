#include "evaluator.h"

namespace chessmate {

namespace {

    // Piece values
    const value_type s_pawnValue = 100;
    const value_type s_knightValue = 320;
    const value_type s_bishopValue = 325;
    const value_type s_rookValue = 500;
    const value_type s_queenValue = 975;
    const value_type s_kingValue = 32767;

    // Array to store the "meaning" of a pawn in a file
    // i.e. account for isolated and doubled pawns
    value_type s_whitePawnFileValue[] = {0, 0, 0, 0, 0, 0, 0, 0};
    value_type s_blackPawnFileValue[] = {0, 0, 0, 0, 0, 0, 0, 0};

    // Count the number of bishops
    value_type s_whiteBishopCount = 0;
    value_type s_blackBishopCount = 0;

    // Tables go from 0th index = A1, to 63rd index = H8

    const value_type s_pawnTable[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  5,  10, 10, -25, -25, 10, 10, 5,  5, -5, -10, 0,  0,  -10,
        -5, 5,  0,  0,  0,  25, 25, 0,  0,  0,  5,  5,   10,  27, 27, 10, 5, 5,  10,  10, 20, 30,
        30, 20, 10, 10, 50, 50, 50, 50, 50, 50, 50, 50,  0,   0,  0,  0,  0, 0,  0,   0};

    const value_type s_knightTable[] = {
        -50, -40, -20, -30, -30, -20, -40, -50, -40, -20, 0,   5,   5,   0,   -20, -40,
        -30, 5,   10,  15,  15,  10,  5,   -30, -30, 0,   15,  20,  20,  15,  0,   -30,
        -30, 5,   15,  20,  20,  15,  5,   -30, -30, 0,   10,  15,  15,  10,  0,   -30,
        -40, -20, 0,   0,   0,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50};

    const value_type s_bishopTable[] = {
        -20, -10, -40, -10, -10, -40, -10, -20, -10, 5,   0,   0,   0,   0,   5,   -10,
        -10, 10,  10,  10,  10,  10,  10,  -10, -10, 0,   10,  10,  10,  10,  0,   -10,
        -10, 5,   5,   10,  10,  5,   5,   -10, -10, 0,   5,   10,  10,  5,   0,   -10,
        -10, 0,   0,   0,   0,   0,   0,   -10, -20, -10, -10, -10, -10, -10, -10, -20};

    const value_type s_kingTable[] = {
        20,  30,  10,  0,   0,   10,  30,  20,  20,  20,  0,   0,   0,   0,   20,  20,
        -10, -20, -20, -20, -20, -20, -20, -10, -20, -30, -30, -40, -40, -30, -30, -20,
        -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30};

    const value_type s_kingTableEndGame[] = {
        -50, -30, -30, -30, -30, -30, -30, -50, -30, -30, 0,   0,   0,   0,   -30, -30,
        -30, -10, 20,  30,  30,  20,  -10, -30, -30, -10, 30,  40,  40,  30,  -10, -30,
        -30, -10, 30,  40,  40,  30,  -10, -30, -30, -10, 20,  30,  30,  20,  -10, -30,
        -30, -20, -10, 0,   0,   -10, -20, -30, -50, -40, -30, -20, -20, -30, -40, -50};

} // namespace

//==================================================================================================
Evaluator::Evaluator(const color_type &engineColor) : m_engineColor(engineColor)
{
}

//==================================================================================================
int Evaluator::Score(const std::shared_ptr<BitBoard> &spBoard, const ValidMoveSet &vms) const
{
    value_type remainingPieces = 0;
    int score = 0;

    // Reinitialize values
    for (square_type i = FILE_A; i <= FILE_H; ++i)
    {
        s_whitePawnFileValue[i] = 0;
        s_blackPawnFileValue[i] = 0;
    }

    s_whiteBishopCount = 0;
    s_blackBishopCount = 0;

    // Check for game over
    if (vms.GetMyValidMoves().empty())
    {
        if (spBoard->IsWhiteInCheck())
        {
            return (m_engineColor == WHITE ? -s_kingValue : s_kingValue);
        }
        else if (spBoard->IsBlackInCheck())
        {
            return (m_engineColor == BLACK ? -s_kingValue : s_kingValue);
        }

        return 0;
    }
    else if (spBoard->IsStalemateViaFiftyMoves())
    {
        return 0;
    }
    else if (spBoard->IsStalemateViaRepetition())
    {
        return 0;
    }

    // Check for checks
    if (spBoard->IsWhiteInCheck())
    {
        score -= (spBoard->IsEndGame() ? 95 : 75);
    }
    else if (spBoard->IsBlackInCheck())
    {
        score += (spBoard->IsEndGame() ? 95 : 75);
    }

    // Add for tempo
    score += ((spBoard->GetPlayerInTurn() == WHITE) ? 10 : -10);

    // Try to prevent opponent from castling
    if (spBoard->HasWhiteCastled())
    {
        score += 40;
    }
    if (spBoard->HasBlackCastled())
    {
        score -= 40;
    }

    // Loop through all pieces
    for (square_type s = 0; s < BOARD_SIZE; ++s)
    {
        square_type rank = GET_RANK(s);
        square_type file = GET_FILE(s);

        if (!spBoard->IsEmpty(rank, file))
        {
            int pieceScore = evaluateSinglePiece(spBoard, vms, rank, file);

            score += (spBoard->IsWhite(rank, file) ? pieceScore : -pieceScore);
            ++remainingPieces;
        }
    }

    // Check for end game phase
    if (remainingPieces < 10)
    {
        spBoard->SetEndGame(true);
    }

    // Check for white isolated pawns
    for (square_type i = FILE_A; i <= FILE_H; ++i)
    {
        if (s_whitePawnFileValue[i] > 0)
        {
            if (((i > FILE_A) && (s_whitePawnFileValue[i - 1] == 0)) ||
                ((i < FILE_H) && (s_whitePawnFileValue[i + 1] == 0)))
            {
                score -= 15;
            }
        }
    }

    // Check for black isolated pawns
    for (square_type i = FILE_A; i <= FILE_H; ++i)
    {
        if (s_blackPawnFileValue[i] > 0)
        {
            if (((i > FILE_A) && (s_blackPawnFileValue[i - 1] == 0)) ||
                ((i < FILE_H) && (s_blackPawnFileValue[i + 1] == 0)))
            {
                score += 15;
            }
        }
    }

    // Check for white passed pawns
    for (square_type i = FILE_A; i <= FILE_H; ++i)
    {
        if ((s_whitePawnFileValue[i] > 0) && (s_blackPawnFileValue[i] == 0))
        {
            score += s_whitePawnFileValue[i];
        }
    }

    // Check for black passed pawns
    for (square_type i = FILE_A; i <= FILE_H; ++i)
    {
        if ((s_blackPawnFileValue[i] > 0) && (s_whitePawnFileValue[i] == 0))
        {
            score += s_blackPawnFileValue[i];
        }
    }

    return ((m_engineColor == WHITE) ? score : -score);
}

//==================================================================================================
int Evaluator::evaluateSinglePiece(
    const std::shared_ptr<BitBoard> &spBoard,
    const ValidMoveSet &vms,
    const square_type &rank,
    const square_type &file) const
{
    MoveList myMoveSet = vms.GetMyValidMoves();
    MoveList oppMoveSet = vms.GetOppValidMoves();

    square_type loc = GET_SQUARE(rank, file);
    color_type pieceColor = spBoard->GetOccupant(rank, file);
    int score = 0;

    if (pieceColor == BLACK)
    {
        loc = BOARD_SIZE - loc - 1;
    }

    // Account for the attacked/defended value of the piece
    bool useMyMoves = (pieceColor == spBoard->GetPlayerInTurn());

    value_type defendedValue = 0;
    value_type attackedValue = 0;

    // Get correct attack/defense values
    if (useMyMoves)
    {
        defendedValue = vms.GetDefendValue(rank, file);
        attackedValue = vms.GetAttackValue(rank, file);
    }
    else
    {
        defendedValue = vms.GetAttackValue(rank, file);
        attackedValue = vms.GetDefendValue(rank, file);
    }

    score += defendedValue;
    score -= attackedValue;

    // Penalize further for situations where we will lose the piece
    if (defendedValue < attackedValue)
    {
        score -= (attackedValue - defendedValue) * 10;
    }

    // Add points for mobility
    MoveList &moveSet = (useMyMoves ? myMoveSet : oppMoveSet);

    for (auto it = moveSet.begin(); it != moveSet.end(); ++it)
    {
        if ((it->GetStartRank() == rank) && (it->GetStartFile() == file))
        {
            ++score;
        }
    }

    // Evaluate depending on piece

    // Pawn
    if (spBoard->IsPawn(rank, file))
    {
        score += s_pawnValue;
        score += s_pawnTable[loc];

        // Rook-file pawns worth less - can only attack in one direction
        if (file == FILE_A || file == FILE_H)
        {
            score -= 15;
        }

        // White pawn
        if (pieceColor == WHITE)
        {
            // If the value isn't 0, we have a doubled pawn
            if (s_whitePawnFileValue[file] > 0)
            {
                score -= 16;
            }

            // Good to advance pawn to second rank
            if (rank == RANK_2)
            {
                if (attackedValue == 0)
                {
                    s_whitePawnFileValue[file] += 200;

                    if (defendedValue > 0)
                    {
                        s_whitePawnFileValue[file] += 50;
                    }
                }
            }
            // Good to advance to third rank
            else if (rank == RANK_3)
            {
                if (attackedValue == 0)
                {
                    s_whitePawnFileValue[file] += 100;

                    if (defendedValue > 0)
                    {
                        s_whitePawnFileValue[file] += 25;
                    }
                }
            }

            // Mark that there is a pawn in this file
            s_whitePawnFileValue[file] += 10;
        }

        // Black pawn
        else if (pieceColor == BLACK)
        {
            // If the value isn't 0, we have a doubled pawn
            if (s_blackPawnFileValue[file] > 0)
            {
                score -= 16;
            }

            // Good to advance pawn to seventh rank
            if (rank == RANK_7)
            {
                if (attackedValue == 0)
                {
                    s_blackPawnFileValue[file] += 200;

                    if (defendedValue > 0)
                    {
                        s_blackPawnFileValue[file] += 50;
                    }
                }
            }
            // Good to advance to sixth rank
            else if (rank == RANK_6)
            {
                if (attackedValue == 0)
                {
                    s_blackPawnFileValue[file] += 100;

                    if (defendedValue > 0)
                    {
                        s_blackPawnFileValue[file] += 25;
                    }
                }
            }

            // Mark that there is a pawn in this file
            s_blackPawnFileValue[file] += 10;
        }
    }

    // Knight
    else if (spBoard->IsKnight(rank, file))
    {
        score += s_knightValue;
        score += s_knightTable[loc];

        // Knights are worth less in the end game phase
        if (spBoard->IsEndGame())
        {
            score -= 10;
        }
    }

    // Bishop
    else if (spBoard->IsBishop(rank, file))
    {
        score += s_bishopValue;
        score += s_bishopTable[loc];

        // Bishops are worth more in the end game phase
        if (spBoard->IsEndGame())
        {
            score += 10;
        }

        // Bishops are better if we have more than one
        if (pieceColor == WHITE)
        {
            ++s_whiteBishopCount;
            if (s_whiteBishopCount >= 2)
            {
                score += 10;
            }
        }
        else if (pieceColor == BLACK)
        {
            ++s_blackBishopCount;
            if (s_blackBishopCount >= 2)
            {
                score += 10;
            }
        }
    }

    // Rook
    else if (spBoard->IsRook(rank, file))
    {
        score += s_rookValue;

        // Encourage rooks not to move until we have castled
        if (pieceColor == WHITE)
        {
            if (!spBoard->HasWhiteCastled())
            {
                if (rank != RANK_1)
                {
                    score -= 10;
                }
                else if ((file != FILE_A) && (file != FILE_H))
                {
                    score -= 10;
                }
            }
        }
        else if (pieceColor == BLACK)
        {
            if (!spBoard->HasBlackCastled())
            {
                if (rank != RANK_8)
                {
                    score -= 10;
                }
                else if ((file != FILE_A) && (file != FILE_H))
                {
                    score -= 10;
                }
            }
        }
    }

    // Queen
    else if (spBoard->IsQueen(rank, file))
    {
        score += s_queenValue;

        // Discourage queen from moving too early
        if (pieceColor == WHITE)
        {
            if (spBoard->HasWhiteMovedQueen() && !spBoard->IsEndGame())
            {
                score -= 10;
            }
        }
        else if (pieceColor == BLACK)
        {
            if (spBoard->HasBlackMovedQueen() && !spBoard->IsEndGame())
            {
                score -= 10;
            }
        }
    }

    // King
    else if (spBoard->IsKing(rank, file))
    {
        score += s_kingValue;

        // Keep king mobile
        value_type numberOfKingMoves = 0;

        for (auto it = myMoveSet.begin(); it != myMoveSet.end(); ++it)
        {
            if ((it->GetStartRank() == rank) && (it->GetStartFile() == file))
            {
                ++numberOfKingMoves;
            }
        }

        if (numberOfKingMoves < 2)
        {
            score -= 5;
        }

        if (spBoard->IsEndGame())
        {
            score += s_kingTableEndGame[loc];
        }
        else
        {
            score += s_kingTable[loc];

            // Encourage castling
            bool castled = false;

            if (pieceColor == WHITE)
            {
                castled =
                    spBoard->HasWhiteMovedKingsideRook() || spBoard->HasWhiteMovedQueensideRook();

                if (spBoard->HasWhiteMovedKing() && !castled)
                {
                    score -= 30;
                }
            }
            else if (pieceColor == BLACK)
            {
                castled =
                    spBoard->HasBlackMovedKingsideRook() || spBoard->HasBlackMovedQueensideRook();

                if (spBoard->HasBlackMovedKing() && !castled)
                {
                    score -= 30;
                }
            }
        }
    }

    return score;
}

} // namespace chessmate
