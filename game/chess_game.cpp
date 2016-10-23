#include <string>
#include <vector>

#include <fly/fly.h>
#include <fly/logging/logger.h>
#include <fly/string/string.h>

#include "chess_game.h"

#include <movement/valid_move_set.h>

namespace chessmate {

//==============================================================================
ChessGamePtr ChessGame::Create(
    const GameConfigPtr &spConfig,
    const fly::SocketPtr &spClientSocket,
    const MoveSetPtr &spMoveSet,
    const Message &msg
)
{
    Message::MessageType type = msg.GetMessageType();
    std::string data = msg.GetData();

    if ((type != Message::START_GAME) || !msg.IsValid())
    {
        return ChessGamePtr();
    }

    std::vector<std::string> arr = fly::String::Split(data, ' ');
    color_type engineColor = std::stoi(arr[0]);
    value_type difficulty = std::stoi(arr[1]);

    ChessGamePtr spGame = std::make_shared<ChessGame>(
        spConfig, spClientSocket, spMoveSet, engineColor, difficulty
    );

    return spGame;
}

//==============================================================================
ChessGamePtr ChessGame::Create(
    const GameConfigPtr &spConfig,
    const fly::SocketPtr &spClientSocket,
    const MoveSetPtr &spMoveSet,
    const color_type &engineColor,
    const value_type &difficulty
)
{
    ChessGamePtr spGame = std::make_shared<ChessGame>(
        spConfig, spClientSocket, spMoveSet, engineColor, difficulty
    );

    std::string data = fly::String::Format("%d %d", engineColor, difficulty);
    Message msg(Message::START_GAME, data);

    return (spGame->sendMessage(msg) ? spGame : ChessGamePtr());
}

//==============================================================================
ChessGame::ChessGame(
    const GameConfigPtr &spConfig,
    const fly::SocketPtr &spClientSocket,
    const MoveSetPtr &spMoveSet,
    const color_type &engineColor,
    const value_type &difficulty
) :
    m_spConfig(spConfig),
    m_gameId(spClientSocket->GetSocketId()),
    m_wpClientSocket(spClientSocket),
    m_wpMoveSet(spMoveSet),
    m_maxDepth(2 * difficulty + 1),
    m_checkMaxDepth(m_spConfig->IncreaseEndGameDifficulty()),
    m_spBoard(std::make_shared<BitBoard>()),
    m_moveSelector(spMoveSet, m_spBoard, engineColor)
{
    LOGC("Initialized game %d: Engine color = %d, max depth = %d",
        m_gameId, engineColor, m_maxDepth);

    LOGI(m_gameId, "Initialized game: Engine color = %d, max depth = %d",
        engineColor, m_maxDepth);
}

//==============================================================================
ChessGame::~ChessGame()
{
    LOGC("Game finished, ID = %d", m_gameId);
}

//==============================================================================
int ChessGame::GetGameID() const
{
    return m_gameId;
}

//==============================================================================
bool ChessGame::IsValid() const
{
    fly::SocketPtr spClientSocket = m_wpClientSocket.lock();

    if (spClientSocket && spClientSocket->IsValid())
    {
        return true;
    }

    LOGW(m_gameId, "Client disconnected");
    return false;
}

//==============================================================================
bool ChessGame::MakeMove(Move &move) const
{
    ValidMoveSet vms(m_wpMoveSet, m_spBoard);
    MoveList list = vms.GetMyValidMoves();

    // Check all valid moves - if this move is valid, make it
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if (move == *it)
        {
            LOGD(m_gameId, "Made valid move: %s", move);
            m_spBoard->MakeMove(move);
            return true;
        }
    }

    LOGD(m_gameId, "Made invalid move: %s", move);
    return false;
}

//==============================================================================
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

        Message m(Message::STARTED_GAME, idStr);
        return sendMessage(m);
    }

    // STARTED GAME
    // Parse game ID
    if (type == Message::START_GAME)
    {
        try
        {
            m_gameId = fly::String::Convert<int>(data);
        }
        catch (...)
        {
            m_gameId = -1;
        }

        return (m_gameId != -1);
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
    LOGW(m_gameId, "Unrecognized message: %d - %s", type, data);
    return false;
}

//==============================================================================
bool ChessGame::sendMessage(const Message &msg) const
{
    std::string serialized = msg.Serialize();
    LOGD(m_gameId, "Sending message %s", serialized);

    fly::SocketPtr spClientSocket = m_wpClientSocket.lock();

    if (spClientSocket && spClientSocket->IsValid())
    {
        return spClientSocket->SendAsync(serialized);
    }

    LOGW(m_gameId, "Client disconnected");
    return false;
}

//==============================================================================
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

//==============================================================================
bool ChessGame::anyValidMoves() const
{
    ValidMoveSet vms(m_wpMoveSet, m_spBoard);
    return !vms.GetMyValidMoves().empty();
}

//==============================================================================
Move ChessGame::getBestMove()
{
    LOGD(m_gameId, "Searching for best move");
    Move m = m_moveSelector.GetBestMove(m_maxDepth);
    LOGD(m_gameId, "Best move is %s", m);

    m_spBoard->MakeMove(m); // Always promote to queen for now

    // Increment max search depth in end game
    if (m_checkMaxDepth && m_spBoard->IsEndGame())
    {
        m_maxDepth += m_spConfig->EndGameDifficultyIncrease();
        m_checkMaxDepth = false;
    }

    return m;
}

}
