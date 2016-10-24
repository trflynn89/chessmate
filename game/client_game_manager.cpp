#include <chrono>
#include <functional>

#include <fly/logging/logger.h>
#include <fly/config/config_manager.h>
#include <fly/socket/socket.h>
#include <fly/socket/socket_manager.h>

#include "client_game_manager.h"

#include <game/chess_game.h>
#include <game/message.h>
#include <movement/move_set.h>

namespace chessmate {

//==============================================================================
ClientGameManager::ClientGameManager(
    fly::ConfigManagerPtr &spConfigManager,
    const fly::SocketManagerPtr &spSocketManager
) :
    GameManager(spConfigManager, spSocketManager)
{
}

//==============================================================================
ClientGameManager::~ClientGameManager()
{
    Stop();
}

//==============================================================================
bool ClientGameManager::DoWork()
{
    fly::AsyncConnect connect;
    bool healthy = checkForConnection(connect);

    if (healthy)
    {
        std::lock_guard<std::mutex> lock(m_gamesMutex);
        createGameFromConnect(connect);
    }

    return healthy;
}

//==============================================================================
bool ClientGameManager::createGameSocket(int connectPort)
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
        auto state = spSocket->ConnectAsync("localhost", connectPort);

        if (state == fly::Socket::NOT_CONNECTED)
        {
            LOGE(-1, "Could not connect to port %d", connectPort);
            spSocket.reset();
        }
    }

    return (spSocket && (spSocket->IsConnecting() || spSocket->IsConnected()));
}

//==============================================================================
bool ClientGameManager::checkForConnection(fly::AsyncConnect &connect) const
{
    fly::SocketManagerPtr spSocketManager = m_wpSocketManager.lock();

    if (spSocketManager)
    {
        spSocketManager->WaitForCompletedConnect(connect, m_spConfig->QueueWaitTime());
    }
    else
    {
        LOGE(-1, "No socket manager");
        return false;
    }

    return true;
}

//==============================================================================
void ClientGameManager::createGameFromConnect(const fly::AsyncConnect &connect)
{
    fly::SocketPtr spSocket = m_wpGameSocket.lock();

    if (spSocket && connect.IsValid())
    {
        ChessGamePtr spGame = ChessGame::Create(
            m_spConfig, spSocket, m_spMoveSet, 0, 1
        );

        m_gamesMap[connect.GetSocketId()] = spGame;
    }
}

}
