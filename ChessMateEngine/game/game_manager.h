#pragma once

#include "fly/types/concurrency/concurrent_queue.hpp"

#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace fly::net {

class IPv4Address;
class SocketService;

template <typename IPAddressType>
class Endpoint;

template <typename EndpointType>
class ListenSocket;

template <typename EndpointType>
class TcpSocket;

} // namespace fly::net

namespace fly::task {
class ParallelTaskRunner;
} // namespace fly::task

namespace chessmate {

class ChessGame;
class GameConfig;
class Message;
class MoveSet;

/**
 * Manager class to own and control all chess game instances.
 *
 * Provides an interface for managing chess games. Allows for adding new games
 * to the manager, and for stopping any game(s).
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 21, 2016
 */
class GameManager : public std::enable_shared_from_this<GameManager>
{
    using ListenSocket = fly::net::ListenSocket<fly::net::Endpoint<fly::net::IPv4Address>>;
    using TcpSocket = fly::net::TcpSocket<fly::net::Endpoint<fly::net::IPv4Address>>;

public:
    /**
     * Map of games indexed by the client ID.
     */
    typedef std::map<int, std::shared_ptr<ChessGame>> GamesMap;

    /**
     * Map of clients awaiting game initialization.
     */
    typedef std::map<int, std::shared_ptr<TcpSocket>> PendingMap;

    /**
     * Constructor, stores a weak reference to the socket manager.
     *
     * @param SocketManagerPtr Reference to the socket manager.
     */
    GameManager(
        const std::shared_ptr<fly::task::ParallelTaskRunner> &,
        const std::shared_ptr<fly::net::SocketService> &,
        const std::shared_ptr<GameConfig> &);

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
    void StartGame(std::shared_ptr<TcpSocket>);

    /**
     * Stop and delete a game.
     *
     * @param int The socket ID of the game.
     */
    void StopGame(std::shared_ptr<TcpSocket>);
    void StopGame(int);

    /**
     * Stop and delete all games being managed.
     */
    void StopAllGames();

    /**
     * Intialize the game manager. Create a socket to be used for accepting new
     * game clients, and set the socket manager callbacks for when a client
     * connects or disconnects.
     *
     * @return True if initialization of successful, false otherwise.
     */
    bool Start();

    /**
     * Stop the game manager. Stop all ongoing games and clear the callbacks for
     * client connects/disconnects.
     */
    void Stop();

private:
    struct AsyncRequest
    {
        std::uint64_t m_socket_id {0};
        std::string m_message;
    };

    bool receive_message(std::uint64_t socket_id);
    void receive_client();

    /**
     * Process any data received by the socket manager.
     *
     * @return True if the game manager is healthy.
     */
    bool processMessage();

    /**
     * Create the accept socket for new games to connect to.
     *
     * @param int The port to listen on.
     *
     * @return True if the socket could be created and initialized.
     */
    bool createAcceptSocket(std::uint16_t);

    /**
     * Find a game associated with an AsyncRequest and launch an async task to
     * process the message in the request.
     *
     * @param AsyncRequest The request to process.
     */
    void giveRequestToGame(const AsyncRequest &);

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
    std::shared_ptr<ChessGame> createOrFindGame(std::uint64_t, const Message &);

    /**
     * Handle a message retrieved by the message receiver thread.
     *
     * @param std::shared_ptr<ChessGame> The chess game the message is intended for.
     * @param Message The message to process.
     */
    void handleMessage(const std::shared_ptr<ChessGame>, const Message);

    /**
     * Remove any completed futures from the vector of futures.
     */
    void deleteFinishedFutures();

    GamesMap m_gamesMap;
    PendingMap m_pendingMap;
    std::mutex m_gamesMutex;

    std::shared_ptr<fly::task::ParallelTaskRunner> m_spTaskRunner;
    std::weak_ptr<fly::net::SocketService> m_wpSocketService;
    std::shared_ptr<ListenSocket> m_accept_socket;

    fly::ConcurrentQueue<AsyncRequest> m_pending_messages;

    std::mutex m_runningFuturesMutex;
    std::vector<std::future<void>> m_runningFutures;

    std::shared_ptr<MoveSet> m_spMoveSet;

    std::shared_ptr<GameConfig> m_spConfig;
};

} // namespace chessmate
