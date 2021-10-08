#include "move.h"

#include <fly/fly.hpp>

namespace chessmate {

//==================================================================================================
Move::Move() :
    m_startRank(-1),
    m_startFile(-1),
    m_endRank(-1),
    m_endFile(-1),
    m_movingPiece(-1),
    m_promotionPiece(-1),
    m_IsCheck(false),
    m_IsCheckmate(false),
    m_IsStalemate(false),
    m_IsCapture(false),
    m_IsKingsideCastle(false),
    m_IsQueensideCastle(false),
    m_IsEnPassant(false)
{
    FLY_UNUSED(m_IsStalemate);
}

//==================================================================================================
Move::Move(
    const square_type &sRank,
    const square_type &sFile,
    const square_type &eRank,
    const square_type &eFile) :
    m_startRank(sRank),
    m_startFile(sFile),
    m_endRank(eRank),
    m_endFile(eFile),
    m_movingPiece(-1),
    m_promotionPiece(-1),
    m_IsCheck(false),
    m_IsCheckmate(false),
    m_IsStalemate(false),
    m_IsCapture(false),
    m_IsKingsideCastle(false),
    m_IsQueensideCastle(false),
    m_IsEnPassant(false)
{
}

//==================================================================================================
Move::Move(
    const square_type &sRank,
    const square_type &sFile,
    const square_type &eRank,
    const square_type &eFile,
    const piece_type &promotionPiece) :
    m_startRank(sRank),
    m_startFile(sFile),
    m_endRank(eRank),
    m_endFile(eFile),
    m_movingPiece(-1),
    m_promotionPiece(promotionPiece),
    m_IsCheck(false),
    m_IsCheckmate(false),
    m_IsStalemate(false),
    m_IsCapture(false),
    m_IsKingsideCastle(false),
    m_IsQueensideCastle(false),
    m_IsEnPassant(false)
{
}

//==================================================================================================
Move::Move(const std::string &move, color_type player) :
    m_promotionPiece(PAWN),
    m_IsCheck(false),
    m_IsCheckmate(false),
    m_IsStalemate(false),
    m_IsCapture(false),
    m_IsKingsideCastle(false),
    m_IsQueensideCastle(false),
    m_IsEnPassant(false)
{
    // Parse for castle
    if (move.compare("O-O") == 0)
    {
        m_movingPiece = KING;
        m_IsKingsideCastle = true;

        m_startFile = FILE_E;
        m_endFile = FILE_G;

        if (player == WHITE)
        {
            m_startRank = RANK_1;
            m_endRank = RANK_1;
        }
        else
        {
            m_startRank = RANK_8;
            m_endRank = RANK_8;
        }

        return;
    }
    else if (move.compare("O-O-O") == 0)
    {
        m_movingPiece = KING;
        m_IsQueensideCastle = true;

        m_startFile = FILE_E;
        m_endFile = FILE_C;

        if (player == WHITE)
        {
            m_startRank = RANK_1;
            m_endRank = RANK_1;
        }
        else
        {
            m_startRank = RANK_8;
            m_endRank = RANK_8;
        }

        return;
    }

    unsigned int ix = 0;

    // Parse starting piece
    if (move[ix] == 'N')
    {
        m_movingPiece = KNIGHT;
        ++ix;
    }
    else if (move[ix] == 'B')
    {
        m_movingPiece = BISHOP;
        ++ix;
    }
    else if (move[ix] == 'R')
    {
        m_movingPiece = ROOK;
        ++ix;
    }
    else if (move[ix] == 'Q')
    {
        m_movingPiece = QUEEN;
        ++ix;
    }
    else if (move[ix] == 'K')
    {
        m_movingPiece = KING;
        ++ix;
    }
    else
    {
        m_movingPiece = PAWN;
    }

    // Parse starting position
    m_startFile = move[ix++] - 'a';
    m_startRank = move[ix++] - '1';

    // Parse for capture
    if (move[ix] == 'x')
    {
        ix++;
        m_IsCapture = true;
    }

    // Parse ending position
    m_endFile = move[ix++] - 'a';
    m_endRank = move[ix++] - '1';

    // Special moves
    while (ix < move.length())
    {
        if (move[ix] == '=')
        {
            ++ix;

            if (move[ix] == 'N')
            {
                m_promotionPiece = KNIGHT;
            }
            else if (move[ix] == 'B')
            {
                m_promotionPiece = BISHOP;
            }
            else if (move[ix] == 'R')
            {
                m_promotionPiece = ROOK;
            }
            else if (move[ix] == 'Q')
            {
                m_promotionPiece = QUEEN;
            }
            else
            {
                m_promotionPiece = PAWN;
            }
        }
        else if (move[ix] == 'e')
        {
            if (move[++ix] == 'p')
            {
                m_IsEnPassant = true;
            }
        }
        else if (move[ix] == '+')
        {
            m_IsCheck = true;
        }
        else if (move[ix] == '#')
        {
            m_IsCheckmate = true;
        }

        ++ix;
    }
}

//==================================================================================================
square_type Move::GetStartFile() const
{
    return m_startFile;
}

//==================================================================================================
square_type Move::GetStartRank() const
{
    return m_startRank;
}

//==================================================================================================
square_type Move::GetEndFile() const
{
    return m_endFile;
}

//==================================================================================================
square_type Move::GetEndRank() const
{
    return m_endRank;
}

//==================================================================================================
std::string Move::GetPGNString() const
{
    if (m_IsKingsideCastle)
    {
        return "O-O";
    }
    else if (m_IsQueensideCastle)
    {
        return "O-O-O";
    }

    std::string ret("");

    // Set the moving piece
    if (m_movingPiece == KNIGHT)
    {
        ret += "N";
    }
    else if (m_movingPiece == BISHOP)
    {
        ret += "B";
    }
    else if (m_movingPiece == ROOK)
    {
        ret += "R";
    }
    else if (m_movingPiece == QUEEN)
    {
        ret += "Q";
    }
    else if (m_movingPiece == KING)
    {
        ret += "K";
    }

    // Append the start file and start rank
    ret += ('a' + m_startFile);
    ret += ('1' + m_startRank);

    // If move is capture, indicate such
    if (m_IsCapture)
    {
        ret += 'x';
    }

    // Append the end file and end rank
    ret += ('a' + m_endFile);
    ret += ('1' + m_endRank);

    // If move is a promotion, indicate the promotion piece
    if (m_promotionPiece == KNIGHT)
    {
        ret += "=N";
    }
    else if (m_promotionPiece == BISHOP)
    {
        ret += "=B";
    }
    else if (m_promotionPiece == ROOK)
    {
        ret += "=R";
    }
    else if (m_promotionPiece == QUEEN)
    {
        ret += "=Q";
    }

    // If move is an enpassant, indicate such
    if (m_IsEnPassant)
    {
        ret += "ep";
    }

    // If move is check or checkmate, indicate such
    if (m_IsCheckmate)
    {
        ret += '#';
    }
    else if (m_IsCheck)
    {
        ret += '+';
    }

    return ret;
}

//==================================================================================================
piece_type Move::GetMovingPiece() const
{
    return m_movingPiece;
}

//==================================================================================================
void Move::SetMovingPiece(piece_type movingPiece)
{
    m_movingPiece = movingPiece;
}

//==================================================================================================
piece_type Move::GetPromotionPiece() const
{
    return m_promotionPiece;
}

//==================================================================================================
void Move::SetPromotionPiece(piece_type promotionPiece)
{
    m_promotionPiece = promotionPiece;
}

//==================================================================================================
bool Move::IsCapture() const
{
    return m_IsCapture;
}

//==================================================================================================
void Move::SetCapture()
{
    m_IsCapture = true;
}

//==================================================================================================
bool Move::IsCheck() const
{
    return m_IsCheck;
}

//==================================================================================================
void Move::SetCheck()
{
    m_IsCheck = true;
}

//==================================================================================================
bool Move::IsCheckmate() const
{
    return m_IsCheckmate;
}

//==================================================================================================
void Move::SetCheckmate()
{
    m_IsCheckmate = true;
}

//==================================================================================================
bool Move::IsEnPassant() const
{
    return m_IsEnPassant;
}

//==================================================================================================
void Move::SetEnPassant()
{
    m_IsEnPassant = true;
}

//==================================================================================================
bool Move::IsKingsideCastle() const
{
    return m_IsKingsideCastle;
}

//==================================================================================================
void Move::SetKingsideCastle()
{
    m_IsKingsideCastle = true;
}

//==================================================================================================
bool Move::IsQueensideCastle() const
{
    return m_IsQueensideCastle;
}

//==================================================================================================
void Move::SetQueensideCastle()
{
    m_IsQueensideCastle = true;
}

//==================================================================================================
bool Move::operator==(const Move &move) const
{
    return (
        (m_startRank == move.m_startRank) && (m_startFile == move.m_startFile) &&
        (m_endRank == move.m_endRank) && (m_endFile == move.m_endFile));
}

} // namespace chessmate
