#include <chrono>
#include <functional>

#include <fly/logging/logger.h>
#include <fly/config/config_manager.h>
#include <fly/socket/socket.h>
#include <fly/socket/socket_manager.h>

#include "server_game_manager.h"

#include <game/chess_game.h>
#include <game/message.h>
#include <movement/move_set.h>

namespace chessmate {

//==============================================================================
ServerGameManager::ServerGameManager(
    fly::ConfigManagerPtr &spConfigManager,
    const fly::SocketManagerPtr &spSocketManager
) :
    GameManager(spConfigManager, spSocketManager)
{
}

//==============================================================================
ServerGameManager::~ServerGameManager()
{
    Stop();

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
bool ServerGameManager::DoWork()
{
    bool healthy = false;
    {
        std::lock_guard<std::mutex> lock(m_gamesMutex);

        fly::AsyncRequest request;
        healthy = receiveSingleMessage(request);

        if (healthy)
        {
            giveRequestToGame(request);
        }
    }

    deleteFinishedFutures();
    return healthy;
}

//==============================================================================
bool ServerGameManager::createGameSocket(int acceptPort)
{
    fly::SocketManagerPtr spSocketManager = m_wpSocketManager.lock();
    fly::SocketPtr spSocket;

    if (spSocketManager)
    {
        m_wpGameSocket = spSocketManager->CreateAsyncTcpSocket();
        spSocket = m_wpGameSocket.lock();
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
bool ServerGameManager::receiveSingleMessage(fly::AsyncRequest &request) const
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
void ServerGameManager::giveRequestToGame(const fly::AsyncRequest &request)
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

        const ServerGameManagerPtr &spThis = SharedFromThis<ServerGameManager>();
        auto function = &ServerGameManager::handleMessage;

        auto future = std::async(std::launch::async, function, spThis, spGame, message);

        std::lock_guard<std::mutex> lock(m_runningFuturesMutex);
        m_runningFutures.push_back(std::move(future));
    }
}

//==============================================================================
ChessGamePtr ServerGameManager::createOrFindGame(int socketId, const Message &message)
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
void ServerGameManager::handleMessage(const ChessGamePtr spGame, const Message message)
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
void ServerGameManager::deleteFinishedFutures()
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
