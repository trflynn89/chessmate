#pragma once

#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <fly/fly.h>
#include <fly/task/runner.h>

namespace fly {

DEFINE_CLASS_PTRS(AsyncRequest);
DEFINE_CLASS_PTRS(ConfigManager);
DEFINE_CLASS_PTRS(Socket);
DEFINE_CLASS_PTRS(SocketManager);

}

namespace chessmate {

DEFINE_CLASS_PTRS(ChessGame);
DEFINE_CLASS_PTRS(GameConfig);
DEFINE_CLASS_PTRS(GameManager);
DEFINE_CLASS_PTRS(Message);
DEFINE_CLASS_PTRS(MoveSet);

/**
 * Manager class to own and control all chess game instances.
 *
 * Provides an interface for managing chess games. Allows for adding new games
 * to the manager, and for stopping any game(s).
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 21, 2016
 */
class GameManager : public fly::Runner
{
public:
    /**
     * Map of games indexed by the client ID.
     */
    typedef std::map<int, ChessGamePtr> GamesMap;

    /**
     * Map of clients awaiting game initialization.
     */
    typedef std::map<int, fly::SocketWPtr> PendingMap;

    /**
     * Constructor, stores a weak reference to the socket manager.
     *
     * @param ConfigManagerPtr Reference to the configuration manager.
     * @param SocketManagerPtr Reference to the socket manager.
     */
    GameManager(fly::ConfigManagerPtr &, const fly::SocketManagerPtr &);

    /**
     * Destructor. Stop all games if they have not been already.
     */
    ~GameManager();

    /**
     * Create and start new game. The game will hold a weak reference to the
     * given client socket.
     *
     * @param SocketPtr Shared pointer to the socket connected to the game client.
     */
    void StartGame(const fly::SocketPtr &);

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

protected:
    /**
     * Intialize the game manager. Create a socket to be used for accepting new
     * game clients, and set the socket manager callbacks for when a client
     * connects or disconnects.
     *
     * @return True if initialization of successful, false otherwise.
     */
    virtual bool StartRunner();

    /**
     * Stop the game manager. Stop all ongoing games and clear the callbacks for
     * client connects/disconnects.
     */
    virtual void StopRunner();

    /**
     * Process any data received by the socket manager.
     *
     * @return True if the game manager is healthy.
     */
    virtual bool DoWork();

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
     * Wait for a short time for a message to be available.
     *
     * @param AsyncRequest Reference to request object to store a receive.
     *
     * @return True if the socket manager could be queried for a request.
     */
    bool receiveSingleMessage(fly::AsyncRequest &) const;

    /**
     * Find a game associated with an AsyncRequest and launch an async task to
     * process the message in the request.
     *
     * @param AsyncRequest The request to process.
     */
    void giveRequestToGame(const fly::AsyncRequest &);

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
     */
    void deleteFinishedFutures();

    GamesMap m_gamesMap;
    PendingMap m_pendingMap;
    std::mutex m_gamesMutex;

    fly::SocketManagerWPtr m_wpSocketManager;

    std::mutex m_runningFuturesMutex;
    std::vector<std::future<void>> m_runningFutures;

    MoveSetPtr m_spMoveSet;

    GameConfigPtr m_spConfig;
};

}
