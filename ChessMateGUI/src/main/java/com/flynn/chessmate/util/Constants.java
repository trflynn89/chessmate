package com.flynn.chessmate.util;

import javax.swing.ImageIcon;
import javax.swing.KeyStroke;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.io.FileNotFoundException;
import java.net.URL;

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
    public static final ImageIcon WPAWN_WTILE;
    public static final ImageIcon WPAWN_BTILE;
    public static final ImageIcon WPAWN_HTILE;
    public static final ImageIcon WPAWN_STILE;
    public static final ImageIcon WPAWN_MTILE;
    public static final ImageIcon WPAWN_TRANSPARENT;
    public static final ImageIcon WKNIGHT_WTILE;
    public static final ImageIcon WKNIGHT_BTILE;
    public static final ImageIcon WKNIGHT_HTILE;
    public static final ImageIcon WKNIGHT_STILE;
    public static final ImageIcon WKNIGHT_MTILE;
    public static final ImageIcon WKNIGHT_TRANSPARENT;
    public static final ImageIcon WBISHOP_WTILE;
    public static final ImageIcon WBISHOP_BTILE;
    public static final ImageIcon WBISHOP_HTILE;
    public static final ImageIcon WBISHOP_STILE;
    public static final ImageIcon WBISHOP_MTILE;
    public static final ImageIcon WBISHOP_TRANSPARENT;
    public static final ImageIcon WROOK_WTILE;
    public static final ImageIcon WROOK_BTILE;
    public static final ImageIcon WROOK_HTILE;
    public static final ImageIcon WROOK_STILE;
    public static final ImageIcon WROOK_MTILE;
    public static final ImageIcon WROOK_TRANSPARENT;
    public static final ImageIcon WQUEEN_WTILE;
    public static final ImageIcon WQUEEN_BTILE;
    public static final ImageIcon WQUEEN_HTILE;
    public static final ImageIcon WQUEEN_STILE;
    public static final ImageIcon WQUEEN_MTILE;
    public static final ImageIcon WQUEEN_TRANSPARENT;
    public static final ImageIcon WKING_WTILE;
    public static final ImageIcon WKING_BTILE;
    public static final ImageIcon WKING_HTILE;
    public static final ImageIcon WKING_STILE;
    public static final ImageIcon WKING_MTILE;
    public static final ImageIcon WKING_CTILE;
    public static final ImageIcon WKING_TRANSPARENT;

    // Black piece images
    public static final ImageIcon BPAWN_WTILE;
    public static final ImageIcon BPAWN_BTILE;
    public static final ImageIcon BPAWN_HTILE;
    public static final ImageIcon BPAWN_STILE;
    public static final ImageIcon BPAWN_MTILE;
    public static final ImageIcon BPAWN_TRANSPARENT;
    public static final ImageIcon BKNIGHT_WTILE;
    public static final ImageIcon BKNIGHT_BTILE;
    public static final ImageIcon BKNIGHT_HTILE;
    public static final ImageIcon BKNIGHT_STILE;
    public static final ImageIcon BKNIGHT_MTILE;
    public static final ImageIcon BKNIGHT_TRANSPARENT;
    public static final ImageIcon BBISHOP_WTILE;
    public static final ImageIcon BBISHOP_BTILE;
    public static final ImageIcon BBISHOP_HTILE;
    public static final ImageIcon BBISHOP_STILE;
    public static final ImageIcon BBISHOP_MTILE;
    public static final ImageIcon BBISHOP_TRANSPARENT;
    public static final ImageIcon BROOK_WTILE;
    public static final ImageIcon BROOK_BTILE;
    public static final ImageIcon BROOK_HTILE;
    public static final ImageIcon BROOK_STILE;
    public static final ImageIcon BROOK_MTILE;
    public static final ImageIcon BROOK_TRANSPARENT;
    public static final ImageIcon BQUEEN_WTILE;
    public static final ImageIcon BQUEEN_BTILE;
    public static final ImageIcon BQUEEN_HTILE;
    public static final ImageIcon BQUEEN_STILE;
    public static final ImageIcon BQUEEN_MTILE;
    public static final ImageIcon BQUEEN_TRANSPARENT;
    public static final ImageIcon BKING_WTILE;
    public static final ImageIcon BKING_BTILE;
    public static final ImageIcon BKING_HTILE;
    public static final ImageIcon BKING_STILE;
    public static final ImageIcon BKING_MTILE;
    public static final ImageIcon BKING_CTILE;
    public static final ImageIcon BKING_TRANSPARENT;

    // Mixed kings image
    public static final ImageIcon MIXED_KINGS;

    // Empty piece images
    public static final ImageIcon EMPTY_WTILE;
    public static final ImageIcon EMPTY_BTILE;
    public static final ImageIcon EMPTY_HTILE;
    public static final ImageIcon EMPTY_STILE;
    public static final ImageIcon EMPTY_MTILE;

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

    static
    {
        try
        {
            // White piece images
            WPAWN_WTILE = LoadImage("wpawn/wtile.png");
            WPAWN_BTILE = LoadImage("wpawn/btile.png");
            WPAWN_HTILE = LoadImage("wpawn/htile.png");
            WPAWN_STILE = LoadImage("wpawn/stile.png");
            WPAWN_MTILE = LoadImage("wpawn/mtile.png");
            WPAWN_TRANSPARENT = LoadImage("wpawn/transparent.png");
            WKNIGHT_WTILE = LoadImage("wknight/wtile.png");
            WKNIGHT_BTILE = LoadImage("wknight/btile.png");
            WKNIGHT_HTILE = LoadImage("wknight/htile.png");
            WKNIGHT_STILE = LoadImage("wknight/stile.png");
            WKNIGHT_MTILE = LoadImage("wknight/mtile.png");
            WKNIGHT_TRANSPARENT = LoadImage("wknight/transparent.png");
            WBISHOP_WTILE = LoadImage("wbishop/wtile.png");
            WBISHOP_BTILE = LoadImage("wbishop/btile.png");
            WBISHOP_HTILE = LoadImage("wbishop/htile.png");
            WBISHOP_STILE = LoadImage("wbishop/stile.png");
            WBISHOP_MTILE = LoadImage("wbishop/mtile.png");
            WBISHOP_TRANSPARENT = LoadImage("wbishop/transparent.png");
            WROOK_WTILE = LoadImage("wrook/wtile.png");
            WROOK_BTILE = LoadImage("wrook/btile.png");
            WROOK_HTILE = LoadImage("wrook/htile.png");
            WROOK_STILE = LoadImage("wrook/stile.png");
            WROOK_MTILE = LoadImage("wrook/mtile.png");
            WROOK_TRANSPARENT = LoadImage("wrook/transparent.png");
            WQUEEN_WTILE = LoadImage("wqueen/wtile.png");
            WQUEEN_BTILE = LoadImage("wqueen/btile.png");
            WQUEEN_HTILE = LoadImage("wqueen/htile.png");
            WQUEEN_STILE = LoadImage("wqueen/stile.png");
            WQUEEN_MTILE = LoadImage("wqueen/mtile.png");
            WQUEEN_TRANSPARENT = LoadImage("wqueen/transparent.png");
            WKING_WTILE = LoadImage("wking/wtile.png");
            WKING_BTILE = LoadImage("wking/btile.png");
            WKING_HTILE = LoadImage("wking/htile.png");
            WKING_STILE = LoadImage("wking/stile.png");
            WKING_MTILE = LoadImage("wking/mtile.png");
            WKING_CTILE = LoadImage("wking/ctile.png");
            WKING_TRANSPARENT = LoadImage("wking/transparent.png");

            // Black piece images
            BPAWN_WTILE = LoadImage("bpawn/wtile.png");
            BPAWN_BTILE = LoadImage("bpawn/btile.png");
            BPAWN_HTILE = LoadImage("bpawn/htile.png");
            BPAWN_STILE = LoadImage("bpawn/stile.png");
            BPAWN_MTILE = LoadImage("bpawn/mtile.png");
            BPAWN_TRANSPARENT = LoadImage("bpawn/transparent.png");
            BKNIGHT_WTILE = LoadImage("bknight/wtile.png");
            BKNIGHT_BTILE = LoadImage("bknight/btile.png");
            BKNIGHT_HTILE = LoadImage("bknight/htile.png");
            BKNIGHT_STILE = LoadImage("bknight/stile.png");
            BKNIGHT_MTILE = LoadImage("bknight/mtile.png");
            BKNIGHT_TRANSPARENT = LoadImage("bknight/transparent.png");
            BBISHOP_WTILE = LoadImage("bbishop/wtile.png");
            BBISHOP_BTILE = LoadImage("bbishop/btile.png");
            BBISHOP_HTILE = LoadImage("bbishop/htile.png");
            BBISHOP_STILE = LoadImage("bbishop/stile.png");
            BBISHOP_MTILE = LoadImage("bbishop/mtile.png");
            BBISHOP_TRANSPARENT = LoadImage("bbishop/transparent.png");
            BROOK_WTILE = LoadImage("brook/wtile.png");
            BROOK_BTILE = LoadImage("brook/btile.png");
            BROOK_HTILE = LoadImage("brook/htile.png");
            BROOK_STILE = LoadImage("brook/stile.png");
            BROOK_MTILE = LoadImage("brook/mtile.png");
            BROOK_TRANSPARENT = LoadImage("brook/transparent.png");
            BQUEEN_WTILE = LoadImage("bqueen/wtile.png");
            BQUEEN_BTILE = LoadImage("bqueen/btile.png");
            BQUEEN_HTILE = LoadImage("bqueen/htile.png");
            BQUEEN_STILE = LoadImage("bqueen/stile.png");
            BQUEEN_MTILE = LoadImage("bqueen/mtile.png");
            BQUEEN_TRANSPARENT = LoadImage("bqueen/transparent.png");
            BKING_WTILE = LoadImage("bking/wtile.png");
            BKING_BTILE = LoadImage("bking/btile.png");
            BKING_HTILE = LoadImage("bking/htile.png");
            BKING_STILE = LoadImage("bking/stile.png");
            BKING_MTILE = LoadImage("bking/mtile.png");
            BKING_CTILE = LoadImage("bking/ctile.png");
            BKING_TRANSPARENT = LoadImage("bking/transparent.png");

            // Mixed kings image
            MIXED_KINGS = LoadImage("mixedkings.png");

            // Empty piece images
            EMPTY_WTILE = LoadImage("empty/wtile.png");
            EMPTY_BTILE = LoadImage("empty/btile.png");
            EMPTY_HTILE = LoadImage("empty/htile.png");
            EMPTY_STILE = LoadImage("empty/stile.png");
            EMPTY_MTILE = LoadImage("empty/mtile.png");
        }
        catch (FileNotFoundException ex)
        {
            throw new ExceptionInInitializerError(ex);
        }
    }

    /**
     * Safely attempt to load a static image resource.
     */
    private static ImageIcon LoadImage(String resource) throws FileNotFoundException
    {
        final URL resourceUrl = Constants.class.getResource("/images/" + resource);

        if (resourceUrl != null)
        {
            return new ImageIcon(resourceUrl);
        }

        throw new FileNotFoundException(resource);
    }
}
