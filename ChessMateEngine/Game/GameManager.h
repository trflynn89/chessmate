#pragma once

#include <atomic>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <Game/ChessGame.h>
#include <Game/GameConfig.h>
#include <Game/Message.h>
#include <Movement/MoveSet.h>
#include <Util/Config/ConfigManager.h>
#include <Util/Socket/Socket.h>
#include <Util/Socket/SocketManager.h>

namespace Game {

DEFINE_CLASS_PTRS(GameManager);

/**
 * Manager class to own and control all chess game instances.
 *
 * Provides an interface for managing chess games. Allows for adding new games
 * to the manager, and for stopping any game(s).
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 19, 2016
 */
class GameManager : public std::enable_shared_from_this<GameManager>
{
public:
    /**
     * Map of games indexed by the client ID.
     */
    typedef std::map<int, ChessGamePtr> GamesMap;

    /*!
     * Map of clients awaiting game initialization.
     */
    typedef std::map<int, Util::SocketWPtr> PendingMap;

    /**
     * Vector of futures for void return values.
     */
    typedef std::vector<std::future<void>> FutureVector;

    /**
     * Constructor, stores a weak reference to the socket manager.
     *
     * @param ConfigManagerPtr Reference to the configuration manager.
     * @param SocketManagerPtr Reference to the socket manager.
     */
    GameManager(Util::ConfigManagerPtr &, const Util::SocketManagerPtr &);

    /**
     * Destructor. Stop all games if they have not been already.
     */
    ~GameManager();

    /**
     * Intialize the game manager. Create a socket to be used for accepting new
     * game clients, and set the socket manager callbacks for when a client
     * connects or disconnects.
     *
     * @return True if initialization of successful, false otherwise.
     */
    bool StartGameManager();

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
    void StartGame(const Util::SocketPtr &);

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
    Util::AsyncRequest receiveSingleMessage() const;

    /**
     * Find a game associated with an AsyncRequest and launch an async task to
     * process the message in the request.
     *
     * @param FutureVector Vector to push the new task's future onto.
     * @param AsyncRequest The request to process.
     */
    void giveRequestToGame(FutureVector &, const Util::AsyncRequest &);

    /**
     * Depending on the given message type, either create a chess game or find
     * an already-existing chess game from the given socket ID.
     *
     * A chess game is blindy created for START_GAME messages. If a client sent
     * this message already, its previous game object will be released.
     *
     * @param int Socket ID of the client.
     * @param Message The message from the client.
     *
     * @return A shared pointer around the created ChessGame instance.
     */
    ChessGamePtr createOrFindGame(int, const Message &);

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
    PendingMap m_pendingMap;
    std::mutex m_gamesMutex;

    Util::SocketManagerWPtr m_wpSocketManager;

    FutureVector m_workerFutures;
    std::atomic_bool m_aKeepRunning;

    Movement::MoveSetPtr m_spMoveSet;

    GameConfigPtr m_spConfig;
};

}
