package com.flynn.chessmate.game;

import com.flynn.chessmate.gui.BoardGUI;
import com.flynn.chessmate.gui.SquareGUI;
import com.flynn.chessmate.movement.Move;
import com.flynn.chessmate.util.Constants;

import javax.swing.ImageIcon;
import javax.swing.JOptionPane;

import java.awt.Color;
import java.util.ArrayList;

/**
 * Class to represent the board.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
public class Board
{
    private SquareGUI[][] m_board;
    private ArrayList<Move> m_whiteMoves;
    private ArrayList<Move> m_blackMoves;

    private Color m_playerInTurn;

    private SquareGUI m_whiteKing;
    private SquareGUI m_blackKing;

    /**
     * Default constructor.
     */
    public Board()
    {
        m_playerInTurn = Color.WHITE;

        m_whiteMoves = new ArrayList<Move>();
        m_blackMoves = new ArrayList<Move>();

        m_board = new SquareGUI[Constants.NUM_RANKS][Constants.NUM_FILES];
        for (short i = 0; i < Constants.NUM_RANKS; i++)
        {
            for (short j = 0; j < Constants.NUM_FILES; j++)
            {
                m_board[i][j] = new SquareGUI(i, j, SquareGUI.getSquareImage(i, j));
            }
        }
        initializePieces();

        m_whiteKing = m_board[Constants.RANK_1][Constants.FILE_E];
        m_blackKing = m_board[Constants.RANK_8][Constants.FILE_E];
    }

    /**
     * @return The color of player in turn.
     */
    public Color getPlayerInTurn()
    {
        return m_playerInTurn;
    }

    /**
     * Signify a player is in check.
     *
     * @param playerInCheck The color of the player in check.
     */
    public void setCheck(Color playerInCheck)
    {
        if (playerInCheck == Color.WHITE)
        {
            m_whiteKing.checkSquare();
        }
        else if (playerInCheck == Color.BLACK)
        {
            m_blackKing.checkSquare();
        }
    }

    /**
     * Get a specific square.
     *
     * @param rank The rank of the tile.
     * @param file The file of the tile.
     * @return The desired tile.
     */
    public SquareGUI getSquare(int rank, int file)
    {
        return m_board[rank][file];
    }

    /**
     * @return A list of all moves made by the white player.
     */
    public ArrayList<Move> getWhiteMoves()
    {
        return m_whiteMoves;
    }

    /**
     * @return A list of all moves made by the black player.
     */
    public ArrayList<Move> getBlackMoves()
    {
        return m_blackMoves;
    }

    /**
     * Set the properities for a move.
     *
     * @param move The move to analyze.
     */
    public void setMoveStats(Move move)
    {
        SquareGUI start = m_board[move.getStartRank()][move.getStartFile()];
        SquareGUI end = m_board[move.getEndRank()][move.getEndFile()];

        // Set ambiguity for display
        setAmbiguityType(move);

        // Check if this is a capturing move
        Piece p = end.getPiece();
        if (p != null)
        {
            move.setCapture();
        }

        // Check if this was an en passant
        checkForEnPassant(move);

        // Set the moving piece
        move.setMovingPiece(start.getPiece());

        // Check if this was a castle
        checkForCastle(move);

        // Check if we can promote a pawn
        checkForPawnPromotion(move);
    }

    /**
     * Make a move that has been validated by the engine.
     *
     * @param move The move to execute.
     */
    public void makeMove(Move move)
    {
        SquareGUI start = m_board[move.getStartRank()][move.getStartFile()];
        SquareGUI end = m_board[move.getEndRank()][move.getEndFile()];

        // Set ambiguity for display
        setAmbiguityType(move);

        // Check if this is a capturing move
        if (move.isCapture())
        {
            // Check if this was an en passant
            if (move.isEnPassant())
            {
                if (m_playerInTurn == Color.WHITE)
                {
                    BoardGUI.addCapturedPiece(Piece.BLACK_PAWN);
                }
                else
                {
                    BoardGUI.addCapturedPiece(Piece.WHITE_PAWN);
                }

                SquareGUI ep = m_board[move.getStartRank()][move.getEndFile()];
                ep.setPiece(null);
            }
            else
            {
                Piece p = end.getPiece();
                BoardGUI.addCapturedPiece(p);
            }
        }

        // Check if this was a castle
        if (move.isKingsideCastle())
        {
            if (m_playerInTurn == Color.WHITE)
            {
                m_board[Constants.RANK_1][Constants.FILE_F].setPiece(Piece.WHITE_ROOK);
                m_board[Constants.RANK_1][Constants.FILE_H].setPiece(null);
            }
            else
            {
                m_board[Constants.RANK_8][Constants.FILE_F].setPiece(Piece.BLACK_ROOK);
                m_board[Constants.RANK_8][Constants.FILE_H].setPiece(null);
            }
        }
        else if (move.isQueensideCastle())
        {
            if (m_playerInTurn == Color.WHITE)
            {
                m_board[Constants.RANK_1][Constants.FILE_D].setPiece(Piece.WHITE_ROOK);
                m_board[Constants.RANK_1][Constants.FILE_A].setPiece(null);
            }
            else
            {
                m_board[Constants.RANK_8][Constants.FILE_D].setPiece(Piece.BLACK_ROOK);
                m_board[Constants.RANK_8][Constants.FILE_A].setPiece(null);
            }
        }

        // Move the piece
        end.setPiece(move.getMovingPiece());
        start.setPiece(null);

        if (m_playerInTurn == Color.WHITE)
        {
            m_whiteMoves.add(move);
        }
        else if (m_playerInTurn == Color.BLACK)
        {
            m_blackMoves.add(move);
        }

        // Check if we can promote a pawn
        Piece p = move.getPawnPromotionPiece();
        if (p != null)
        {
            end.setPiece(p);
        }

        // Change the player in turn
        m_playerInTurn = (m_playerInTurn == Color.WHITE ? Color.BLACK : Color.WHITE);

        // Highlight the start/end squares
        SquareGUI.resetAllSquares(this, true);
        start.moveSquare();
        end.moveSquare();

        // Update king location
        if (start.equals(m_whiteKing))
        {
            m_whiteKing = end;
        }
        else if (start.equals(m_blackKing))
        {
            m_blackKing = end;
        }

        // Update GUI
        BoardGUI.updateDisplay();
    }

    /**
     * Initialize the board to the standard setup.
     */
    private void initializePieces()
    {
        // Pawns
        for (int i = 0; i < Constants.NUM_FILES; i++)
        {
            m_board[Constants.RANK_2][i].setPiece(Piece.WHITE_PAWN);
            m_board[Constants.RANK_7][i].setPiece(Piece.BLACK_PAWN);
        }

        // Rooks
        m_board[Constants.RANK_1][Constants.FILE_A].setPiece(Piece.WHITE_ROOK);
        m_board[Constants.RANK_1][Constants.FILE_H].setPiece(Piece.WHITE_ROOK);
        m_board[Constants.RANK_8][Constants.FILE_A].setPiece(Piece.BLACK_ROOK);
        m_board[Constants.RANK_8][Constants.FILE_H].setPiece(Piece.BLACK_ROOK);

        // Knights
        m_board[Constants.RANK_1][Constants.FILE_B].setPiece(Piece.WHITE_KNIGHT);
        m_board[Constants.RANK_1][Constants.FILE_G].setPiece(Piece.WHITE_KNIGHT);
        m_board[Constants.RANK_8][Constants.FILE_B].setPiece(Piece.BLACK_KNIGHT);
        m_board[Constants.RANK_8][Constants.FILE_G].setPiece(Piece.BLACK_KNIGHT);

        // Bishops
        m_board[Constants.RANK_1][Constants.FILE_C].setPiece(Piece.WHITE_BISHOP);
        m_board[Constants.RANK_1][Constants.FILE_F].setPiece(Piece.WHITE_BISHOP);
        m_board[Constants.RANK_8][Constants.FILE_C].setPiece(Piece.BLACK_BISHOP);
        m_board[Constants.RANK_8][Constants.FILE_F].setPiece(Piece.BLACK_BISHOP);

        // Queens
        m_board[Constants.RANK_1][Constants.FILE_D].setPiece(Piece.WHITE_QUEEN);
        m_board[Constants.RANK_8][Constants.FILE_D].setPiece(Piece.BLACK_QUEEN);

        // Kings
        m_board[Constants.RANK_1][Constants.FILE_E].setPiece(Piece.WHITE_KING);
        m_board[Constants.RANK_8][Constants.FILE_E].setPiece(Piece.BLACK_KING);
    }

    /**
     * Check if a pawn can be promoted.
     *
     * @param m_playerInTurn The color of the player in turn.
     */
    private void checkForPawnPromotion(Move move)
    {
        int homeRank = (m_playerInTurn == Color.WHITE ? 7 : 0);
        int fromRank = (m_playerInTurn == Color.WHITE ? 6 : 1);

        if (move.getEndRank() == homeRank && move.getStartRank() == fromRank)
        {
            SquareGUI s = m_board[move.getStartRank()][move.getStartFile()];
            Piece pawn = (m_playerInTurn == Color.WHITE ? Piece.WHITE_PAWN : Piece.BLACK_PAWN);

            if (s.getPiece() == pawn)
            {
                ImageIcon[] buttons;
                int p, offset = 0;

                if (m_playerInTurn == Color.WHITE)
                {
                    buttons = new ImageIcon[] {
                        Constants.WKNIGHT_TRANSPARENT,
                        Constants.WBISHOP_TRANSPARENT,
                        Constants.WROOK_TRANSPARENT,
                        Constants.WQUEEN_TRANSPARENT};
                }
                else
                {
                    offset = 4;
                    buttons = new ImageIcon[] {
                        Constants.BKNIGHT_TRANSPARENT,
                        Constants.BBISHOP_TRANSPARENT,
                        Constants.BROOK_TRANSPARENT,
                        Constants.BQUEEN_TRANSPARENT};
                }

                do
                {
                    p = offset +
                        JOptionPane.showOptionDialog(
                            null,
                            "Select a piece",
                            "Pawn Promotion",
                            0,
                            JOptionPane.INFORMATION_MESSAGE,
                            null,
                            buttons,
                            0);
                } while (p == 0);

                switch (p)
                {
                    case 0:
                        move.setPawnPromotionPiece(Piece.WHITE_KNIGHT);
                        break;
                    case 1:
                        move.setPawnPromotionPiece(Piece.WHITE_BISHOP);
                        break;
                    case 2:
                        move.setPawnPromotionPiece(Piece.WHITE_ROOK);
                        break;
                    case 3:
                        move.setPawnPromotionPiece(Piece.WHITE_QUEEN);
                        break;
                    case 4:
                        move.setPawnPromotionPiece(Piece.BLACK_KNIGHT);
                        break;
                    case 5:
                        move.setPawnPromotionPiece(Piece.BLACK_BISHOP);
                        break;
                    case 6:
                        move.setPawnPromotionPiece(Piece.BLACK_ROOK);
                        break;
                    case 7:
                        move.setPawnPromotionPiece(Piece.BLACK_QUEEN);
                        break;
                }
            }
        }
    }

    /**
     * Check if a move was a castling move.
     *
     * @param move The move to check.
     */
    private void checkForCastle(Move move)
    {
        int sRank = move.getStartRank();
        int sFile = move.getStartFile();
        int eFile = move.getEndFile();

        Piece p = m_board[sRank][sFile].getPiece();

        if (p == Piece.WHITE_KING)
        {
            if (sRank == 0 && sFile == 4)
            {
                if (eFile == 6)
                {
                    move.setKingsideCastle();
                }
                else if (eFile == 2)
                {
                    move.setQueensideCastle();
                }
            }
        }
        else if (p == Piece.BLACK_KING)
        {
            if (sRank == 7 && sFile == 4)
            {
                if (eFile == 6)
                {
                    move.setKingsideCastle();
                }
                else if (eFile == 2)
                {
                    move.setQueensideCastle();
                }
            }
        }
    }

    /**
     * Check if a move was an en passant move.
     *
     * @param move The move to check.
     */
    private void checkForEnPassant(Move move)
    {
        int sRank = move.getStartRank();
        int sFile = move.getStartFile();
        int eRank = move.getEndRank();
        int eFile = move.getEndFile();

        Piece movingPiece = m_board[sRank][sFile].getPiece();
        Piece epPiece = m_board[sRank][eFile].getPiece();
        Piece destPiece = m_board[eRank][eFile].getPiece();

        if (epPiece == null || destPiece != null)
        {
            return;
        }

        if (movingPiece == Piece.WHITE_PAWN && epPiece == Piece.BLACK_PAWN)
        {
            if (sRank == 4)
            {
                if ((eFile == sFile - 1) || (eFile == sFile + 1))
                {
                    move.setCapture();
                    move.setEnPassant();
                }
            }
        }
        else if (movingPiece == Piece.BLACK_PAWN && epPiece == Piece.WHITE_PAWN)
        {
            if (sRank == 3)
            {
                if ((eFile == sFile - 1) || (eFile == sFile + 1))
                {
                    move.setCapture();
                    move.setEnPassant();
                }
            }
        }
    }

    /**
     * Set the ambiguity type for a move.
     *
     * @param move The move being made.
     */
    private void setAmbiguityType(Move move)
    {
        int sRank = move.getStartRank();
        int sFile = move.getStartFile();
        int eRank = move.getEndRank();
        int eFile = move.getEndFile();

        Piece movingPiece = m_board[sRank][sFile].getPiece();

        // If the moving piece is a pawn and is capturing, set file ambiguity
        if (move.isCapture() &&
            (movingPiece == Piece.WHITE_PAWN || movingPiece == Piece.BLACK_PAWN))
        {
            move.setAmbiguityType(1);
            return;
        }

        // Otherwise, check for other ambiguities
        for (int i = 0; i < Constants.NUM_RANKS; i++)
        {
            for (int j = 0; j < Constants.NUM_FILES; j++)
            {
                Piece ambiguousPiece = m_board[i][j].getPiece();

                // If the piece types are the same, but not on the same tile
                if ((movingPiece == ambiguousPiece) && ((i != sRank) || (j != sFile)))
                {
                    int rise = Math.abs(i - eRank);
                    int run = Math.abs(j - eFile);

                    switch (movingPiece)
                    {
                        case WHITE_PAWN:
                            if (m_board[eRank][eFile].getPiece() != null)
                            {
                                if (i + 1 == eRank)
                                {
                                    if (eFile == j - 1 || eFile == j + 1)
                                    {
                                        move.setAmbiguityType(1);
                                    }
                                }
                            }
                            break;

                        case BLACK_PAWN:
                            if (m_board[eRank][eFile].getPiece() != null)
                            {
                                if (i - 1 == eRank)
                                {
                                    if (eFile == j - 1 || eFile == j + 1)
                                    {
                                        move.setAmbiguityType(1);
                                    }
                                }
                            }
                            break;

                        case WHITE_KNIGHT:
                        case BLACK_KNIGHT:
                            if ((rise == 2 && run == 1) || (rise == 1 && run == 2))
                            {
                                if (j != sFile)
                                {
                                    move.setAmbiguityType(1);
                                }
                                else if (i != sRank)
                                {
                                    move.setAmbiguityType(2);
                                }
                            }
                            break;

                        case WHITE_BISHOP:
                        case BLACK_BISHOP:
                            if (rise == run)
                            {
                                if (spacesBetweenAreEmpty(i, j, eRank, eFile, movingPiece))
                                {
                                    if (j != sFile)
                                    {
                                        move.setAmbiguityType(1);
                                    }
                                    else if (i != sRank)
                                    {
                                        move.setAmbiguityType(2);
                                    }
                                }
                            }
                            break;

                        case WHITE_ROOK:
                        case BLACK_ROOK:
                            if (rise == 0 || run == 0)
                            {
                                if (spacesBetweenAreEmpty(i, j, eRank, eFile, movingPiece))
                                {
                                    if (j != sFile)
                                    {
                                        move.setAmbiguityType(1);
                                    }
                                    else if (i != sRank)
                                    {
                                        move.setAmbiguityType(2);
                                    }
                                }
                            }
                            break;

                        case WHITE_QUEEN:
                        case BLACK_QUEEN:
                            if ((rise == run) || (rise == 0 || run == 0))
                            {
                                if (spacesBetweenAreEmpty(i, j, eRank, eFile, movingPiece))
                                {
                                    if (j != sFile)
                                    {
                                        move.setAmbiguityType(1);
                                    }
                                    else if (i != sRank)
                                    {
                                        move.setAmbiguityType(2);
                                    }
                                }
                            }
                            break;

                        default:
                            break;
                    }
                }
            }
        }
    }

    /**
     * Check that the spaces between two tiles are empty.
     *
     * @param sRank The rank of the starting tile.
     * @param sFile The file of the starting tile.
     * @param eRank The rank of the ending tile.
     * @param eFile The rank of the ending file.
     * @param piece The piece being moved.
     * @return True if the spaces between are empty, false otherwise.
     */
    private boolean spacesBetweenAreEmpty(int sRank, int sFile, int eRank, int eFile, Piece piece)
    {
        int rise = sRank - eRank;
        int run = sFile - eFile;

        boolean isBishop = (piece == Piece.WHITE_BISHOP || piece == Piece.BLACK_BISHOP);
        boolean isRook = (piece == Piece.WHITE_ROOK || piece == Piece.BLACK_ROOK);
        boolean isQueen = (piece == Piece.WHITE_QUEEN || piece == Piece.BLACK_QUEEN);

        // Rook or queen - lateral
        if (isRook || isQueen)
        {
            // North
            if (rise > 0 && run == 0)
            {
                for (int i = sRank - 1; i > eRank; i--)
                {
                    if (m_board[i][sFile].getPiece() != null)
                    {
                        return false;
                    }
                }
            }

            // South
            if (rise < 0 && run == 0)
            {
                for (int i = sRank + 1; i < eRank; i++)
                {
                    if (m_board[i][sFile].getPiece() != null)
                    {
                        return false;
                    }
                }
            }

            // East
            if (rise == 0 && run < 0)
            {
                for (int j = sFile + 1; j < eFile; j++)
                {
                    if (m_board[sRank][j].getPiece() != null)
                    {
                        return false;
                    }
                }
            }

            // West
            if (rise == 0 && run > 0)
            {
                for (int j = sFile - 1; j > eFile; j--)
                {
                    if (m_board[sRank][j].getPiece() != null)
                    {
                        return false;
                    }
                }
            }
        }

        // Bishop or queen - diagonal
        if (isBishop || isQueen)
        {
            // North-west
            if (rise > 0 && run > 0)
            {
                for (int i = sRank - 1, j = sFile - 1; i > eRank && j > eFile; i--, j--)
                {
                    if (m_board[i][j].getPiece() != null)
                    {
                        return false;
                    }
                }
            }

            // North-east
            if (rise > 0 && run < 0)
            {
                for (int i = sRank - 1, j = sFile + 1; i > eRank && j < eFile; i--, j++)
                {
                    if (m_board[i][j].getPiece() != null)
                    {
                        return false;
                    }
                }
            }

            // South-west
            if (rise < 0 && run > 0)
            {
                for (int i = sRank + 1, j = sFile - 1; i < eRank && j > eFile; i++, j--)
                {
                    if (m_board[i][j].getPiece() != null)
                    {
                        return false;
                    }
                }
            }

            // South-east
            if (rise < 0 && run < 0)
            {
                for (int i = sRank + 1, j = sFile + 1; i < eRank && j < eFile; i++, j++)
                {
                    if (m_board[i][j].getPiece() != null)
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }
}
