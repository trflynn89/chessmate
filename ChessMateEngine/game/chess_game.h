#pragma once

#include "engine/move_selector.h"
#include "game/bit_board.h"
#include "game/game_config.h"
#include "game/message.h"
#include "movement/move.h"
#include "movement/move_set.h"

#include <memory>

namespace fly::net {

class IPv4Address;

template <typename IPAddressType>
class Endpoint;

template <typename EndpointType>
class TcpSocket;

} // namespace fly::net

namespace chessmate {

/**
 * Class to represent a single chess game, and to maintain its board and
 * communication to its client.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version September 9, 2014
 */
class ChessGame : public std::enable_shared_from_this<ChessGame>
{
    using TcpSocket = fly::net::TcpSocket<fly::net::Endpoint<fly::net::IPv4Address>>;

    /**
     * Enumerated list of game difficulties.
     */
    enum Difficulty
    {
        EASY,
        MEDIUM,
        HARD
    };

public:
    /**
     * Create a ChessGame instance from the initialization message received
     * from the game client.
     *
     * @param std::shared_ptr<GameConfig> The game configuration.
     * @param SocketPtr The game client's socket.
     * @param std::shared_ptr<MoveSet> The list of possible moves.
     * @param Message The START_GAME message containing the client's settings.
     *
     * @return A shared pointer around the created ChessGame instance.
     */
    static std::shared_ptr<ChessGame> Create(
        const std::shared_ptr<GameConfig> &,
        std::shared_ptr<TcpSocket>,
        const std::shared_ptr<MoveSet> &,
        const Message &);

    /**
     * Constructor to set the game's client socket.
     *
     * @param std::shared_ptr<GameConfig> The game configuration.
     * @param SocketPtr The game client's socket.
     * @param std::shared_ptr<MoveSet> The list of possible moves.
     * @param color_type The color of the engine.
     * @param value_type The difficulty of the engine.
     */
    ChessGame(
        const std::shared_ptr<GameConfig> &,
        std::shared_ptr<TcpSocket>,
        const std::shared_ptr<MoveSet> &,
        const color_type &,
        const value_type &);

    /**
     * Destructor to close the client socket.
     */
    ~ChessGame();

    /**
     * @return This game's ID - same as the client's socket ID.
     */
    int GetGameID() const;

    /**
     * Check if the game is still valid, i.e. the client socket is still open.
     *
     * @return True if the game is in a valid state, false otherwise.
     */
    bool IsValid() const;

    /**
     * If a move is valid, make it.
     *
     * @param Move The move to attempt to make.
     *
     * @return True if the move was valid, false otherwise.
     */
    bool MakeMove(Move &) const;

    /**
     * Process a message and perform any appropriate action.
     *
     * @return True if the game should continue, false otherwise.
     */
    bool ProcessMessage(const Message &);

    std::shared_ptr<TcpSocket> client() const
    {
        return m_client_socket;
    }

private:
    /**
     * Send a message to the client.
     *
     * @param Message The message to send.
     *
     * @return True if the message could be sent.
     */
    bool sendMessage(const Message &);

    /**
     * Retrieve a move's PGN string and determine stalemate status.
     *
     * Stalemate status:
     * 0 = Not in stalemate
     * 1 = No valid moves
     * 2 = 50 moves rule reached
     * 3 = 3 move repetition
     *
     * @param Move The move to convert to a PGN string.
     *
     * @return String of the format "pgnString stalemateStatus".
     */
    std::string makeMoveAndStalemateMsg(Move &move) const;

    /**
     * @return True if there are any valid moves that can be made.
     */
    bool anyValidMoves() const;

    /**
     * Use the engine to figure out the best move on the current board.
     *
     * @return The best move calculated by the engine.
     */
    Move getBestMove();

    const std::shared_ptr<GameConfig> m_spConfig;

    int m_gameId;

    std::shared_ptr<TcpSocket> m_client_socket;
    std::weak_ptr<MoveSet> m_wpMoveSet;

    value_type m_maxDepth;
    bool m_checkMaxDepth;

    std::shared_ptr<BitBoard> m_spBoard;

    MoveSelector m_moveSelector;

    std::string m_last_message;
};

} // namespace chessmate
