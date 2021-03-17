package com.flynn.chessmate.game;

import com.flynn.chessmate.communication.Message;
import com.flynn.chessmate.communication.Message.MessageType;
import com.flynn.chessmate.communication.Reader;
import com.flynn.chessmate.gui.BoardGUI;
import com.flynn.chessmate.util.Constants;

import javax.swing.JOptionPane;

import java.awt.Color;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ConnectException;
import java.net.Socket;
import java.net.UnknownHostException;

/**
 * Class to represent a single chess game and to maintain its board and
 * communication to the engine.
 *
 * @author Timothy Flynn (trflynn89@pm.me)
 * @version March 3, 2013
 */
public class ChessGame extends Thread
{
    private int m_sessionId;
    private Board m_board;

    private Socket m_socket;
    private BufferedReader m_input;
    private PrintWriter m_output;

    private boolean m_enabled;
    private boolean m_shutdownExpected;

    private Color m_playerColor;
    private int m_difficulty;
    private boolean m_engineOpponent;

    /**
     * Constructor to set the game's options and initiate communication with
     * the engine.
     */
    public ChessGame(Color playerColor, int difficulty, boolean engineOpponent)
    {
        super("ChessGame");

        m_enabled = false;
        m_shutdownExpected = false;

        m_playerColor = playerColor;
        m_difficulty = difficulty;
        m_engineOpponent = engineOpponent;

        // Try to connect to each configured host
        for (String host : Constants.HOSTS)
        {
            try
            {
                m_socket = new Socket(host, Constants.PORT);

                m_output = new PrintWriter(m_socket.getOutputStream(), false);
                m_input = new BufferedReader(new InputStreamReader(m_socket.getInputStream()));

                new Reader(this).start();

                m_enabled = true;
                break;
            }
            catch (UnknownHostException e)
            { // Ignore, just log
                System.err.println("Don't know about host: " + host);
            }
            catch (ConnectException e)
            { // Ignore, just log
                System.err.println("Error with host: " + host);
            }
            catch (IOException e)
            { // Ignore, just log
                System.err.println("IOException with host: " + host);
            }
        }

        if (!m_enabled)
        {
            JOptionPane.showMessageDialog(
                null,
                "Could not connect to server.",
                "Error",
                JOptionPane.ERROR_MESSAGE);
            System.exit(-1);
        }
    }

    /**
     * Initialize the game and its board.
     */
    public void initializeGame()
    {
        m_board = new Board();

        int c = (m_playerColor == Color.WHITE ? 1 : 0);
        Message m = new Message(MessageType.START_GAME, c + " " + m_difficulty);

        this.sendMessage(m);
    }

    /**
     * Request a move from the engine.
     */
    public void requestMove()
    {
        if (m_engineOpponent && m_board.getPlayerInTurn() != m_playerColor)
        {
            BoardGUI.acceptMoves(false);
            BoardGUI.setWaitCursor(true);

            Message msg = new Message(MessageType.GET_MOVE);
            this.sendMessage(msg);

            BoardGUI.setStatus("Engine is thinking...");
        }
        else
        {
            BoardGUI.acceptMoves(true);
            BoardGUI.setWaitCursor(false);
            BoardGUI.setStatus("Your move!");
        }
    }

    /**
     * @return True if a disconnect from the engine was expected.
     */
    public boolean isShutdownExpected()
    {
        return m_shutdownExpected;
    }

    /**
     * Set the game's session ID.
     *
     * @param sessionId The game's session ID.
     */
    public void setSessionId(int sessionId)
    {
        m_sessionId = sessionId;
    }

    /**
     * @return The game's session ID.
     */
    public int getSessionId()
    {
        return m_sessionId;
    }

    /**
     * @return This game's board.
     */
    public Board getBoard()
    {
        return m_board;
    }

    /**
     * @return The color of the human player, or just white if game is PvP.
     */
    public Color getPlayerColor()
    {
        return m_playerColor;
    }

    /**
     * @return True if playing against the engine.
     */
    public boolean isEngineOpponent()
    {
        return m_engineOpponent;
    }

    /**
     * Gracefully disconnect from the engine.
     */
    public void disconnectFromServer()
    {
        if (m_enabled)
        {
            try
            {
                BoardGUI.acceptMoves(false);
                BoardGUI.setWaitCursor(false);

                Message m = new Message(MessageType.DISCONNECT);
                this.sendMessage(m);

                m_enabled = false;
                m_shutdownExpected = true;

                m_socket.close();
                m_input.close();
                m_output.close();
            }
            catch (IOException e)
            {
            }
        }
    }

    /**
     * @return A message read from the engine.
     */
    public Message readMessage()
    {
        StringBuilder recvBuffer = new StringBuilder();
        boolean finishedReading = false, goodMessage = false;

        while (m_enabled && !finishedReading)
        {
            try
            {
                int c = m_input.read();

                if (c == -1)
                {
                    finishedReading = true;
                }
                else if (Message.isEoM((char)c))
                {
                    finishedReading = true;
                    goodMessage = true;
                }
                else
                {
                    recvBuffer.append((char)c);
                }
            }
            catch (IOException e)
            {
                finishedReading = true;
            }
        }

        Message m = new Message();

        if (goodMessage)
        {
            m = new Message(recvBuffer.toString());
        }

        return m;
    }

    /**
     * Send a message to the engine.
     *
     * @param The message to send.
     */
    public void sendMessage(Message msg)
    {
        if (m_enabled)
        {
            m_output.print(msg.serialize());
            m_output.flush();
        }
    }
}
