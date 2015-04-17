#pragma once

#include <memory>

#include <ChessMateEngine.h>
#include <Game/BitBoard.h>
#include <Game/Message.h>
#include <Movement/Move.h>
#include <Util/Socket/Socket.h>

namespace ChessMate { namespace Game {

DEFINE_CLASS_PTRS(ChessGame);

/**
 * Class to represent a single chess game, and to maintain its board and
 * communication to its client.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version September 9, 2014
 */
class ChessGame : public std::enable_shared_from_this<ChessGame>
{
public:
	/**
	 * Constructor to set the game's client socket.
	 *
	 * @param SocketPtr The client socket, to be stored as a weak reference.
	 */
	ChessGame(const Util::Socket::SocketPtr &);

	/**
	 * Destructor to close the client socket.
	 */
	~ChessGame();

	/**
	 * @return This game's ID.
	 */
	int GetGameID() const;

	/**
	 * @return This game's board.
	 */
	ChessMate::Game::BitBoard *GetBoard() const;

	/**
	 * Check if the game is still valid, i.e. the client socket is still open.
	 *
	 * @return True if the game is in a valid state, false otherwise.
	 */
	bool IsValid() const;

	/**
	 * If a move is valid, make it.
	 *
	 * @return True if the move was valid, false otherwise.
	 */
	bool MakeMove(ChessMate::Movement::Move &) const;

	/**
	 * Process a message and perform any appropriate action.
	 *
	 * @return True if the game should continue, false otherwise.
	 */
	bool ProcessMessage(const Message &);

private:
	/**
	 * Send a message to the client.
	 *
	 * @param The message to send.
	 */
	bool sendMessage(const Message &) const;

	/**
	 * Initialize the game and its board.
	 *
	 * @param Color of the engine.
	 * @param Difficulty of the engine.
	 */
	void initializeGame(color_t, value_t);

	/**
	 * Retrieve a move's PGN string and determine stalemate status.
	 *
	 * Stalemate status:
	 * 0 = Not in stalemate
	 * 1 = No valid moves
	 * 2 = 50 moves rule reached
	 * 3 = 3 move repetition
	 *
	 * @return String of the format "pgnString stalemateStatus".
	 */
	std::string makeMoveAndStalemateMsg(ChessMate::Movement::Move &move) const;

	/**
	 * @return True if there are any valid moves that can be made.
	 */
	bool anyValidMoves() const;

	/**
	 * Use the engine to figure out the best move on the current board.
	 *
	 * @return The best move calculated by the engine.
	 */
	ChessMate::Movement::Move getBestMove();

	int m_gameId;

	Util::Socket::SocketWPtr m_wpClientSocket;

	color_t m_engineColor;
	value_t m_maxDepth;
	bool m_checkMaxDepth;

	ChessMate::Game::BitBoard *m_board;
};

}}
