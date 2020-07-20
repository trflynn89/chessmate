package com.flynn.chessmate.movement;

import com.flynn.chessmate.game.Piece;
import com.flynn.chessmate.util.Constants;

import java.awt.Color;

/**
 * Data structure to store information about a move.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
public class Move
{
    private int m_startRank;
    private int m_startFile;
    private int m_endRank;
    private int m_endFile;

    private Piece m_movingPiece;

    private boolean m_isCheck;
    private boolean m_isCheckmate;
    private boolean m_isStalemate;
    private boolean m_isCapture;
    private boolean m_isKingsideCastle;
    private boolean m_isQueensideCastle;
    private boolean m_isEnPassant;
    private Piece m_pawnPromotionPiece;
    private int m_ambiguityType;

    /**
     * Constructor.
     *
     * @param rank The rank of the start position.
     * @param file The file of the start position.
     * @param i The rank of the end position.
     * @param j The file of the end position.
     */
    public Move(int rank, int file, int i, int j)
    {
        this.m_startRank = rank;
        this.m_startFile = file;
        this.m_endRank = i;
        this.m_endFile = j;

        m_isCheck = false;
        m_isCheckmate = false;
        m_isStalemate = false;
        m_isCapture = false;
        m_isKingsideCastle = false;
        m_isQueensideCastle = false;
        m_isEnPassant = false;
        m_pawnPromotionPiece = null;
        m_ambiguityType = 0;
    }

    /**
     * Constructor to parse a move in PGN format.
     *
     * @param move The PGN formatted string.
     * @param player The color of the player who generated the move.
     */
    public Move(String move, Color player)
    {
        // Parse for castle
        if (move.equals("O-O"))
        {
            m_isKingsideCastle = true;

            m_startFile = Constants.FILE_E;
            m_endFile = Constants.FILE_G;

            if (player == Color.WHITE)
            {
                m_movingPiece = Piece.WHITE_KING;
                m_startRank = Constants.RANK_1;
                m_endRank = Constants.RANK_1;
            }
            else
            {
                m_movingPiece = Piece.BLACK_KING;
                m_startRank = Constants.RANK_8;
                m_endRank = Constants.RANK_8;
            }

            return;
        }
        else if (move.equals("O-O-O"))
        {
            m_isQueensideCastle = true;

            m_startFile = Constants.FILE_E;
            m_endFile = Constants.FILE_C;

            if (player == Color.WHITE)
            {
                m_movingPiece = Piece.WHITE_KING;
                m_startRank = Constants.RANK_1;
                m_endRank = Constants.RANK_1;
            }
            else
            {
                m_movingPiece = Piece.BLACK_KING;
                m_startRank = Constants.RANK_8;
                m_endRank = Constants.RANK_8;
            }

            return;
        }

        int ix = 0;

        // Parse moving piece
        if (move.startsWith("N"))
        {
            ix++;
            m_movingPiece = (player == Color.WHITE ? Piece.WHITE_KNIGHT : Piece.BLACK_KNIGHT);
        }
        else if (move.startsWith("B"))
        {
            ix++;
            m_movingPiece = (player == Color.WHITE ? Piece.WHITE_BISHOP : Piece.BLACK_BISHOP);
        }
        else if (move.startsWith("R"))
        {
            ix++;
            m_movingPiece = (player == Color.WHITE ? Piece.WHITE_ROOK : Piece.BLACK_ROOK);
        }
        else if (move.startsWith("Q"))
        {
            ix++;
            m_movingPiece = (player == Color.WHITE ? Piece.WHITE_QUEEN : Piece.BLACK_QUEEN);
        }
        else if (move.startsWith("K"))
        {
            ix++;
            m_movingPiece = (player == Color.WHITE ? Piece.WHITE_KING : Piece.BLACK_KING);
        }
        else
        {
            m_movingPiece = (player == Color.WHITE ? Piece.WHITE_PAWN : Piece.BLACK_PAWN);
        }

        // Parse starting position
        m_startFile = move.charAt(ix++) - 'a';
        m_startRank = move.charAt(ix++) - '1';

        // Parse for capture
        if (move.contains("x"))
        {
            ix++;
            m_isCapture = true;
        }

        // Parse ending position
        m_endFile = move.charAt(ix++) - 'a';
        m_endRank = move.charAt(ix++) - '1';

        // Parse pawn promotion
        if (move.contains("=N"))
        {
            m_pawnPromotionPiece =
                (player == Color.WHITE ? Piece.WHITE_KNIGHT : Piece.BLACK_KNIGHT);
        }
        else if (move.contains("=B"))
        {
            m_pawnPromotionPiece =
                (player == Color.WHITE ? Piece.WHITE_BISHOP : Piece.BLACK_BISHOP);
        }
        else if (move.contains("=R"))
        {
            m_pawnPromotionPiece = (player == Color.WHITE ? Piece.WHITE_ROOK : Piece.BLACK_ROOK);
        }
        else if (move.contains("=Q"))
        {
            m_pawnPromotionPiece = (player == Color.WHITE ? Piece.WHITE_QUEEN : Piece.BLACK_QUEEN);
        }

        // Parse for enpassant
        if (move.contains("ep"))
        {
            m_isEnPassant = true;
        }

        // Parse for check / checkmate
        if (move.contains("+"))
        {
            m_isCheck = true;
        }
        else if (move.contains("#"))
        {
            m_isCheckmate = true;
        }
    }

    /**
     * @return A string representation of the move.
     */
    public String getPgnString(boolean preserveAmbiguity)
    {
        if (m_isKingsideCastle)
        {
            return "O-O";
        }
        else if (m_isQueensideCastle)
        {
            return "O-O-O";
        }

        StringBuilder move = new StringBuilder();

        // Set the moving piece
        switch (m_movingPiece)
        {
            case WHITE_KNIGHT:
            case BLACK_KNIGHT:
                move.append("N");
                break;
            case WHITE_BISHOP:
            case BLACK_BISHOP:
                move.append("B");
                break;
            case WHITE_ROOK:
            case BLACK_ROOK:
                move.append("R");
                break;
            case WHITE_QUEEN:
            case BLACK_QUEEN:
                move.append("Q");
                break;
            case WHITE_KING:
            case BLACK_KING:
                move.append("K");
                break;
            default:
                break;
        }

        // Append the start file and start rank
        if (!preserveAmbiguity || (m_ambiguityType & 1) > 0)
        {
            move.append((char)('a' + m_startFile));
        }
        if (!preserveAmbiguity || (m_ambiguityType & 2) > 0)
        {
            move.append(String.valueOf(m_startRank + 1));
        }

        // If move is capture, indicate such
        if (m_isCapture)
        {
            move.append("x");
        }

        // Append the end file and end rank
        move.append((char)('a' + m_endFile));
        move.append(String.valueOf(m_endRank + 1));

        // If move is a promotion, indicate the promotion piece
        if (m_pawnPromotionPiece != null)
        {
            if (m_pawnPromotionPiece != Piece.WHITE_PAWN &&
                m_pawnPromotionPiece != Piece.BLACK_PAWN)
            {
                switch (m_pawnPromotionPiece)
                {
                    case WHITE_KNIGHT:
                    case BLACK_KNIGHT:
                        move.append("=N");
                        break;
                    case WHITE_BISHOP:
                    case BLACK_BISHOP:
                        move.append("=B");
                        break;
                    case WHITE_ROOK:
                    case BLACK_ROOK:
                        move.append("=R");
                        break;
                    case WHITE_QUEEN:
                    case BLACK_QUEEN:
                        move.append("=Q");
                        break;
                    default:
                        break;
                }
            }
        }

        // If move is an enpassant, indicate such
        if (m_isEnPassant)
        {
            move.append("ep");
        }

        // If move is check or checkmate, indicate such
        if (m_isCheckmate)
        {
            move.append("#");
        }
        else if (m_isCheck)
        {
            move.append("+");
        }

        return move.toString();
    }

    /**
     * @return The rank of the start position.
     */
    public int getStartRank()
    {
        return m_startRank;
    }

    /**
     * @return The file of the start position.
     */
    public int getStartFile()
    {
        return m_startFile;
    }

    /**
     * @return The rank of the end position.
     */
    public int getEndRank()
    {
        return m_endRank;
    }

    /**
     * @return The file of the end position.
     */
    public int getEndFile()
    {
        return m_endFile;
    }

    /**
     * Set the moving piece for this move.
     *
     * @param movingPiece The moving piece.
     */
    public void setMovingPiece(Piece movingPiece)
    {
        this.m_movingPiece = movingPiece;
    }

    /**
     * @return The piece being moved.
     */
    public Piece getMovingPiece()
    {
        return m_movingPiece;
    }

    /**
     * If this move promotes a pawn, set the piece being promoted to.
     *
     * @param pawnPromotionPiece The piece to promote to.
     */
    public void setPawnPromotionPiece(Piece pawnPromotionPiece)
    {
        this.m_pawnPromotionPiece = pawnPromotionPiece;
    }

    /**
     * @return The piece to promote to, or null.
     */
    public Piece getPawnPromotionPiece()
    {
        return m_pawnPromotionPiece;
    }

    /**
     * Set that this move is a capturing move.
     */
    public void setCapture()
    {
        this.m_isCapture = true;
    }

    /**
     * @return True if the move is a capturing move, false otherwise.
     */
    public boolean isCapture()
    {
        return m_isCapture;
    }

    /**
     * Set that this move puts the opponent in check.
     */
    public void setCheck()
    {
        this.m_isCheck = true;
    }

    /**
     * @return True if the opponent is in check, false otherwise
     */
    public boolean isCheck()
    {
        return m_isCheck;
    }

    /**
     * Set that this move puts the opponent in checkmate.
     */
    public void setCheckmate()
    {
        this.m_isCheckmate = true;
    }

    /**
     * @return True if the opponent is in checkmate, false otherwise
     */
    public boolean isCheckmate()
    {
        return m_isCheckmate;
    }

    /**
     * Set that this move puts the board in stalemate.
     */
    public void setStalemate()
    {
        this.m_isStalemate = true;
    }

    /**
     * @return True if the board is in stalemate, false otherwise
     */
    public boolean isStalemate()
    {
        return m_isStalemate;
    }

    /**
     * Set that this move is an en passant.
     *
     * @param isEnPassant True if the move is an en passant, false otherwise.
     */
    public void setEnPassant()
    {
        this.m_isEnPassant = true;
    }

    /**
     * @return True if this move is an en passant.
     */
    public boolean isEnPassant()
    {
        return m_isEnPassant;
    }

    /**
     * Set that this move is a kingside castle.
     */
    public void setKingsideCastle()
    {
        this.m_isKingsideCastle = true;
    }

    /**
     * @return True if this move is a kingside castle.
     */
    public boolean isKingsideCastle()
    {
        return m_isKingsideCastle;
    }

    /**
     * Set that this move is a queenside castle.
     */
    public void setQueensideCastle()
    {
        this.m_isQueensideCastle = true;
    }

    /**
     * @return True if this move is a queenside castle.
     */
    public boolean isQueensideCastle()
    {
        return m_isQueensideCastle;
    }

    /**
     * Set the ambiguity type for the move. Use the following values:
     * <li>0 = No ambiguity.
     * <li>1 = Resolve ambiguity by indicating file.
     * <li>2 = Resolve ambiguity by indicating rank.
     * <li>3 = Resolve ambiguity by indicating file and rank.
     *
     * @param ambiguityType Integer representing the type of ambiguity.
     */
    public void setAmbiguityType(int ambiguityType)
    {
        m_ambiguityType = ambiguityType;
    }
}
