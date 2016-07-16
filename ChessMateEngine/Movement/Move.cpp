#include "Move.h"

namespace Movement {

//==============================================================================
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
}

//==============================================================================
Move::Move(
    const Game::square_type &sRank,
    const Game::square_type &sFile,
    const Game::square_type &eRank,
    const Game::square_type &eFile
) :
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

//==============================================================================
Move::Move(
    const Game::square_type &sRank,
    const Game::square_type &sFile,
    const Game::square_type &eRank,
    const Game::square_type &eFile,
    const Game::piece_type &promotionPiece
) :
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

//==============================================================================
Move::Move(const std::string &move, Game::color_type player) :
    m_promotionPiece(Game::PAWN),
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
        m_movingPiece = Game::KING;
        m_IsKingsideCastle = true;

        m_startFile = Game::FILE_E;
        m_endFile = Game::FILE_G;

        if (player == Game::WHITE)
        {
            m_startRank = Game::RANK_1;
            m_endRank = Game::RANK_1;
        }
        else
        {
            m_startRank = Game::RANK_8;
            m_endRank = Game::RANK_8;
        }

        return;
    }
    else if (move.compare("O-O-O") == 0)
    {
        m_movingPiece = Game::KING;
        m_IsQueensideCastle = true;

        m_startFile = Game::FILE_E;
        m_endFile = Game::FILE_C;

        if (player == Game::WHITE)
        {
            m_startRank = Game::RANK_1;
            m_endRank = Game::RANK_1;
        }
        else
        {
            m_startRank = Game::RANK_8;
            m_endRank = Game::RANK_8;
        }

        return;
    }

    unsigned int ix = 0;

    // Parse starting piece
    if (move[ix] == 'N')
    {
        m_movingPiece = Game::KNIGHT;
        ++ix;
    }
    else if (move[ix] == 'B')
    {
        m_movingPiece = Game::BISHOP;
        ++ix;
    }
    else if (move[ix] == 'R')
    {
        m_movingPiece = Game::ROOK;
        ++ix;
    }
    else if (move[ix] == 'Q')
    {
        m_movingPiece = Game::QUEEN;
        ++ix;
    }
    else if (move[ix] == 'K')
    {
        m_movingPiece = Game::KING;
        ++ix;
    }
    else
    {
        m_movingPiece = Game::PAWN;
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
                m_promotionPiece = Game::KNIGHT;
            }
            else if (move[ix] == 'B')
            {
                m_promotionPiece = Game::BISHOP;
            }
            else if (move[ix] == 'R')
            {
                m_promotionPiece = Game::ROOK;
            }
            else if (move[ix] == 'Q')
            {
                m_promotionPiece = Game::QUEEN;
            }
            else
            {
                m_promotionPiece = Game::PAWN;
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

//==============================================================================
Game::square_type Move::GetStartFile() const
{
    return m_startFile;
}

//==============================================================================
Game::square_type Move::GetStartRank() const
{
    return m_startRank;
}

//==============================================================================
Game::square_type Move::GetEndFile() const
{
    return m_endFile;
}

//==============================================================================
Game::square_type Move::GetEndRank() const
{
    return m_endRank;
}

//==============================================================================
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
    if (m_movingPiece == Game::KNIGHT)
    {
        ret += "N";
    }
    else if (m_movingPiece == Game::BISHOP)
    {
        ret += "B";
    }
    else if (m_movingPiece == Game::ROOK)
    {
        ret += "R";
    }
    else if (m_movingPiece == Game::QUEEN)
    {
        ret += "Q";
    }
    else if (m_movingPiece == Game::KING)
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
    if (m_promotionPiece == Game::KNIGHT)
    {
        ret += "=N";
    }
    else if (m_promotionPiece == Game::BISHOP)
    {
        ret += "=B";
    }
    else if (m_promotionPiece == Game::ROOK)
    {
        ret += "=R";
    }
    else if (m_promotionPiece == Game::QUEEN)
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

//==============================================================================
Game::piece_type Move::GetMovingPiece() const
{
    return m_movingPiece;
}

//==============================================================================
void Move::SetMovingPiece(Game::piece_type movingPiece)
{
    m_movingPiece = movingPiece;
}

//==============================================================================
Game::piece_type Move::GetPromotionPiece() const
{
    return m_promotionPiece;
}

//==============================================================================
void Move::SetPromotionPiece(Game::piece_type promotionPiece)
{
    m_promotionPiece = promotionPiece;
}

//==============================================================================
bool Move::IsCapture() const
{
    return m_IsCapture;
}

//==============================================================================
void Move::SetCapture()
{
    m_IsCapture = true;
}

//==============================================================================
bool Move::IsCheck() const
{
    return m_IsCheck;
}

//==============================================================================
void Move::SetCheck()
{
    m_IsCheck = true;
}

//==============================================================================
bool Move::IsCheckmate() const
{
    return m_IsCheckmate;
}

//==============================================================================
void Move::SetCheckmate()
{
    m_IsCheckmate = true;
}

//==============================================================================
bool Move::IsEnPassant() const
{
    return m_IsEnPassant;
}

//==============================================================================
void Move::SetEnPassant()
{
    m_IsEnPassant = true;
}

//==============================================================================
bool Move::IsKingsideCastle() const
{
    return m_IsKingsideCastle;
}

//==============================================================================
void Move::SetKingsideCastle()
{
    m_IsKingsideCastle = true;
}

//==============================================================================
bool Move::IsQueensideCastle() const
{
    return m_IsQueensideCastle;
}

//==============================================================================
void Move::SetQueensideCastle()
{
    m_IsQueensideCastle = true;
}

//==============================================================================
bool Move::operator == (const Move &move) const
{
    return
    (
        (m_startRank == move.m_startRank) &&
        (m_startFile == move.m_startFile) &&
        (m_endRank == move.m_endRank) &&
        (m_endFile == move.m_endFile)
    );
}

//==============================================================================
std::ostream &operator << (std::ostream &stream, const Move &move)
{
    stream << move.GetPGNString();
    return stream;
}

}
