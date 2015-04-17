package cm.communication;

import java.awt.Color;

import cm.communication.Message.MessageType;
import cm.game.ChessGame;
import cm.gui.BoardGUI;
import cm.movement.Move;

/**
 * Static class to process a message received from the server.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
public class Processor
{
	/**
	 * Process the message and perform any appropriate action.
	 *
	 * @param game The game receiving this message.
	 * @param msg The message to process.
	 * @return True if the game should continue, false otherwise.
	 */
	public static boolean process
	(
		ChessGame game,
		Message msg
	)
	{
		BoardGUI.setStatus(" ");
		
		MessageType type = msg.getMessageType();
		String data = msg.getData();
		
		Move m;
		
		switch(type)
		{
		// START GAME
		// Parse the session ID received from the server
		case START_GAME:
			int id = Integer.parseInt(data);
			game.setSessionId(id);
			return true;
			
		// MAKE MOVE
		// Parse unambiguous PGN string from server and make it
		case MAKE_MOVE:
			String str[] = data.split(" ");
			Color c = game.getBoard().getPlayerInTurn();
			
			m = new Move(str[0], c);
			game.getBoard().makeMove(m);
			
			// Check or checkmate?
			if(m.isCheck())
			{
				if(c == Color.WHITE)
				{
					game.getBoard().setCheck(Color.BLACK);
					BoardGUI.setStatus("Black in check!");
				}
				else
				{
					game.getBoard().setCheck(Color.WHITE);
					BoardGUI.setStatus("White in check!");
				}
			}
			else if(m.isCheckmate())
			{
				if(c == Color.WHITE)
				{
					game.getBoard().setCheck(Color.BLACK);
					BoardGUI.setStatus("Game over! White wins!");
				}
				else
				{
					game.getBoard().setCheck(Color.WHITE);
					BoardGUI.setStatus("Game over! Black wins!");
				}
				
				game.disconnectFromServer();
				return false;
			}
			
			// Stalemate?
			int status = Integer.parseInt(str[1]);
			if(status > 0)
			{
				m.setStalemate();
				
				if(status == 1)
				{
					BoardGUI.setStatus("Stalemate! No valid moves!");
				}
				else if(status == 2)
				{
					BoardGUI.setStatus("Stalemate! Fifty moves rule");
				}
				else if(status == 3)
				{
					BoardGUI.setStatus("Stalemate! Three move repetition rule");
				}
				
				game.disconnectFromServer();
				return false;
			}

			game.requestMove();
			return true;
			
		// INVALID MOVE
		// We sent a bad move - alert the player
		case INVALID_MOVE:
			m = new Move(data, game.getBoard().getPlayerInTurn());
			game.getBoard().setMoveStats(m);
			
			BoardGUI.setStatus("Invalid move! " + m.getPgnString(true));
			return true;

		// INVALID TYPE
		// Bad message received - end this session
		case INVALID_TYPE:
		default:
			return false;
		}
	}
}
