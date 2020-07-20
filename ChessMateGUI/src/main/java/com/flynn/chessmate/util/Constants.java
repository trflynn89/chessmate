package com.flynn.chessmate.util;

import javax.swing.ImageIcon;
import javax.swing.KeyStroke;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

/**
 * Constant values used for this project.
 *
 * @author Timothy Flynn (trflynn89@pm.me)
 * @version March 3, 2013
 */
public class Constants
{
    // Version
    public static final String VERSION = "1.0";

    // Network info
    public static String[] HOSTS = {"127.0.0.1", "54.243.140.199"};
    public static int PORT = 12389;

    // Sizes
    public static final int NUM_RANKS = 8;
    public static final int NUM_FILES = 8;
    public static final int BOARD_SIZE = NUM_RANKS * NUM_FILES;
    public static int GET_RANK(int s)
    {
        return (s / NUM_RANKS);
    }
    public static int GET_FILE(int s)
    {
        return (s % NUM_FILES);
    }
    public static final int BOARD_HEIGHT = 900;
    public static final int BOARD_WIDTH = 1100;
    public static final int PIECE_SIZE = 85;
    public static final int TINY_PIECE_SIZE = PIECE_SIZE / 2;

    // Files
    public static final int FILE_A = 0;
    public static final int FILE_B = 1;
    public static final int FILE_C = 2;
    public static final int FILE_D = 3;
    public static final int FILE_E = 4;
    public static final int FILE_F = 5;
    public static final int FILE_G = 6;
    public static final int FILE_H = 7;

    // Ranks
    public static final int RANK_1 = 0;
    public static final int RANK_2 = 1;
    public static final int RANK_3 = 2;
    public static final int RANK_4 = 3;
    public static final int RANK_5 = 4;
    public static final int RANK_6 = 5;
    public static final int RANK_7 = 6;
    public static final int RANK_8 = 7;

    // White piece images
    public static final ImageIcon WPAWN_WTILE =
        new ImageIcon(Constants.class.getResource("/images/wpawn/wtile.png"));
    public static final ImageIcon WPAWN_BTILE =
        new ImageIcon(Constants.class.getResource("/images/wpawn/btile.png"));
    public static final ImageIcon WPAWN_HTILE =
        new ImageIcon(Constants.class.getResource("/images/wpawn/htile.png"));
    public static final ImageIcon WPAWN_STILE =
        new ImageIcon(Constants.class.getResource("/images/wpawn/stile.png"));
    public static final ImageIcon WPAWN_MTILE =
        new ImageIcon(Constants.class.getResource("/images/wpawn/mtile.png"));
    public static final ImageIcon WPAWN_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/wpawn/transparent.png"));
    public static final ImageIcon WKNIGHT_WTILE =
        new ImageIcon(Constants.class.getResource("/images/wknight/wtile.png"));
    public static final ImageIcon WKNIGHT_BTILE =
        new ImageIcon(Constants.class.getResource("/images/wknight/btile.png"));
    public static final ImageIcon WKNIGHT_HTILE =
        new ImageIcon(Constants.class.getResource("/images/wknight/htile.png"));
    public static final ImageIcon WKNIGHT_STILE =
        new ImageIcon(Constants.class.getResource("/images/wknight/stile.png"));
    public static final ImageIcon WKNIGHT_MTILE =
        new ImageIcon(Constants.class.getResource("/images/wknight/mtile.png"));
    public static final ImageIcon WKNIGHT_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/wknight/transparent.png"));
    public static final ImageIcon WBISHOP_WTILE =
        new ImageIcon(Constants.class.getResource("/images/wbishop/wtile.png"));
    public static final ImageIcon WBISHOP_BTILE =
        new ImageIcon(Constants.class.getResource("/images/wbishop/btile.png"));
    public static final ImageIcon WBISHOP_HTILE =
        new ImageIcon(Constants.class.getResource("/images/wbishop/htile.png"));
    public static final ImageIcon WBISHOP_STILE =
        new ImageIcon(Constants.class.getResource("/images/wbishop/stile.png"));
    public static final ImageIcon WBISHOP_MTILE =
        new ImageIcon(Constants.class.getResource("/images/wbishop/mtile.png"));
    public static final ImageIcon WBISHOP_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/wbishop/transparent.png"));
    public static final ImageIcon WROOK_WTILE =
        new ImageIcon(Constants.class.getResource("/images/wrook/wtile.png"));
    public static final ImageIcon WROOK_BTILE =
        new ImageIcon(Constants.class.getResource("/images/wrook/btile.png"));
    public static final ImageIcon WROOK_HTILE =
        new ImageIcon(Constants.class.getResource("/images/wrook/htile.png"));
    public static final ImageIcon WROOK_STILE =
        new ImageIcon(Constants.class.getResource("/images/wrook/stile.png"));
    public static final ImageIcon WROOK_MTILE =
        new ImageIcon(Constants.class.getResource("/images/wrook/mtile.png"));
    public static final ImageIcon WROOK_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/wrook/transparent.png"));
    public static final ImageIcon WQUEEN_WTILE =
        new ImageIcon(Constants.class.getResource("/images/wqueen/wtile.png"));
    public static final ImageIcon WQUEEN_BTILE =
        new ImageIcon(Constants.class.getResource("/images/wqueen/btile.png"));
    public static final ImageIcon WQUEEN_HTILE =
        new ImageIcon(Constants.class.getResource("/images/wqueen/htile.png"));
    public static final ImageIcon WQUEEN_STILE =
        new ImageIcon(Constants.class.getResource("/images/wqueen/stile.png"));
    public static final ImageIcon WQUEEN_MTILE =
        new ImageIcon(Constants.class.getResource("/images/wqueen/mtile.png"));
    public static final ImageIcon WQUEEN_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/wqueen/transparent.png"));
    public static final ImageIcon WKING_WTILE =
        new ImageIcon(Constants.class.getResource("/images/wking/wtile.png"));
    public static final ImageIcon WKING_BTILE =
        new ImageIcon(Constants.class.getResource("/images/wking/btile.png"));
    public static final ImageIcon WKING_HTILE =
        new ImageIcon(Constants.class.getResource("/images/wking/htile.png"));
    public static final ImageIcon WKING_STILE =
        new ImageIcon(Constants.class.getResource("/images/wking/stile.png"));
    public static final ImageIcon WKING_MTILE =
        new ImageIcon(Constants.class.getResource("/images/wking/mtile.png"));
    public static final ImageIcon WKING_CTILE =
        new ImageIcon(Constants.class.getResource("/images/wking/ctile.png"));
    public static final ImageIcon WKING_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/wking/transparent.png"));

    // Black piece images
    public static final ImageIcon BPAWN_WTILE =
        new ImageIcon(Constants.class.getResource("/images/bpawn/wtile.png"));
    public static final ImageIcon BPAWN_BTILE =
        new ImageIcon(Constants.class.getResource("/images/bpawn/btile.png"));
    public static final ImageIcon BPAWN_HTILE =
        new ImageIcon(Constants.class.getResource("/images/bpawn/htile.png"));
    public static final ImageIcon BPAWN_STILE =
        new ImageIcon(Constants.class.getResource("/images/bpawn/stile.png"));
    public static final ImageIcon BPAWN_MTILE =
        new ImageIcon(Constants.class.getResource("/images/bpawn/mtile.png"));
    public static final ImageIcon BPAWN_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/bpawn/transparent.png"));
    public static final ImageIcon BKNIGHT_WTILE =
        new ImageIcon(Constants.class.getResource("/images/bknight/wtile.png"));
    public static final ImageIcon BKNIGHT_BTILE =
        new ImageIcon(Constants.class.getResource("/images/bknight/btile.png"));
    public static final ImageIcon BKNIGHT_HTILE =
        new ImageIcon(Constants.class.getResource("/images/bknight/htile.png"));
    public static final ImageIcon BKNIGHT_STILE =
        new ImageIcon(Constants.class.getResource("/images/bknight/stile.png"));
    public static final ImageIcon BKNIGHT_MTILE =
        new ImageIcon(Constants.class.getResource("/images/bknight/mtile.png"));
    public static final ImageIcon BKNIGHT_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/bknight/transparent.png"));
    public static final ImageIcon BBISHOP_WTILE =
        new ImageIcon(Constants.class.getResource("/images/bbishop/wtile.png"));
    public static final ImageIcon BBISHOP_BTILE =
        new ImageIcon(Constants.class.getResource("/images/bbishop/btile.png"));
    public static final ImageIcon BBISHOP_HTILE =
        new ImageIcon(Constants.class.getResource("/images/bbishop/htile.png"));
    public static final ImageIcon BBISHOP_STILE =
        new ImageIcon(Constants.class.getResource("/images/bbishop/stile.png"));
    public static final ImageIcon BBISHOP_MTILE =
        new ImageIcon(Constants.class.getResource("/images/bbishop/mtile.png"));
    public static final ImageIcon BBISHOP_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/bbishop/transparent.png"));
    public static final ImageIcon BROOK_WTILE =
        new ImageIcon(Constants.class.getResource("/images/brook/wtile.png"));
    public static final ImageIcon BROOK_BTILE =
        new ImageIcon(Constants.class.getResource("/images/brook/btile.png"));
    public static final ImageIcon BROOK_HTILE =
        new ImageIcon(Constants.class.getResource("/images/brook/htile.png"));
    public static final ImageIcon BROOK_STILE =
        new ImageIcon(Constants.class.getResource("/images/brook/stile.png"));
    public static final ImageIcon BROOK_MTILE =
        new ImageIcon(Constants.class.getResource("/images/brook/mtile.png"));
    public static final ImageIcon BROOK_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/brook/transparent.png"));
    public static final ImageIcon BQUEEN_WTILE =
        new ImageIcon(Constants.class.getResource("/images/bqueen/wtile.png"));
    public static final ImageIcon BQUEEN_BTILE =
        new ImageIcon(Constants.class.getResource("/images/bqueen/btile.png"));
    public static final ImageIcon BQUEEN_HTILE =
        new ImageIcon(Constants.class.getResource("/images/bqueen/htile.png"));
    public static final ImageIcon BQUEEN_STILE =
        new ImageIcon(Constants.class.getResource("/images/bqueen/stile.png"));
    public static final ImageIcon BQUEEN_MTILE =
        new ImageIcon(Constants.class.getResource("/images/bqueen/mtile.png"));
    public static final ImageIcon BQUEEN_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/bqueen/transparent.png"));
    public static final ImageIcon BKING_WTILE =
        new ImageIcon(Constants.class.getResource("/images/bking/wtile.png"));
    public static final ImageIcon BKING_BTILE =
        new ImageIcon(Constants.class.getResource("/images/bking/btile.png"));
    public static final ImageIcon BKING_HTILE =
        new ImageIcon(Constants.class.getResource("/images/bking/htile.png"));
    public static final ImageIcon BKING_STILE =
        new ImageIcon(Constants.class.getResource("/images/bking/stile.png"));
    public static final ImageIcon BKING_MTILE =
        new ImageIcon(Constants.class.getResource("/images/bking/mtile.png"));
    public static final ImageIcon BKING_CTILE =
        new ImageIcon(Constants.class.getResource("/images/bking/ctile.png"));
    public static final ImageIcon BKING_TRANSPARENT =
        new ImageIcon(Constants.class.getResource("/images/bking/transparent.png"));

    // Mixed kings image
    public static final ImageIcon MIXED_KINGS =
        new ImageIcon(Constants.class.getResource("/images/mixedkings.png"));

    // Empty piece images
    public static final ImageIcon EMPTY_WTILE =
        new ImageIcon(Constants.class.getResource("/images/empty/wtile.png"));
    public static final ImageIcon EMPTY_BTILE =
        new ImageIcon(Constants.class.getResource("/images/empty/btile.png"));
    public static final ImageIcon EMPTY_HTILE =
        new ImageIcon(Constants.class.getResource("/images/empty/htile.png"));
    public static final ImageIcon EMPTY_STILE =
        new ImageIcon(Constants.class.getResource("/images/empty/stile.png"));
    public static final ImageIcon EMPTY_MTILE =
        new ImageIcon(Constants.class.getResource("/images/empty/mtile.png"));

    // Colors
    public static final Color BACKGROUND = new Color(214, 217, 223);

    // Commands
    public static final String ABOUT = "about";
    public static final String BLACK = "black";
    public static final String CANCEL = "cancel";
    public static final String COMPUTER = "computer";
    public static final String EASY = "easy";
    public static final String HARD = "hard";
    public static final String HUMAN = "human";
    public static final String INFO = "info";
    public static final String MEDIUM = "medium";
    public static final String NEW_GAME = "new_game";
    public static final String PGN = "pgn";
    public static final String QUIT = "quit";
    public static final String RANDOM = "random";
    public static final String START = "start";
    public static final String WHITE = "white";

    // Key strokes
    public final static KeyStroke CTRL0 =
        KeyStroke.getKeyStroke(KeyEvent.VK_0, ActionEvent.CTRL_MASK);
    public final static KeyStroke CTRL1 =
        KeyStroke.getKeyStroke(KeyEvent.VK_1, ActionEvent.CTRL_MASK);
    public final static KeyStroke ALT0 =
        KeyStroke.getKeyStroke(KeyEvent.VK_0, ActionEvent.ALT_MASK);
    public final static KeyStroke ESC = KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0);
    public final static KeyStroke F1 = KeyStroke.getKeyStroke(KeyEvent.VK_F1, 0);
}
