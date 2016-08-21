#include <chrono>
#include <functional>

#include <fly/logging/logger.h>
#include <fly/config/config_manager.h>
#include <fly/socket/socket.h>
#include <fly/socket/socket_manager.h>

#include "game_manager.h"

#include <game/chess_game.h>
#include <game/message.h>
#include <movement/move_set.h>

namespace chessmate {

//==============================================================================
GameManager::GameManager(
    fly::ConfigManagerPtr &spConfigManager,
    const fly::SocketManagerPtr &spSocketManager
) :
    Runner(spConfigManager, "GameManager"),
    m_wpSocketManager(spSocketManager),
    m_spMoveSet(std::make_shared<MoveSet>()),
    m_spConfig(spConfigManager->CreateConfig<GameConfig>())
{
}

//==============================================================================
GameManager::~GameManager()
{
}

//==============================================================================
void GameManager::StartGame(const fly::SocketPtr &spClientSocket)
{
    std::lock_guard<std::mutex> lock(m_gamesMutex);
    m_pendingMap[spClientSocket->GetSocketId()] = spClientSocket;
}

//==============================================================================
void GameManager::StopGame(int socketId)
{
    std::lock_guard<std::mutex> lock(m_gamesMutex);

    LOGI(-1, "Stopping game %d", socketId);
    m_pendingMap.erase(socketId);
    m_gamesMap.erase(socketId);
}

//==============================================================================
void GameManager::StopAllGames()
{
    std::lock_guard<std::mutex> lock(m_gamesMutex);

    LOGI(-1, "Stopping %u games", m_gamesMap.size());
    m_gamesMap.clear();
}

//==============================================================================
bool GameManager::StartRunner()
{
    const int acceptPort = m_spConfig->AcceptPort();
    bool ret = false;

    if (setSocketCallbacks() && createAcceptSocket(acceptPort))
    {
        LOGI(-1, "Accepting games on port %d", acceptPort);
        LOGC("Accepting games on port %d", acceptPort);

        ret = true;
    }
    else
    {
        LOGE(-1, "Could not start game manager on port %d", acceptPort);
        LOGC("Could not start game manager on port %d", acceptPort);
    }

    return ret;
}

//==============================================================================
void GameManager::StopRunner()
{
    LOGC("Stopping game manager");

    fly::SocketManagerPtr spSocketManager = m_wpSocketManager.lock();

    if (spSocketManager)
    {
        spSocketManager->ClearClientCallbacks();
    }

    StopAllGames();

    std::lock_guard<std::mutex> lock(m_runningFuturesMutex);

    for (auto &future : m_runningFutures)
    {
        if (future.valid())
        {
            future.get();
        }
    }
}

//==============================================================================
bool GameManager::DoWork()
{
    fly::AsyncRequest request;
    bool healthy = receiveSingleMessage(request);

    if (healthy)
    {
        giveRequestToGame(request);
    }

    deleteFinishedFutures();
    return healthy;
}

//==============================================================================
bool GameManager::setSocketCallbacks()
{
    fly::SocketManagerPtr spSocketManager = m_wpSocketManager.lock();

    if (spSocketManager)
    {
        const GameManagerPtr &spThis = SharedFromThis<GameManager>();

        auto newClient = std::bind(&GameManager::StartGame, spThis, std::placeholders::_1);
        auto closedClient = std::bind(&GameManager::StopGame, spThis, std::placeholders::_1);

        spSocketManager->SetClientCallbacks(newClient, closedClient);
    }

    return (spSocketManager.get() != NULL);
}

//==============================================================================
bool GameManager::createAcceptSocket(int acceptPort)
{
    fly::SocketManagerPtr spSocketManager = m_wpSocketManager.lock();
    fly::SocketPtr spSocket;

    if (spSocketManager)
    {
        fly::SocketWPtr wpSocket = spSocketManager->CreateAsyncTcpSocket();
        spSocket = wpSocket.lock();
    }

    if (spSocket)
    {
        if (!spSocket->BindForReuse(fly::Socket::InAddrAny(), acceptPort))
        {
            LOGE(-1, "Could not bind socket to port %d", acceptPort);
            spSocket.reset();
        }

        else if (!spSocket->Listen())
        {
            LOGE(-1, "Could not listen on port %d", acceptPort);
            spSocket.reset();
        }
    }

    return (spSocket && spSocket->IsListening());
}

//==============================================================================
bool GameManager::receiveSingleMessage(fly::AsyncRequest &request) const
{
    fly::SocketManagerPtr spSocketManager = m_wpSocketManager.lock();

    if (spSocketManager)
    {
        spSocketManager->WaitForCompletedReceive(request, m_spConfig->QueueWaitTime());
    }
    else
    {
        LOGE(-1, "No socket manager");
        return false;
    }

    return true;
}

//==============================================================================
void GameManager::giveRequestToGame(const fly::AsyncRequest &request)
{
    ChessGamePtr spGame;
    Message message;

    if (request.IsValid())
    {
        message = Message(request.GetRequest());

        if (message.IsValid())
        {
            spGame = createOrFindGame(request.GetSocketId(), message);
        }
        else
        {
            LOGW(request.GetSocketId(), "Cannot convert request to message: %s",
                request.GetRequest());
        }
    }

    if (spGame && spGame->IsValid())
    {
        LOGD(spGame->GetGameID(), "Handling message type: %d", message.GetMessageType());

        const GameManagerPtr &spThis = SharedFromThis<GameManager>();
        auto function = &GameManager::handleMessage;

        auto future = std::async(std::launch::async, function, spThis, spGame, message);

        std::lock_guard<std::mutex> lock(m_runningFuturesMutex);
        m_runningFutures.push_back(std::move(future));
    }
}

//==============================================================================
ChessGamePtr GameManager::createOrFindGame(int socketId, const Message &message)
{
    std::lock_guard<std::mutex> lock(m_gamesMutex);

    if (message.GetMessageType() == Message::START_GAME)
    {
        if (m_pendingMap.find(socketId) == m_pendingMap.end())
        {
            LOGW(socketId, "No pending game associated with socket");
            return ChessGamePtr();
        }

        fly::SocketPtr spSocket = m_pendingMap[socketId].lock();
        m_pendingMap.erase(socketId);

        if (spSocket)
        {
            ChessGamePtr spGame = ChessGame::Create(
                m_spConfig, spSocket, m_spMoveSet, message
            );

            m_gamesMap[socketId] = spGame;
        }
        else
        {
            LOGW(socketId, "Socket closed while handling START_GAME message");
            return ChessGamePtr();
        }
    }

    if (m_gamesMap.find(socketId) == m_gamesMap.end())
    {
        LOGW(socketId, "No game associated with socket");
        return ChessGamePtr();
    }

    return m_gamesMap[socketId];
}

//==============================================================================
void GameManager::handleMessage(const ChessGamePtr spGame, const Message message)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    bool keepPlaying = spGame->ProcessMessage(message);
    auto endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> timeSpan =
        std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

    int gameId = spGame->GetGameID();

    LOGD(gameId, "Processed message type %d in %f seconds",
        message.GetMessageType(), timeSpan.count());

    if (!keepPlaying || !spGame->IsValid())
    {
        LOGI(gameId, "Game will be stopped, keepPlaying = %d, isValid = %d",
            keepPlaying, spGame->IsValid());

        StopGame(gameId);
    }
}

//==============================================================================
void GameManager::deleteFinishedFutures()
{
    static const std::chrono::seconds noWait = std::chrono::seconds::zero();

    std::unique_lock<std::mutex> lock(m_runningFuturesMutex, std::try_to_lock);

    if (lock.owns_lock())
    {
        for (auto it = m_runningFutures.begin(); it != m_runningFutures.end(); )
        {
            if (it->wait_for(noWait) == std::future_status::ready)
            {
                it = m_runningFutures.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

}
