package com.flynn.chessmate.util;

import com.flynn.chessmate.movement.Move;

import javax.swing.table.AbstractTableModel;

import java.util.ArrayList;

/**
 * Model for the table containing the moves.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version August 2, 2011
 */
public class TableModel extends AbstractTableModel
{
    private static final long serialVersionUID = 1L;

    private ArrayList<Move> m_whiteMoves;
    private ArrayList<Move> m_blackMoves;

    /**
     * Initializes the TableModel.
     */
    public TableModel(ArrayList<Move> whiteMoves, ArrayList<Move> blackMoves)
    {
        m_whiteMoves = whiteMoves;
        m_blackMoves = blackMoves;
    }

    /**
     * @return The number of columns of this TableModel.
     */
    @Override
    public int getColumnCount()
    {
        return 3;
    }

    /**
     * @return The number of rows of this TableModel.
     */
    @Override
    public int getRowCount()
    {
        return (m_whiteMoves.size());
    }

    /**
     * Retrieves a desired value from this TableModel.
     *
     * @param rowIndex The row index of the desired value.
     * @param columnIndex The column index of the desired value.
     * @return The desired Object.
     */
    @Override
    public Object getValueAt(int rowIndex, int columnIndex)
    {
        switch (columnIndex)
        {
            case 0: // Move number
                return (rowIndex + 1);
            case 1: // White move
                int sz = m_whiteMoves.size();
                if (sz > rowIndex)
                {
                    Move m = m_whiteMoves.get(rowIndex);
                    return m.getPgnString(true);
                }
                return "";
            case 2: // Black move
                sz = m_blackMoves.size();
                if (sz > rowIndex)
                {
                    Move m = m_blackMoves.get(rowIndex);
                    return m.getPgnString(true);
                }
                return "";
            default: // ? Don't know what happened.
                return null;
        }
    }
}
