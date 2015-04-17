#pragma once

#include <atomic>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <ChessMateEngine.h>
#include <Game/ChessGame.h>
#include <Game/Message.h>
#include <Util/Socket/Socket.h>
#include <Util/Socket/SocketManager.h>

namespace ChessMate { namespace Game {

DEFINE_CLASS_PTRS(GameManager);

/**
 * @brief Manager class to own and control all chess game instances.
 *
 * Provides an interface for managing chess games. Allows for adding new games
 * to the manager, and for stopping any game(s).
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version September 9, 2014
 */
class GameManager : public std::enable_shared_from_this<GameManager>
{
public:
	/**
	 * @brief Map of games indexed by the client ID.
	 */
	typedef std::map<int, ChessGamePtr> GamesMap;

	/**
	 * @brief POD type for the size of a games map.
	 */
	typedef GamesMap::size_type GamesMapSize;

	/**
	 * @brief Vector of futures for void return values.
	 */
	typedef std::vector<std::future<void>> FutureVector;

	/**
	 * Constructor, stores a weak reference to the socket manager.
	 *
	 * @param SocketManagerPtr Reference to the socket manager.
	 */
	GameManager(const Util::Socket::SocketManagerPtr &);

	/**
	 * Destructor. Stop all games if they have not been already.
	 */
	~GameManager();

	/**
	 * Intialize the game manager. Create a socket to be used for accepting new
	 * game clients, and set the socket manager callbacks for when a client
	 * connects or disconnects.
	 *
	 * @param int The port to listen on.
	 *
	 * @return bool True if initialization of successful, false otherwise.
	 */
	bool StartGameManager(int);

	/**
	 * Stop the game manager. Stop all ongoing games, stop the worker threads,
	 * and clear the callbacks for client connects/disconnects.
	 */
	void StopGameManager();

	/**
	 * Create and start new game. The game will hold a weak reference to the
	 * given client socket.
	 *
	 * @param SocketPtr Shared pointer to the socket connected to the game client.
	 */
	void StartGame(const Util::Socket::SocketPtr &);

	/**
	 * Stop and delete a game.
	 *
	 * @param int The socket ID of the game.
	 */
	void StopGame(int);

	/**
	 * Stop and delete all games being managed.
	 */
	void StopAllGames();

	/**
	 * Get the number of games currently being managed.
	 *
	 * @return The size of the in-memory game map.
	 */
	GamesMapSize GetNumberOfGames();

private:
	/**
	 * Set the socket manager callbacks for when a new client connects or an
	 * existing client disconnects.
	 *
	 * @return True if the callbacks were set, false otherwise.
	 */
	bool setSocketCallbacks();

	/**
	 * Create the accept socket for new games to connect to.
	 *
	 * @param int The port to listen on.
	 *
	 * @return True if the socket could be created and initialized.
	 */
	bool createAcceptSocket(int);

	/**
	 * Create the game manager's IO worker threads.
	 */
	void createMessageReceivers();

	/**
	 * IO worker thread function to process data received by the socket
	 * manager.
	 */
	void messageReceiver();

	/**
	 * Wait for a short time for a message to be available.
	 *
	 * @return AsyncRequest The completed receive if found, an invalid receive.
	 */
	Util::Socket::AsyncRequest receiveSingleMessage() const;

	/**
	 * Find a game associated with an AsyncRequest and launch an async task to
	 * process the message in the request.
	 *
	 * @param FutureVector Vector to push the new task's future onto.
	 * @param AsyncRequest The request to process.
	 */
	void giveRequestToGame(FutureVector &, const Util::Socket::AsyncRequest &);

	/**
	 * Handle a message retrieved by the message receiver thread.
	 *
	 * @param ChessGamePtr The chess game the message is intended for.
	 * @param Message The message to process.
	 */
	void handleMessage(const ChessGamePtr, const Message);

	/**
	 * Remove any completed futures from the vector of futures.
	 *
	 * @param FutureVector Vector to check for finished futures.
	 */
	void deleteFinishedFutures(FutureVector &);

	GamesMap m_gamesMap;
	std::mutex m_gamesMutex;

	Util::Socket::SocketManagerWPtr m_wpSocketManager;

	FutureVector m_workerFutures;
	std::atomic_bool m_aKeepRunning;
};

}}
