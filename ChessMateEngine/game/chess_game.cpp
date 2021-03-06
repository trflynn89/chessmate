#include "chess_game.h"

#include "movement/valid_move_set.h"

#include <fly/logger/logger.hpp>
#include <fly/net/endpoint.hpp>
#include <fly/net/ipv4_address.hpp>
#include <fly/net/socket/tcp_socket.hpp>
#include <fly/types/string/string.hpp>

#include <string>
#include <vector>

namespace chessmate {

//==================================================================================================
std::shared_ptr<ChessGame> ChessGame::Create(
    const std::shared_ptr<GameConfig> &spConfig,
    std::shared_ptr<TcpSocket> spClientSocket,
    const std::shared_ptr<MoveSet> &spMoveSet,
    const Message &msg)
{
    Message::MessageType type = msg.GetMessageType();
    std::string data = msg.GetData();

    if ((type != Message::START_GAME) || !msg.IsValid())
    {
        return std::shared_ptr<ChessGame>();
    }

    std::vector<std::string> arr = fly::String::split(data, ' ');
    color_type engineColor = std::stoi(arr[0]);
    value_type difficulty = std::stoi(arr[1]);

    return std::make_shared<ChessGame>(
        spConfig,
        spClientSocket,
        spMoveSet,
        engineColor,
        difficulty);
}

//==================================================================================================
ChessGame::ChessGame(
    const std::shared_ptr<GameConfig> &spConfig,
    std::shared_ptr<TcpSocket> spClientSocket,
    const std::shared_ptr<MoveSet> &spMoveSet,
    const color_type &engineColor,
    const value_type &difficulty) :
    m_spConfig(spConfig),
    m_gameId(spClientSocket->socket_id()),
    m_client_socket(std::move(spClientSocket)),
    m_wpMoveSet(spMoveSet),
    m_maxDepth(2 * difficulty + 1),
    m_checkMaxDepth(m_spConfig->IncreaseEndGameDifficulty()),
    m_spBoard(std::make_shared<BitBoard>()),
    m_moveSelector(spMoveSet, m_spBoard, engineColor)
{
    fly::logger::Logger::get("console")->info(
        "Initialized game {}: Engine color = {}, max depth = {}",
        m_gameId,
        engineColor,
        m_maxDepth);
    LOGI(
        "Initialized game {}: Engine color = {}, max depth = {}",
        m_gameId,
        engineColor,
        m_maxDepth);
}

//==================================================================================================
ChessGame::~ChessGame()
{
    fly::logger::Logger::get("console")->info("Game finished, ID = {}", m_gameId);
}

//==================================================================================================
int ChessGame::GetGameID() const
{
    return m_gameId;
}

//==================================================================================================
bool ChessGame::IsValid() const
{
    if (m_client_socket->is_open())
    {
        return true;
    }

    LOGW("Client {} disconnected", m_gameId);
    return false;
}

//==================================================================================================
bool ChessGame::MakeMove(Move &move) const
{
    ValidMoveSet vms(m_wpMoveSet, m_spBoard);
    MoveList list = vms.GetMyValidMoves();

    // Check all valid moves - if this move is valid, make it
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if (move == *it)
        {
            LOGD("Game {} made valid move: {}", m_gameId, move);
            m_spBoard->MakeMove(move);
            return true;
        }
    }

    LOGD("Game {} made invalid move: {}", m_gameId, move);
    return false;
}

//==================================================================================================
bool ChessGame::ProcessMessage(const Message &msg)
{
    Message::MessageType type = msg.GetMessageType();
    std::string data = msg.GetData();

    // START GAME
    // Engine color and difficulty have already been parsed
    // Send client its game ID
    if (type == Message::START_GAME)
    {
        std::string idStr = std::to_string(m_gameId);

        Message m(Message(Message::START_GAME, idStr));
        return sendMessage(m);
    }

    // MAKE MOVE
    // Parse unambiguous PGN string from client
    // Send move back to client if valid, otherwise invalidate move
    else if (type == Message::MAKE_MOVE)
    {
        Move move(data, m_spBoard->GetPlayerInTurn());
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
    LOGW("Unrecognized message {}: {} - {}", m_gameId, type, data);
    return false;
}

//==================================================================================================
bool ChessGame::sendMessage(const Message &msg)
{
    m_last_message = msg.Serialize();
    LOGD("Sending message {}: {}", m_gameId, m_last_message);

    // TODO send handle errors.
    return m_client_socket->send_async(m_last_message, [](std::size_t) {});
}

//==================================================================================================
std::string ChessGame::makeMoveAndStalemateMsg(Move &move) const
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
    else if (m_spBoard->IsStalemateViaFiftyMoves())
    {
        stalemateStatus = 2;
    }
    else if (m_spBoard->IsStalemateViaRepetition())
    {
        stalemateStatus = 3;
    }

    std::string stalemateStr = std::to_string(stalemateStatus);
    return (move.GetPGNString() + " " + stalemateStr);
}

//==================================================================================================
bool ChessGame::anyValidMoves() const
{
    ValidMoveSet vms(m_wpMoveSet, m_spBoard);
    return !vms.GetMyValidMoves().empty();
}

//==================================================================================================
Move ChessGame::getBestMove()
{
    LOGD("Searching for best move: {}", m_gameId);
    Move m = m_moveSelector.GetBestMove(m_maxDepth);
    LOGD("Best move is {}: {}", m_gameId, m);

    m_spBoard->MakeMove(m); // Always promote to queen for now

    // Increment max search depth in end game
    if (m_checkMaxDepth && m_spBoard->IsEndGame())
    {
        m_maxDepth += m_spConfig->EndGameDifficultyIncrease();
        m_checkMaxDepth = false;
    }

    return m;
}

} // namespace chessmate
