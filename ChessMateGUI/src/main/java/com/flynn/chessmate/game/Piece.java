package com.flynn.chessmate.game;

import java.awt.Color;

/**
 * Enumeration for a chess piece.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version July 24, 2011
 */
public enum Piece
{
	WHITE_PAWN		(Color.WHITE, 1),
	WHITE_KNIGHT	(Color.WHITE, 3),
	WHITE_BISHOP	(Color.WHITE, 3),
	WHITE_ROOK		(Color.WHITE, 5),
	WHITE_QUEEN		(Color.WHITE, 9),
	WHITE_KING		(Color.WHITE, Integer.MAX_VALUE),

	BLACK_PAWN		(Color.BLACK, 1),
	BLACK_KNIGHT	(Color.BLACK, 3),
	BLACK_BISHOP	(Color.BLACK, 3),
	BLACK_ROOK		(Color.BLACK, 5),
	BLACK_QUEEN		(Color.BLACK, 9),
	BLACK_KING		(Color.BLACK, Integer.MAX_VALUE);

	private Color m_color;
	private int m_value;

	/**
	 * Constructor.
	 *
	 * @param color The color of the piece (black or white).
	 * @param value The number of pawns the piece is worth.
	 */
	private Piece(Color color, int value)
	{
		this.m_color = color;
		this.m_value = value;
	}

	/**
	 * @return The color of the piece (black or white).
	 */
	public Color getColor()
	{
		return m_color;
	}

	/**
	 * @return The number of pawns the piece is worth.
	 */
	public int getValue()
	{
		return m_value;
	}
}
