package com.flynn.chessmate.gui;

import com.flynn.chessmate.communication.Message;
import com.flynn.chessmate.communication.Message.MessageType;
import com.flynn.chessmate.game.Board;
import com.flynn.chessmate.game.ChessGame;
import com.flynn.chessmate.game.Piece;
import com.flynn.chessmate.movement.Move;
import com.flynn.chessmate.util.Constants;
import com.flynn.chessmate.util.ImagePanel;
import com.flynn.chessmate.util.PgnFileFilter;
import com.flynn.chessmate.util.PgnGenerator;
import com.flynn.chessmate.util.TableModel;

import org.freixas.tablelayout.TableLayout;

import javax.swing.BorderFactory;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.WindowConstants;
import javax.swing.table.DefaultTableColumnModel;
import javax.swing.table.TableColumn;
import javax.swing.table.TableColumnModel;

import java.awt.Adjustable;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Calendar;

/**
 * GUI for the chess board.
 *
 * @author Timothy Flynn (trflynn89@pm.me)
 * @version March 3, 2013
 */
public class BoardGUI implements ActionListener, MouseListener
{
    private static JFrame m_frame;

    private static ChessGame m_game;
    private static boolean m_acceptMoves;

    private static SquareGUI m_selected;
    private static SquareGUI m_hovered;
    ;

    private static JTable m_table;

    private static JLabel m_whitePawnsCaptured;
    private static JLabel m_whiteKnightsCaptured;
    private static JLabel m_whiteBishopsCaptured;
    private static JLabel m_whiteRooksCaptured;
    private static JLabel m_whiteQueensCaptured;

    private static JLabel m_blackPawnsCaptured;
    private static JLabel m_blackKnightsCaptured;
    private static JLabel m_blackBishopsCaptured;
    private static JLabel m_blackRooksCaptured;
    private static JLabel m_blackQueensCaptured;

    private static JLabel m_status;

    /**
     * Constructor.
     */
    public BoardGUI(ChessGame game)
    {
        m_game = game;
    }

    /**
     * Update the elements of this GUI.
     */
    public static void updateDisplay()
    {
        if (m_table != null)
        {
            m_table.revalidate();
            m_table.repaint();
        }
    }

    /**
     * Should the GUI accept moves from the player(s)?
     *
     * @param acceptMoves True if moves should be accepted.
     */
    public static void acceptMoves(boolean acceptMoves)
    {
        m_acceptMoves = acceptMoves;
    }

    /**
     * Change the cursor to either a normal cursor or a wait cursor.
     *
     * @param wait True if the cursor should be a wait cursor.
     */
    public static void setWaitCursor(boolean wait)
    {
        if (m_frame != null)
        {
            if (wait)
            {
                m_frame.setCursor(new Cursor(Cursor.WAIT_CURSOR));
            }
            else
            {
                m_frame.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
            }
        }
    }

    /**
     * Increment a piece's capture count.
     *
     * @param piece The piece to increment.
     */
    public static void addCapturedPiece(Piece piece)
    {
        switch (piece)
        {
            case WHITE_PAWN:
                int curr = Integer.parseInt(m_whitePawnsCaptured.getText());
                m_whitePawnsCaptured.setText(String.valueOf(curr + 1));
                break;
            case WHITE_KNIGHT:
                curr = Integer.parseInt(m_whiteKnightsCaptured.getText());
                m_whiteKnightsCaptured.setText(String.valueOf(curr + 1));
                break;
            case WHITE_BISHOP:
                curr = Integer.parseInt(m_whiteBishopsCaptured.getText());
                m_whiteBishopsCaptured.setText(String.valueOf(curr + 1));
                break;
            case WHITE_ROOK:
                curr = Integer.parseInt(m_whiteRooksCaptured.getText());
                m_whiteRooksCaptured.setText(String.valueOf(curr + 1));
                break;
            case WHITE_QUEEN:
                curr = Integer.parseInt(m_whiteQueensCaptured.getText());
                m_whiteQueensCaptured.setText(String.valueOf(curr + 1));
                break;
            case BLACK_PAWN:
                curr = Integer.parseInt(m_blackPawnsCaptured.getText());
                m_blackPawnsCaptured.setText(String.valueOf(curr + 1));
                break;
            case BLACK_KNIGHT:
                curr = Integer.parseInt(m_blackKnightsCaptured.getText());
                m_blackKnightsCaptured.setText(String.valueOf(curr + 1));
                break;
            case BLACK_BISHOP:
                curr = Integer.parseInt(m_blackBishopsCaptured.getText());
                m_blackBishopsCaptured.setText(String.valueOf(curr + 1));
                break;
            case BLACK_ROOK:
                curr = Integer.parseInt(m_blackRooksCaptured.getText());
                m_blackRooksCaptured.setText(String.valueOf(curr + 1));
                break;
            case BLACK_QUEEN:
                curr = Integer.parseInt(m_blackQueensCaptured.getText());
                m_blackQueensCaptured.setText(String.valueOf(curr + 1));
                break;
            default:
                break;
        }
    }

    /**
     * Set the game status (check, checkmate, stalemate).
     *
     * @param status The status to set.
     */
    public static void setStatus(String status)
    {
        if (m_status != null)
        {
            m_status.setText(status);
        }
    }

    /**
     * Initialize all components of the GUI, and show it.
     */
    public void createAndShowGUI()
    {
        JPanel panel = new JPanel(new TableLayout("cols=2 tileft=10 tiright=10 ileft=10"));
        panel.add(createTiles());
        panel.add(createMoveTable(), "rspan=2");
        panel.add(createInfoPanel());

        m_frame = new JFrame("ChessMate");
        m_frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        m_frame.setIconImage(Constants.BROOK_TRANSPARENT.getImage());
        m_frame.setJMenuBar(createMenuControls());
        m_frame.add(panel);
        m_frame.pack();
        m_frame.setSize(Constants.BOARD_WIDTH, Constants.BOARD_HEIGHT);
        m_frame.setLocationRelativeTo(null);
        m_frame.setResizable(false);
        m_frame.setVisible(true);

        m_frame.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e)
            {
                m_game.disconnectFromServer();
            }
        });
    }

    /**
     * Initializes all components of the <code>JMenuBar</code>.
     *
     * @return The created <code>JMenuBar</code>.
     */
    private JMenuBar createMenuControls()
    {
        JMenuBar menu = new JMenuBar();
        JMenu file, help;
        JMenuItem game, pgn, quit, info, about;

        menu.setBackground(Constants.BACKGROUND);

        // FILE
        file = new JMenu("File");
        file.setBackground(Constants.BACKGROUND);

        // New game
        game = new JMenuItem("New Game");
        game.setBackground(Constants.BACKGROUND);
        game.setAccelerator(Constants.CTRL0);
        game.setActionCommand(Constants.NEW_GAME);
        game.addActionListener(this);
        file.add(game);

        // View moves
        pgn = new JMenuItem("Save PGN");
        pgn.setBackground(Constants.BACKGROUND);
        pgn.setAccelerator(Constants.CTRL1);
        pgn.setActionCommand(Constants.PGN);
        pgn.addActionListener(this);
        file.add(pgn);

        // Quit
        file.addSeparator();
        quit = new JMenuItem("Quit");
        quit.setBackground(Constants.BACKGROUND);
        quit.setAccelerator(Constants.ESC);
        quit.setActionCommand(Constants.QUIT);
        quit.addActionListener(this);
        file.add(quit);

        // HELP
        help = new JMenu("Help");
        help.setBackground(Constants.BACKGROUND);

        // About
        about = new JMenuItem("About ChessMate");
        about.setBackground(Constants.BACKGROUND);
        about.setAccelerator(Constants.ALT0);
        about.setActionCommand(Constants.ABOUT);
        about.addActionListener(this);
        help.add(about);

        // Info
        help.addSeparator();
        info = new JMenuItem("How To Use");
        info.setBackground(Constants.BACKGROUND);
        info.setAccelerator(Constants.F1);
        info.setActionCommand(Constants.INFO);
        info.addActionListener(this);
        help.add(info);

        menu.add(file);
        menu.add(help);
        menu.setBorder(BorderFactory.createEtchedBorder());

        return menu;
    }

    /**
     * Initialize each tile on the board.
     *
     * @return The JPanel holding the tiles.
     */
    private JPanel createTiles()
    {
        JPanel panel = new JPanel(new TableLayout("cols=9 tc"));
        panel.setBackground(Constants.BACKGROUND);

        Board b = m_game.getBoard();
        Color c = m_game.getPlayerColor();

        if (c == Color.WHITE)
        {
            for (int i = Constants.NUM_RANKS - 1; i >= 0; i--)
            {
                panel.add(new JLabel((i + 1) + "    "));

                for (int j = 0; j < Constants.NUM_FILES; j++)
                {
                    SquareGUI s = b.getSquare(i, j);
                    s.addMouseListener(this);
                    panel.add(s);
                }
            }
        }
        else
        {
            for (int i = 0; i < Constants.NUM_RANKS; i++)
            {
                panel.add(new JLabel((i + 1) + "    "));

                for (int j = Constants.NUM_FILES - 1; j >= 0; j--)
                {
                    SquareGUI s = b.getSquare(i, j);
                    s.addMouseListener(this);
                    panel.add(s);
                }
            }
        }

        String space = "              ";
        for (int i = 0; i < Constants.NUM_FILES; i++)
        {
            switch (i)
            {
                case 0:
                    if (c == Color.WHITE)
                    {
                        panel.add(new JLabel(space + "A"), "skip=1");
                    }
                    else
                    {
                        panel.add(new JLabel(space + "H"), "skip=1");
                    }
                    break;
                case 1:
                    if (c == Color.WHITE)
                    {
                        panel.add(new JLabel(space + "B"));
                    }
                    else
                    {
                        panel.add(new JLabel(space + "G"));
                    }
                    break;
                case 2:
                    if (c == Color.WHITE)
                    {
                        panel.add(new JLabel(space + "C"));
                    }
                    else
                    {
                        panel.add(new JLabel(space + "F"));
                    }
                    break;
                case 3:
                    if (c == Color.WHITE)
                    {
                        panel.add(new JLabel(space + "D"));
                    }
                    else
                    {
                        panel.add(new JLabel(space + "E"));
                    }
                    break;
                case 4:
                    if (c == Color.WHITE)
                    {
                        panel.add(new JLabel(space + "E"));
                    }
                    else
                    {
                        panel.add(new JLabel(space + "D"));
                    }
                    break;
                case 5:
                    if (c == Color.WHITE)
                    {
                        panel.add(new JLabel(space + "F"));
                    }
                    else
                    {
                        panel.add(new JLabel(space + "C"));
                    }
                    break;
                case 6:
                    if (c == Color.WHITE)
                    {
                        panel.add(new JLabel(space + "G"));
                    }
                    else
                    {
                        panel.add(new JLabel(space + "B"));
                    }
                    break;
                case 7:
                    if (c == Color.WHITE)
                    {
                        panel.add(new JLabel(space + "H"));
                    }
                    else
                    {
                        panel.add(new JLabel(space + "A"));
                    }
                    break;
            }
        }

        return panel;
    }

    /**
     * Create the table to display the moves made thus far.
     *
     * @return A JScrollPane containing the moves.
     */
    private JScrollPane createMoveTable()
    {
        Board b = m_game.getBoard();

        TableModel model = new TableModel(b.getWhiteMoves(), b.getBlackMoves());
        m_table = new JTable(model);
        m_table.setBackground(Constants.BACKGROUND);
        m_table.getTableHeader().setBackground(Constants.BACKGROUND);

        TableColumnModel columnModel = new DefaultTableColumnModel();
        String[] headers = new String[] {"", "White", "Black"};
        int index = 0;
        for (String h : headers)
        {
            TableColumn col = new TableColumn(index++);
            col.setHeaderValue(h);
            columnModel.addColumn(col);
        }

        m_table.setColumnModel(columnModel);
        m_table.getColumnModel().getColumn(0).setPreferredWidth(5);
        m_table.setEnabled(false);

        JScrollPane scroll = new JScrollPane(m_table);
        scroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        scroll.getVerticalScrollBar().addAdjustmentListener(new AdjustmentListener() {
            @Override
            public void adjustmentValueChanged(AdjustmentEvent e)
            {
                Adjustable adj = e.getAdjustable();
                adj.setValue(adj.getMaximum());
            }
        });

        return scroll;
    }

    /**
     * Create the panel to display game information.
     *
     * @return The panel holding the status of the information.
     */
    private JPanel createInfoPanel()
    {
        JPanel panel = new JPanel(new TableLayout("cols=1"));
        JPanel statusPanel = new JPanel(new TableLayout("cols=1"));
        JPanel scorePanel = new JPanel(new TableLayout("cols=20"));
        panel.setBackground(Constants.BACKGROUND);
        statusPanel.setBackground(Constants.BACKGROUND);
        scorePanel.setBackground(Constants.BACKGROUND);

        m_status = new JLabel(" ");

        m_whitePawnsCaptured = new JLabel("0");
        m_whiteKnightsCaptured = new JLabel("0");
        m_whiteBishopsCaptured = new JLabel("0");
        m_whiteRooksCaptured = new JLabel("0");
        m_whiteQueensCaptured = new JLabel("0");

        m_blackPawnsCaptured = new JLabel("0");
        m_blackKnightsCaptured = new JLabel("0");
        m_blackBishopsCaptured = new JLabel("0");
        m_blackRooksCaptured = new JLabel("0");
        m_blackQueensCaptured = new JLabel("0");

        statusPanel.add(m_status, "c");

        scorePanel.add(
            new ImagePanel(Constants.WPAWN_TRANSPARENT.getDescription(), Constants.TINY_PIECE_SIZE),
            "s skip=1");
        scorePanel.add(
            new ImagePanel(
                Constants.WKNIGHT_TRANSPARENT.getDescription(),
                Constants.TINY_PIECE_SIZE),
            "s");
        scorePanel.add(
            new ImagePanel(
                Constants.WBISHOP_TRANSPARENT.getDescription(),
                Constants.TINY_PIECE_SIZE),
            "s");
        scorePanel.add(
            new ImagePanel(Constants.WROOK_TRANSPARENT.getDescription(), Constants.TINY_PIECE_SIZE),
            "s");
        scorePanel.add(
            new ImagePanel(
                Constants.WQUEEN_TRANSPARENT.getDescription(),
                Constants.TINY_PIECE_SIZE),
            "s");

        scorePanel.add(
            new ImagePanel(
                Constants.BQUEEN_TRANSPARENT.getDescription(),
                Constants.TINY_PIECE_SIZE),
            "s skip=9");
        scorePanel.add(
            new ImagePanel(Constants.BROOK_TRANSPARENT.getDescription(), Constants.TINY_PIECE_SIZE),
            "s");
        scorePanel.add(
            new ImagePanel(
                Constants.BBISHOP_TRANSPARENT.getDescription(),
                Constants.TINY_PIECE_SIZE),
            "s");
        scorePanel.add(
            new ImagePanel(
                Constants.BKNIGHT_TRANSPARENT.getDescription(),
                Constants.TINY_PIECE_SIZE),
            "s");
        scorePanel.add(
            new ImagePanel(Constants.BPAWN_TRANSPARENT.getDescription(), Constants.TINY_PIECE_SIZE),
            "s");

        scorePanel.add(m_whitePawnsCaptured, "n skip=1");
        scorePanel.add(m_whiteKnightsCaptured, "n");
        scorePanel.add(m_whiteBishopsCaptured, "n");
        scorePanel.add(m_whiteRooksCaptured, "n");
        scorePanel.add(m_whiteQueensCaptured, "n");

        scorePanel.add(m_blackQueensCaptured, "n skip=9");
        scorePanel.add(m_blackRooksCaptured, "n");
        scorePanel.add(m_blackBishopsCaptured, "n");
        scorePanel.add(m_blackKnightsCaptured, "n");
        scorePanel.add(m_blackPawnsCaptured, "n");

        panel.add(statusPanel);
        panel.add(scorePanel);

        return panel;
    }

    /**
     * Performs an action depending on the event that took place.
     *
     * @param e The action being performed.
     */
    @Override
    public void actionPerformed(ActionEvent e)
    {
        String command = e.getActionCommand();

        // New game
        if (command.equals(Constants.NEW_GAME))
        {
            m_frame.dispose();
            m_game.disconnectFromServer();
            new InitializeGameGUI().createAndShowGUI();
        }

        // Generate PGN menu
        else if (command.equals(Constants.PGN))
        {
            JFileChooser fc = new JFileChooser();
            fc.setFileFilter(new PgnFileFilter());
            fc.setAcceptAllFileFilterUsed(false);

            Calendar cal = Calendar.getInstance();
            SimpleDateFormat sdf = new SimpleDateFormat("MM-dd-yyyy_hh-mm-ss");
            fc.setSelectedFile(new File(sdf.format(cal.getTime()) + ".pgn"));

            if (fc.showSaveDialog(m_frame) == JFileChooser.APPROVE_OPTION)
            {
                File file = fc.getSelectedFile();
                String fileName = file.getName();

                // Check if file exists, if so, confirm overwrite
                if (file.exists())
                {
                    int opt = JOptionPane.showConfirmDialog(
                        m_frame,
                        fileName + " already exists.\nDo you want to replace it?",
                        "Confirm",
                        JOptionPane.YES_NO_OPTION);
                    if (opt != JOptionPane.YES_OPTION)
                    {
                        return;
                    }
                }

                // Validate file name
                if (!file.getParentFile().isDirectory() || fileName.contains("/") ||
                    fileName.contains(":") || fileName.contains("*") || fileName.contains("?") ||
                    fileName.contains("\"") || fileName.contains("<") || fileName.contains(">") ||
                    fileName.contains("|"))
                {
                    String msg = "A file name can't contain any of the following "
                        + "characters:\n\\ / : * ? \" < > |";
                    JOptionPane.showMessageDialog(m_frame, msg, "Error", JOptionPane.ERROR_MESSAGE);
                    return;
                }

                // Check file extension, append if neccesary
                int i = fileName.lastIndexOf('.');
                String ext = "";
                if ((i > 0) && (i < fileName.length() - 1))
                {
                    ext = fileName.substring(i + 1).toLowerCase();
                }
                if (!ext.equalsIgnoreCase("pgn"))
                {
                    fileName += ".pgn";
                }

                // Generate PGN
                new PgnGenerator(
                    m_game.getPlayerColor(),
                    m_game.isEngineOpponent(),
                    new File(file.getParentFile(), fileName).getAbsolutePath(),
                    m_game.getBoard().getWhiteMoves(),
                    m_game.getBoard().getBlackMoves())
                    .generate();
            }
        }

        // Quit menu
        else if (command.equals(Constants.QUIT))
        {
            m_frame.dispose();
            m_game.disconnectFromServer();
            System.exit(0);
        }

        // Info menu
        else if (command.equals(Constants.INFO))
        {
            JOptionPane.showMessageDialog(
                null,
                "To create a new game, go to File > New Game or press CTRL+0\n\n"
                    + "To save a PGN file of the game, go to File > Save PGN or press CTRL+1\n\n"
                    +
                    "To move a piece, first click that piece, then click where you want to move that piece.\n"
                    +
                    "If it is a legal move, the board will update. Otherwise, nothing will change.",
                "How To Use",
                JOptionPane.INFORMATION_MESSAGE);
        }

        // About menu
        else if (command.equals(Constants.ABOUT))
        {
            JOptionPane.showMessageDialog(
                null,
                "Created by Timothy Flynn\n"
                    + "trflynn89@pm.me\n"
                    + "Version " + Constants.VERSION + "\n",
                "About ChessMate",
                JOptionPane.INFORMATION_MESSAGE,
                Constants.BROOK_TRANSPARENT);
        }
    }

    /**
     * Performs an action depending on the mouse press event that took place.
     *
     * @param e The action being performed.
     */
    @Override
    public void mousePressed(MouseEvent me)
    {
        Board b = m_game.getBoard();
        SquareGUI.resetAllSquares(b, false);
        SquareGUI prevSelected = m_selected;

        m_selected = (SquareGUI)me.getComponent();

        if (m_acceptMoves && prevSelected != null && prevSelected.getPiece() != null)
        {
            if (m_selected == prevSelected)
            {
                m_selected.resetSquare();
            }
            else
            {
                Move m = new Move(
                    prevSelected.getRank(),
                    prevSelected.getFile(),
                    m_selected.getRank(),
                    m_selected.getFile());

                b.setMoveStats(m);
                m_game.sendMessage(new Message(MessageType.MAKE_MOVE, m.getPgnString(false)));
            }

            m_selected = null;
        }
        else if (m_selected.getPiece() != null)
        {
            m_selected.selectSquare();
        }
        else
        {
            m_hovered.hoverSquare();
            m_selected = null;
        }
    }

    /**
     * Performs an action depending on the mouse entered event that took place.
     *
     * @param e The action being performed.
     */
    @Override
    public void mouseEntered(MouseEvent me)
    {
        m_hovered = (SquareGUI)me.getComponent();
        m_hovered.hoverSquare();
    }

    /**
     * Performs an action depending on the mouse exited event that took place.
     *
     * @param e The action being performed.
     */
    @Override
    public void mouseExited(MouseEvent me)
    {
        if (m_hovered != null)
        {
            m_hovered.resetSquare();
        }
        if (m_selected != null)
        {
            m_selected.selectSquare();
        }
    }

    public void mouseReleased(MouseEvent me)
    {
    }
    public void mouseClicked(MouseEvent me)
    {
    }
}
