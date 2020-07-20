package com.flynn.chessmate.gui;

import com.flynn.chessmate.game.ChessGame;
import com.flynn.chessmate.util.Constants;

import org.freixas.tablelayout.TableLayout;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JToggleButton;
import javax.swing.WindowConstants;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Random;

/**
 * GUI for starting a game. Player color defaults to white, and difficulty
 * defaults to medium.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version August 10, 2011
 */
public class InitializeGameGUI implements ActionListener
{
    private static JFrame m_frame;

    private Color m_playerColor;
    private int m_difficulty;
    private boolean m_engineOpponent;

    private JToggleButton m_whiteButton;
    private JToggleButton m_blackButton;
    private JToggleButton m_randomButton;

    private JRadioButton m_easyButton;
    private JRadioButton m_mediumButton;
    private JRadioButton m_hardButton;

    private JRadioButton m_computerButton;
    private JRadioButton m_humanButton;

    /**
     * Constructor.
     */
    public InitializeGameGUI()
    {
        m_playerColor = getRandomColor();
        m_difficulty = 0;
        m_engineOpponent = true;
    }

    /**
     * Initialize all components of the GUI and show it.
     */
    public void createAndShowGUI()
    {
        TableLayout layout =
            new TableLayout("cols=1 rgap=10 tileft=10 tiright=10 titop=10 tibottom=10");
        JPanel panel = new JPanel(layout);
        panel.setBackground(Constants.BACKGROUND);
        panel.add(createColorSelector());
        panel.add(createDifficultySelector());
        panel.add(createOpponentSelector());
        panel.add(createButtonPanel());

        m_frame = new JFrame("ChessMate");
        m_frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        m_frame.setIconImage(Constants.BROOK_TRANSPARENT.getImage());
        m_frame.setBackground(Constants.BACKGROUND);
        m_frame.add(panel);
        m_frame.pack();
        m_frame.setVisible(true);
        m_frame.setLocationRelativeTo(null);
        m_frame.setResizable(false);
    }

    /**
     * Create the panel to hold the color selector.
     *
     * @return The JPanel holding the color selector.
     */
    private JPanel createColorSelector()
    {
        TableLayout layout = new TableLayout("cols=5");
        JPanel panel = new JPanel(layout);
        panel.setBackground(Constants.BACKGROUND);

        JLabel label = new JLabel("Select a color (default is random):");
        panel.add(label, "cspan=5");

        m_whiteButton = new JToggleButton();
        m_whiteButton.setIcon(Constants.WKING_TRANSPARENT);
        m_whiteButton.setBorder(BorderFactory.createRaisedBevelBorder());
        m_whiteButton.setActionCommand(Constants.WHITE);
        m_whiteButton.addActionListener(this);

        m_randomButton = new JToggleButton();
        m_randomButton.setIcon(Constants.MIXED_KINGS);
        m_randomButton.setBorder(BorderFactory.createRaisedBevelBorder());
        m_randomButton.setActionCommand(Constants.RANDOM);
        m_randomButton.addActionListener(this);
        m_randomButton.setSelected(true);

        m_blackButton = new JToggleButton();
        m_blackButton.setIcon(Constants.BKING_TRANSPARENT);
        m_blackButton.setBorder(BorderFactory.createRaisedBevelBorder());
        m_blackButton.setActionCommand(Constants.BLACK);
        m_blackButton.addActionListener(this);

        panel.add(m_whiteButton);
        panel.add(m_randomButton, "skip=1");
        panel.add(m_blackButton, "skip=1");

        return panel;
    }

    /**
     * Create the panel to hold the difficulty selector.
     *
     * @return The JPanel holding the difficulty selector.
     */
    private JPanel createDifficultySelector()
    {
        TableLayout layout = new TableLayout("cols=1");
        JPanel panel = new JPanel(layout);
        panel.setBackground(Constants.BACKGROUND);

        JLabel label = new JLabel("Select a difficulty (default is easy):");
        panel.add(label);

        m_easyButton = new JRadioButton("Easy");
        m_easyButton.setBackground(Constants.BACKGROUND);
        m_easyButton.setActionCommand(Constants.EASY);
        m_easyButton.addActionListener(this);
        m_easyButton.setSelected(true);

        m_mediumButton = new JRadioButton("Medium");
        m_mediumButton.setBackground(Constants.BACKGROUND);
        m_mediumButton.setActionCommand(Constants.MEDIUM);
        m_mediumButton.addActionListener(this);

        m_hardButton = new JRadioButton("Hard");
        m_hardButton.setBackground(Constants.BACKGROUND);
        m_hardButton.setActionCommand(Constants.HARD);
        m_hardButton.addActionListener(this);

        panel.add(m_easyButton);
        panel.add(m_mediumButton);
        panel.add(m_hardButton);

        return panel;
    }

    /**
     * Create the panel to hold the opponent selector.
     *
     * @return The JPanel holding the opponent selector.
     */
    private JPanel createOpponentSelector()
    {
        TableLayout layout = new TableLayout("cols=1");
        JPanel panel = new JPanel(layout);
        panel.setBackground(Constants.BACKGROUND);

        JLabel label = new JLabel("Select an opponent (default is computer):");
        panel.add(label);

        m_computerButton = new JRadioButton("Computer");
        m_computerButton.setBackground(Constants.BACKGROUND);
        m_computerButton.setActionCommand(Constants.COMPUTER);
        m_computerButton.addActionListener(this);
        m_computerButton.setSelected(true);

        m_humanButton = new JRadioButton("Human");
        m_humanButton.setBackground(Constants.BACKGROUND);
        m_humanButton.setActionCommand(Constants.HUMAN);
        m_humanButton.addActionListener(this);

        panel.add(m_computerButton);
        panel.add(m_humanButton);

        return panel;
    }

    /**
     * Create the panel to hold the control buttons used for this GUI.
     *
     * @return The JPanel holding the control buttons.
     */
    private JPanel createButtonPanel()
    {
        TableLayout layout = new TableLayout("cols=3");
        JPanel panel = new JPanel(layout);
        panel.setBackground(Constants.BACKGROUND);

        JButton start = new JButton("Start Game");
        start.setBorder(BorderFactory.createRaisedBevelBorder());
        start.setActionCommand(Constants.START);
        start.addActionListener(this);

        JButton cancel = new JButton("Cancel");
        cancel.setBorder(BorderFactory.createRaisedBevelBorder());
        cancel.setActionCommand(Constants.CANCEL);
        cancel.addActionListener(this);

        panel.add(start);
        panel.add(cancel, "skip=1");

        return panel;
    }

    /**
     * @return A random color, either Color.WHITE or Color.BLACK.
     */
    private Color getRandomColor()
    {
        Random random = new Random();
        random.setSeed(System.nanoTime());

        return (random.nextInt(2) == 0 ? Color.WHITE : Color.BLACK);
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

        // White button
        if (command.equals(Constants.WHITE))
        {
            m_playerColor = Color.WHITE;
            m_whiteButton.setSelected(true);
            m_randomButton.setSelected(false);
            m_blackButton.setSelected(false);
        }

        // Random button
        else if (command.equals(Constants.RANDOM))
        {
            m_playerColor = getRandomColor();
            m_whiteButton.setSelected(false);
            m_randomButton.setSelected(true);
            m_blackButton.setSelected(false);
        }

        // Black button
        else if (command.equals(Constants.BLACK))
        {
            m_playerColor = Color.BLACK;
            m_whiteButton.setSelected(false);
            m_randomButton.setSelected(false);
            m_blackButton.setSelected(true);
        }

        // Easy button
        else if (command.equals(Constants.EASY))
        {
            m_difficulty = 0;
            m_easyButton.setSelected(true);
            m_mediumButton.setSelected(false);
            m_hardButton.setSelected(false);
        }

        // Medium button
        else if (command.equals(Constants.MEDIUM))
        {
            m_difficulty = 1;
            m_easyButton.setSelected(false);
            m_mediumButton.setSelected(true);
            m_hardButton.setSelected(false);
        }

        // Hard button
        else if (command.equals(Constants.HARD))
        {
            m_difficulty = 2;
            m_easyButton.setSelected(false);
            m_mediumButton.setSelected(false);
            m_hardButton.setSelected(true);
        }

        // Computer button
        else if (command.equals(Constants.COMPUTER))
        {
            m_engineOpponent = true;
            m_computerButton.setSelected(true);
            m_humanButton.setSelected(false);

            // Reset to previous layout
            if (m_whiteButton.isSelected())
            {
                m_playerColor = Color.WHITE;
            }
            else if (m_blackButton.isSelected())
            {
                m_playerColor = Color.BLACK;
            }
            else
            {
                m_playerColor = getRandomColor();
            }

            // Enable other options
            m_whiteButton.setEnabled(true);
            m_randomButton.setEnabled(true);
            m_blackButton.setEnabled(true);
            m_easyButton.setEnabled(true);
            m_mediumButton.setEnabled(true);
            m_hardButton.setEnabled(true);
        }

        // Human button
        else if (command.equals(Constants.HUMAN))
        {
            m_engineOpponent = false;
            m_computerButton.setSelected(false);
            m_humanButton.setSelected(true);

            // Reset to default layout
            m_playerColor = Color.WHITE;

            // Diable other options
            m_whiteButton.setEnabled(false);
            m_randomButton.setEnabled(false);
            m_blackButton.setEnabled(false);
            m_easyButton.setEnabled(false);
            m_mediumButton.setEnabled(false);
            m_hardButton.setEnabled(false);
        }

        // Start button
        else if (command.equals(Constants.START))
        {
            ChessGame game = new ChessGame(m_playerColor, m_difficulty, m_engineOpponent);

            game.initializeGame();
            new BoardGUI(game).createAndShowGUI();
            m_frame.dispose();

            game.requestMove();
        }

        // Cancel button
        else if (command.equals(Constants.CANCEL))
        {
            m_frame.dispose();
        }
    }
}
