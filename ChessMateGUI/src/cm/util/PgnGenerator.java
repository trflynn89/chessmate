package cm.util;

import java.awt.Color;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;

import javax.swing.JOptionPane;

import cm.movement.Move;

/**
 * Class to generate a PGN file based on the moves made thus far.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version August 2, 2011
 */
public class PgnGenerator
{
	private Color m_playerColor;
	private boolean  m_isComputerOpponent;
	private ArrayList<Move> m_whiteMoves;
	private ArrayList<Move> m_blackMoves;
	private String m_fileName;
	
	/**
	 * Constructor.
	 * 
	 * @param playerColor The color of the human player.
	 * @param isComputerOpponent True if the opponent was the engine.
	 * @param moves The list of all moves made thus far.
	 * @param fileName The absolute path of the PGN file to create.
	 */
	public PgnGenerator
	(
		Color playerColor, boolean isComputerOpponent, String fileName,
		ArrayList<Move> whiteMoves, ArrayList<Move> blackMoves
	)
	{
		this.m_playerColor = playerColor;
		this.m_isComputerOpponent = isComputerOpponent;
		this.m_whiteMoves = whiteMoves;
		this.m_blackMoves = blackMoves;
		this.m_fileName = fileName;
	}

	/**
	 * Generate the file.
	 */
	public void generate()
	{
		String whitePlayer, blackPlayer;
		String result = getResult();
		
		if(m_isComputerOpponent)
		{
			whitePlayer =
				(m_playerColor == Color.WHITE ? "Human" : "ChessMate");
			blackPlayer =
				(m_playerColor == Color.BLACK ? "Human" : "ChessMate");
		}
		else
		{
			whitePlayer = "Human 1";
			blackPlayer = "Human 2";
		}
		
		try
		{
			BufferedWriter out = new BufferedWriter(
				new FileWriter(m_fileName));
			
			// Event tags
			writeWithNL(out, "[Event \"\"]");
			writeWithNL(out, "[Site \"\"]");
			writeWithNL(out, "[Date \"" + getDate() + "\"]");
			writeWithNL(out, "[Round \"\"]");
			writeWithNL(out, "[White \"" + whitePlayer + "\"]");
			writeWithNL(out, "[Black \"" + blackPlayer + "\"]");
			writeWithNL(out, "[Result \"" + result + "\"]");
			writeWithNL(out, "");
			
			// Moves
			for(int i=0; i<m_whiteMoves.size(); i++)
			{
				out.write((i+1) + ".");
				out.write(m_whiteMoves.get(i).getPgnString(true) + " ");
				
				if(m_blackMoves.size() > i)
				{
					out.write(m_blackMoves.get(i).getPgnString(true));
				}
				
				out.newLine();
			}
			
			out.write(result);
			out.close();
		}
		catch (IOException e)
		{
			JOptionPane.showMessageDialog(null,
				"Error creating PGN file: " + m_fileName, "Error",
				JOptionPane.ERROR_MESSAGE);
		}
	}
	
	private void writeWithNL(BufferedWriter out, String str) throws IOException
	{
		out.write(str); 
		out.newLine();
	}
	
	/**
	 * @return A string representation of the date.
	 */
	private String getDate()
	{
		Calendar cal = Calendar.getInstance();
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy.MM.dd");
		return sdf.format(cal.getTime());
	}
	
	/**
	 * @return The final result of the game.
	 */
	private String getResult()
	{
		int numWhiteMoves = m_whiteMoves.size();
		int numBlackMoves = m_blackMoves.size();
		
		if(numWhiteMoves > 0)
		{
			Move lastWhiteMove = m_whiteMoves.get(numWhiteMoves-1);
			Move lastBlackMove = m_blackMoves.get(numBlackMoves-1);
			
			if(numWhiteMoves > numBlackMoves)
			{
				if(lastWhiteMove.isCheckmate())
				{
					return "1-0";
				}
				else if(lastWhiteMove.isStalemate())
				{
					return "1/2-1/2";
				}
			}
			else
			{
				if(lastBlackMove.isCheckmate())
				{
					return "0-1";
				}
				else if(lastBlackMove.isStalemate())
				{
					return "1/2-1/2";
				}
			}
		}
		
		return "*";
	}
}
