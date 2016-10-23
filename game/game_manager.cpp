#include <chrono>
#include <functional>

#include <fly/logging/logger.h>
#include <fly/config/config_manager.h>
#include <fly/socket/socket.h>
#include <fly/socket/socket_manager.h>

#include "game_manager.h"

#include <game/client_game_manager.h>
#include <game/game_config.h>
#include <game/server_game_manager.h>
#include <movement/move_set.h>

namespace chessmate {

//==============================================================================
GameManagerPtr GameManager::Create(
    fly::ConfigManagerPtr &spConfigManager,
    const fly::SocketManagerPtr &spSocketManager,
    bool isServer
)
{
    GameManagerPtr spGameManager;

    if (isServer)
    {
        spGameManager = std::make_shared<ServerGameManager>(spConfigManager, spSocketManager);
    }
    else
    {
        spGameManager = std::make_shared<ClientGameManager>(spConfigManager, spSocketManager);
    }

    return spGameManager;
}

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
    bool success = true;

    if (setSocketCallbacks() && createGameSocket(acceptPort))
    {
        LOGI(-1, "Started game manager on port %d", acceptPort);
        LOGC("Started game manager on port %d", acceptPort);
    }
    else
    {
        LOGE(-1, "Could not start game manager on port %d", acceptPort);
        LOGC("Could not start game manager on port %d", acceptPort);

        success = false;
    }

    return success;
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

}
