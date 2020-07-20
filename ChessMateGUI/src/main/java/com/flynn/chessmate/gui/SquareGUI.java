package com.flynn.chessmate.gui;

import com.flynn.chessmate.game.Board;
import com.flynn.chessmate.game.Piece;
import com.flynn.chessmate.util.Constants;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;

/**
 * Class to represent a square on the board. Stores location, color, and piece.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
public class SquareGUI extends JButton
{
    private static final long serialVersionUID = 1L;

    private int m_rank;
    private int m_file;
    private Piece m_piece = null;

    private ImageIcon m_square = null;

    /**
     * Constructor.
     *
     * @param rank The rank of the square.
     * @param file The file of the square.
     * @param square The image of the square.
     */
    public SquareGUI(int rank, int file, ImageIcon square)
    {
        m_rank = rank;
        m_file = file;

        setSquareImage(square);
        setBorder(BorderFactory.createRaisedBevelBorder());
    }

    /**
     * @return The rank of the square.
     */
    public int getRank()
    {
        return m_rank;
    }

    /**
     * @return The file of the square.
     */
    public int getFile()
    {
        return m_file;
    }

    /**
     * Set the piece the square contains.
     *
     * @param piece The new piece.
     */
    public void setPiece(Piece piece)
    {
        this.m_piece = piece;
        setSquareImage(getSquareImage(this));
    }

    /**
     * @return The piece the square contains.
     */
    public Piece getPiece()
    {
        return m_piece;
    }

    /**
     * Get the (non-special) image of a square.
     *
     * @param square The square to get the image from.
     * @return The image of the square.
     */
    public static ImageIcon getSquareImage(SquareGUI square)
    {
        boolean whiteTile = ((square.getRank() + square.getFile()) % 2 == 1);

        Piece p = square.getPiece();

        if (p != null)
        {
            switch (square.getPiece())
            {
                case WHITE_PAWN:
                    return (whiteTile ? Constants.WPAWN_WTILE : Constants.WPAWN_BTILE);
                case WHITE_KNIGHT:
                    return (whiteTile ? Constants.WKNIGHT_WTILE : Constants.WKNIGHT_BTILE);
                case WHITE_BISHOP:
                    return (whiteTile ? Constants.WBISHOP_WTILE : Constants.WBISHOP_BTILE);
                case WHITE_ROOK:
                    return (whiteTile ? Constants.WROOK_WTILE : Constants.WROOK_BTILE);
                case WHITE_QUEEN:
                    return (whiteTile ? Constants.WQUEEN_WTILE : Constants.WQUEEN_BTILE);
                case WHITE_KING:
                    return (whiteTile ? Constants.WKING_WTILE : Constants.WKING_BTILE);
                case BLACK_PAWN:
                    return (whiteTile ? Constants.BPAWN_WTILE : Constants.BPAWN_BTILE);
                case BLACK_KNIGHT:
                    return (whiteTile ? Constants.BKNIGHT_WTILE : Constants.BKNIGHT_BTILE);
                case BLACK_BISHOP:
                    return (whiteTile ? Constants.BBISHOP_WTILE : Constants.BBISHOP_BTILE);
                case BLACK_ROOK:
                    return (whiteTile ? Constants.BROOK_WTILE : Constants.BROOK_BTILE);
                case BLACK_QUEEN:
                    return (whiteTile ? Constants.BQUEEN_WTILE : Constants.BQUEEN_BTILE);
                case BLACK_KING:
                    return (whiteTile ? Constants.BKING_WTILE : Constants.BKING_BTILE);
            }
        }

        return (whiteTile ? Constants.EMPTY_WTILE : Constants.EMPTY_BTILE);
    }

    /**
     * Get the image of a square. Will be an "empty" image, used for Tile
     * construction.
     *
     * @param rank The rank of the square.
     * @param file The color of the square.
     * @return The image of the square.
     */
    public static ImageIcon getSquareImage(int rank, int file)
    {
        boolean whiteTile = ((rank + file) % 2 == 1);
        return (whiteTile ? Constants.EMPTY_WTILE : Constants.EMPTY_BTILE);
    }

    /**
     * Reset all squares to original background color.
     *
     * A square is special if it is highlighted as being part of a move or
     * being in check.
     *
     * @param board The board to modify.
     * @param ignoreSpecial True if speciality of a square should be ignored.
     */
    public static void resetAllSquares(Board board, boolean ignoreSpecial)
    {
        for (int i = 0; i < Constants.NUM_RANKS; i++)
        {
            for (int j = 0; j < Constants.NUM_FILES; j++)
            {
                SquareGUI t = board.getSquare(i, j);

                if (ignoreSpecial || t.shouldReset())
                {
                    t.setSquareImage(SquareGUI.getSquareImage(t));
                }
            }
        }
    }

    /**
     * Hover over the square.
     */
    public void hoverSquare()
    {
        if (!shouldReset())
        {
            return;
        }

        if (m_piece != null)
        {
            switch (m_piece)
            {
                case WHITE_PAWN:
                    setSquareImage(Constants.WPAWN_HTILE);
                    return;
                case WHITE_KNIGHT:
                    setSquareImage(Constants.WKNIGHT_HTILE);
                    return;
                case WHITE_BISHOP:
                    setSquareImage(Constants.WBISHOP_HTILE);
                    return;
                case WHITE_ROOK:
                    setSquareImage(Constants.WROOK_HTILE);
                    return;
                case WHITE_QUEEN:
                    setSquareImage(Constants.WQUEEN_HTILE);
                    return;
                case WHITE_KING:
                    setSquareImage(Constants.WKING_HTILE);
                    return;
                case BLACK_PAWN:
                    setSquareImage(Constants.BPAWN_HTILE);
                    return;
                case BLACK_KNIGHT:
                    setSquareImage(Constants.BKNIGHT_HTILE);
                    return;
                case BLACK_BISHOP:
                    setSquareImage(Constants.BBISHOP_HTILE);
                    return;
                case BLACK_ROOK:
                    setSquareImage(Constants.BROOK_HTILE);
                    return;
                case BLACK_QUEEN:
                    setSquareImage(Constants.BQUEEN_HTILE);
                    return;
                case BLACK_KING:
                    setSquareImage(Constants.BKING_HTILE);
                    return;
            }
        }

        setSquareImage(Constants.EMPTY_HTILE);
    }

    /**
     * Reset the square to its normal image.
     */
    public void resetSquare()
    {
        if (shouldReset())
        {
            setSquareImage(getSquareImage(this));
        }
    }

    /**
     * Select the square.
     */
    public void selectSquare()
    {
        if (m_piece != null)
        {
            switch (m_piece)
            {
                case WHITE_PAWN:
                    setSquareImage(Constants.WPAWN_STILE);
                    return;
                case WHITE_KNIGHT:
                    setSquareImage(Constants.WKNIGHT_STILE);
                    return;
                case WHITE_BISHOP:
                    setSquareImage(Constants.WBISHOP_STILE);
                    return;
                case WHITE_ROOK:
                    setSquareImage(Constants.WROOK_STILE);
                    return;
                case WHITE_QUEEN:
                    setSquareImage(Constants.WQUEEN_STILE);
                    return;
                case WHITE_KING:
                    setSquareImage(Constants.WKING_STILE);
                    return;
                case BLACK_PAWN:
                    setSquareImage(Constants.BPAWN_STILE);
                    return;
                case BLACK_KNIGHT:
                    setSquareImage(Constants.BKNIGHT_STILE);
                    return;
                case BLACK_BISHOP:
                    setSquareImage(Constants.BBISHOP_STILE);
                    return;
                case BLACK_ROOK:
                    setSquareImage(Constants.BROOK_STILE);
                    return;
                case BLACK_QUEEN:
                    setSquareImage(Constants.BQUEEN_STILE);
                    return;
                case BLACK_KING:
                    setSquareImage(Constants.BKING_STILE);
                    return;
            }
        }

        setSquareImage(Constants.EMPTY_STILE);
    }

    /**
     * Highlight the square as having been part of a move.
     */
    public void moveSquare()
    {
        if (m_piece != null)
        {
            switch (m_piece)
            {
                case WHITE_PAWN:
                    setSquareImage(Constants.WPAWN_MTILE);
                    return;
                case WHITE_KNIGHT:
                    setSquareImage(Constants.WKNIGHT_MTILE);
                    return;
                case WHITE_BISHOP:
                    setSquareImage(Constants.WBISHOP_MTILE);
                    return;
                case WHITE_ROOK:
                    setSquareImage(Constants.WROOK_MTILE);
                    return;
                case WHITE_QUEEN:
                    setSquareImage(Constants.WQUEEN_MTILE);
                    return;
                case WHITE_KING:
                    setSquareImage(Constants.WKING_MTILE);
                    return;
                case BLACK_PAWN:
                    setSquareImage(Constants.BPAWN_MTILE);
                    return;
                case BLACK_KNIGHT:
                    setSquareImage(Constants.BKNIGHT_MTILE);
                    return;
                case BLACK_BISHOP:
                    setSquareImage(Constants.BBISHOP_MTILE);
                    return;
                case BLACK_ROOK:
                    setSquareImage(Constants.BROOK_MTILE);
                    return;
                case BLACK_QUEEN:
                    setSquareImage(Constants.BQUEEN_MTILE);
                    return;
                case BLACK_KING:
                    setSquareImage(Constants.BKING_MTILE);
                    return;
            }
        }

        setSquareImage(Constants.EMPTY_MTILE);
    }

    /**
     * Highlight the square (if it is a king) as being in check.
     */
    public void checkSquare()
    {
        if (m_piece != null)
        {
            if (m_piece.equals(Piece.WHITE_KING))
            {
                setSquareImage(Constants.WKING_CTILE);
            }
            else if (m_piece.equals(Piece.BLACK_KING))
            {
                setSquareImage(Constants.BKING_CTILE);
            }
        }
    }

    /**
     * Set the image of the square.
     */
    private void setSquareImage(ImageIcon square)
    {
        m_square = square;

        setIcon(square);
        setDisabledIcon(square);
    }

    /**
     * @return True if the square's highlighting is non-special.
     */
    private boolean shouldReset()
    {
        if (m_square.equals(Constants.WPAWN_MTILE) || m_square.equals(Constants.WKNIGHT_MTILE) ||
            m_square.equals(Constants.WBISHOP_MTILE) || m_square.equals(Constants.WROOK_MTILE) ||
            m_square.equals(Constants.WQUEEN_MTILE) || m_square.equals(Constants.WKING_MTILE) ||
            m_square.equals(Constants.BPAWN_MTILE) || m_square.equals(Constants.BKNIGHT_MTILE) ||
            m_square.equals(Constants.BBISHOP_MTILE) || m_square.equals(Constants.BROOK_MTILE) ||
            m_square.equals(Constants.BQUEEN_MTILE) || m_square.equals(Constants.BKING_MTILE) ||
            m_square.equals(Constants.EMPTY_MTILE))
        {
            return false;
        }

        else if (m_square.equals(Constants.WKING_CTILE) || m_square.equals(Constants.BKING_CTILE))
        {
            return false;
        }

        return true;
    }
}
