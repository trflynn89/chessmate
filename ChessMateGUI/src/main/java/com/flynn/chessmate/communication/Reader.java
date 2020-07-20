package com.flynn.chessmate.communication;

import javax.swing.JOptionPane;

import com.flynn.chessmate.game.ChessGame;
import com.flynn.chessmate.gui.BoardGUI;

/**
 * Thread for reading a Message from the server.
 *
 * Run in its own thread so that it can run constantly while the rest of the
 * application continues to run.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
public class Reader extends Thread {

	protected ChessGame m_game;

	/**
	 * Creates the Thread and stores the communicator.
	 */
	public Reader(ChessGame game)
	{
		super("Reader");
		this.m_game = game;
	}

	/**
	 * Runs the Reader. Constantly checks for a Message from the server, and
	 * sends it to the Processor.
	 */
	public void run()
	{
		boolean keepRunning = true;

		while (keepRunning)
		{
			Message m = m_game.readMessage();
			keepRunning = Processor.process(m_game, m);
		}

		if (m_game.isShutdownExpected())
		{
			m_game.disconnectFromServer();
		}
		else
		{
			JOptionPane.showMessageDialog
			(
				null,
				"Sorry! Either the server shut down or something else went " +
				"wrong.\nPlease try playing again later!\n\n" +
				"If the problem persists, contact me at " +
				"timothy.flynn@outlook.com",
				"Error", JOptionPane.ERROR_MESSAGE
			);
		}

		BoardGUI.acceptMoves(false);
	}
}
