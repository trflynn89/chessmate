/**
 * Implementation of the ChessGame interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 3, 2013
 */
#include "ChessGame.h"

#include <string>
#include <vector>

#include <Engine/MoveSelector.h>
#include <Movement/ValidMoveSet.h>
#include <Util/Logging/Logger.h>
#include <Util/String/String.h>

using std::string;
using std::vector;

using ChessMate::Engine::MoveSelector;
using ChessMate::Game::BitBoard;
using ChessMate::Movement::Move;
using ChessMate::Movement::ValidMoveSet;
using ChessMate::Movement::validMoveList_t;
using Util::Socket::SocketPtr;
using Util::String::String;

namespace ChessMate { namespace Game {

//=============================================================================
ChessGame::ChessGame(const SocketPtr &spClientSocket) :
	m_gameId(spClientSocket->GetSocketId()),
	m_wpClientSocket(spClientSocket),
	m_engineColor(0),
	m_maxDepth(0),
	m_checkMaxDepth(true),
	m_board(new BitBoard())
{
	LOGC("Game started, ID = %d", m_gameId);
}

//=============================================================================
ChessGame::~ChessGame()
{
	LOGC("Game finished, ID = %d", m_gameId);

	if (m_board)
	{
		delete m_board;
	}
}

//=============================================================================
int ChessGame::GetGameID() const
{
	return m_gameId;
}

//=============================================================================
BitBoard *ChessGame::GetBoard() const
{
	return m_board;
}

//=============================================================================
bool ChessGame::IsValid() const
{
	SocketPtr spClientSocket = m_wpClientSocket.lock();

	if (spClientSocket && spClientSocket->IsValid())
	{
		return true;
	}

	LOGW(m_gameId, "Client disconnected");
	return false;
}

//=============================================================================
bool ChessGame::MakeMove(Move &move) const
{
	ValidMoveSet vms(*m_board);
	validMoveList_t list = vms.GetMyValidMoves();

	// Check all valid moves - if this move is valid, make it
	for (value_t i = 0; i < vms.GetNumMyValidMoves(); ++i)
	{
		if (move == list[i])
		{
			LOGD(m_gameId, "Made valid move: %s", move.GetPGNString().c_str());
			m_board->MakeMove(move);
			return true;
		}
	}

	LOGD(m_gameId, "Made invalid move: %s", move.GetPGNString().c_str());
	return false;
}

//=============================================================================
bool ChessGame::ProcessMessage(const Message &msg)
{
	Message::MessageType type = msg.GetMessageType();
	string data = msg.GetData();

	// START GAME
	// Parse the engine color and difficulty - still sent even if PvP
	// Send client its game ID
	if (type == Message::START_GAME)
	{
		vector<string> arr = String::Split(data, ' ');

		// Parse the engine color and difficulty
		color_t engineColor = std::stoi(arr[0]);
		value_t difficulty = std::stoi(arr[1]);

		// Initialize the game
		initializeGame(engineColor, difficulty);

		// Respond with game's game ID
		string idStr = std::to_string(m_gameId);

		Message m(Message(Message::START_GAME, idStr));
		return sendMessage(m);
	}

	// MAKE MOVE
	// Parse unambiguous PGN string from client
	// Send move back to client if valid, otherwise invalidate move
	else if (type == Message::MAKE_MOVE)
	{
		Move move(data, m_board->GetPlayerInTurn());
		Message m;

		// Try to make the move
		if (MakeMove(move))
		{
			m = Message(Message::MAKE_MOVE, makeMoveAndStalemateMsg(move));
		}
		else
		{
			m = Message(Message::INVALID_MOVE, move.GetPGNString());
		}

		return sendMessage(m);
	}

	// GET MOVE
	// Use the engine to find a move and send to client
	else if (type == Message::GET_MOVE)
	{
		// Find a move. We know a move will be found - client will only
		// request a move if it knows one can be made.
		Move move = getBestMove();

		Message m(Message::MAKE_MOVE, makeMoveAndStalemateMsg(move));
		return sendMessage(m);
	}

	// DISCONNECT
	// End this game
	else if (type == Message::DISCONNECT)
	{
		return false;
	}

	// UNKNOWN TYPE
	// Unknown message received - log and end this game
	LOGW(m_gameId, "Unrecognized message: %d - %s", type, data.c_str());
	return false;
}

//=============================================================================
bool ChessGame::sendMessage(const Message &msg) const
{
	string ser = msg.Serialize();
	LOGD(m_gameId, "Sending message %s", ser.c_str());

	SocketPtr spClientSocket = m_wpClientSocket.lock();

	if (spClientSocket && spClientSocket->IsValid())
	{
		return spClientSocket->SendAsync(ser);
	}

	LOGW(m_gameId, "Client disconnected");
	return false;
}

//=============================================================================
void ChessGame::initializeGame(color_t engineColor, value_t difficulty)
{
	m_engineColor = engineColor;

	// Set the difficulty-based search depth
	if (difficulty == difficulty_E)
	{
		m_maxDepth = 2;
	}
	if (difficulty == difficulty_M)
	{
		m_maxDepth = 3;
	}
	else if (difficulty == difficulty_H)
	{
		m_maxDepth = 5;
	}

	LOGI(m_gameId, "Initialized game: Engine color = %d, max depth = %d",
		m_engineColor, m_maxDepth);
}

//=============================================================================
string ChessGame::makeMoveAndStalemateMsg(Move &move) const
{
	short stalemateStatus = 0;

	if (!anyValidMoves())
	{
		if (move.IsCheck())
		{
			move.SetCheckmate();
		}
		else
		{
			stalemateStatus = 1;
		}
	}
	else if (m_board->IsStalemateViaFiftyMoves())
	{
		stalemateStatus = 2;
	}
	else if (m_board->IsStalemateViaThreeMoveRep())
	{
		stalemateStatus = 3;
	}

	string stalemateStr = std::to_string(stalemateStatus);
	return (move.GetPGNString() + " " + stalemateStr);
}

//=============================================================================
bool ChessGame::anyValidMoves() const
{
	ValidMoveSet vms(*m_board);

	return (vms.GetNumMyValidMoves() > 0);
}

//=============================================================================
Move ChessGame::getBestMove()
{
	LOGD(m_gameId, "Searching for best move");
	Move m = MoveSelector().GetBestMove(m_engineColor, m_board, m_maxDepth);
	LOGD(m_gameId, "Best move is %s", m.GetPGNString().c_str());

	m_board->MakeMove(m); // Always promote to queen for now

	// Increment max search depth in end game
	if (m_checkMaxDepth && m_board->IsEndGame())
	{
		m_checkMaxDepth = false;
		m_maxDepth += 2;
	}

	return m;
}

}}
