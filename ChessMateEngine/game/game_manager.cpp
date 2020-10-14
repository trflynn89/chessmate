#include "game_manager.h"

#include "game/chess_game.h"
#include "game/message.h"
#include "movement/move_set.h"

#include <fly/config/config_manager.hpp>
#include <fly/logger/logger.hpp>
#include <fly/socket/socket.hpp>
#include <fly/socket/socket_manager.hpp>
#include <fly/task/task_runner.hpp>
#include <fly/types/numeric/literals.hpp>

#include <chrono>
#include <functional>
#include <thread>

using namespace fly::literals::numeric_literals;

namespace chessmate {

//==================================================================================================
GameManager::GameManager(
    const std::shared_ptr<fly::ParallelTaskRunner> &spTaskRunner,
    const std::shared_ptr<fly::SocketManager> &spSocketManager,
    const std::shared_ptr<GameConfig> &spConfig) :
    m_spTaskRunner(spTaskRunner),
    m_wpSocketManager(spSocketManager),
    m_spMoveSet(std::make_shared<MoveSet>()),
    m_spConfig(spConfig)
{
}

//==================================================================================================
GameManager::~GameManager()
{
}

//==================================================================================================
bool GameManager::Start()
{
    const int acceptPort = m_spConfig->AcceptPort();
    bool ret = false;

    if (setSocketCallbacks() && createAcceptSocket(acceptPort))
    {
        const unsigned int receivers = std::max(1_u32, std::thread::hardware_concurrency());

        LOGI("Accepting games on port %d with %u receivers", acceptPort, receivers);
        fly::Logger::get("console")->info(
            "Accepting games on port %d with %u receivers",
            acceptPort,
            receivers);

        for (unsigned int i = 0_u32; i < receivers; ++i)
        {
            auto task = std::make_shared<MessageReceiver>(shared_from_this());
            m_spTaskRunner->post_task(task);
            m_receiverTasks.push_back(task);
        }

        ret = true;
    }
    else
    {
        LOGE("Could not start game manager on port %d", acceptPort);
        fly::Logger::get("console")->error("Could not start game manager on port %d", acceptPort);
    }

    return ret;
}

//==================================================================================================
void GameManager::Stop()
{
    fly::Logger::get("console")->info("Stopping game manager");

    std::shared_ptr<fly::SocketManager> spSocketManager = m_wpSocketManager.lock();

    if (spSocketManager)
    {
        spSocketManager->clear_client_callbacks();
    }

    m_receiverTasks.clear();
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

//==================================================================================================
void GameManager::StartGame(std::shared_ptr<fly::Socket> spClientSocket)
{
    std::lock_guard<std::mutex> lock(m_gamesMutex);
    m_pendingMap[spClientSocket->get_socket_id()] = spClientSocket;
}

//==================================================================================================
void GameManager::StopGame(std::shared_ptr<fly::Socket> spClientSocket)
{
    StopGame(spClientSocket->get_socket_id());
}

//==================================================================================================
void GameManager::StopGame(int socketId)
{
    LOGI("Stopping game %d", socketId);

    std::lock_guard<std::mutex> lock(m_gamesMutex);
    m_pendingMap.erase(socketId);
    m_gamesMap.erase(socketId);
}

//==================================================================================================
void GameManager::StopAllGames()
{
    std::lock_guard<std::mutex> lock(m_gamesMutex);

    LOGI("Stopping %u games", m_gamesMap.size());
    m_gamesMap.clear();
}

//==================================================================================================
bool GameManager::processMessage()
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

//==================================================================================================
bool GameManager::setSocketCallbacks()
{
    std::shared_ptr<fly::SocketManager> spSocketManager = m_wpSocketManager.lock();

    if (spSocketManager)
    {
        const auto &spThis = shared_from_this();

        auto newClient = std::bind(&GameManager::StartGame, spThis, std::placeholders::_1);
        auto closedClient = std::bind<void (GameManager::*)(std::shared_ptr<fly::Socket>)>(
            &GameManager::StopGame,
            spThis,
            std::placeholders::_1);

        spSocketManager->set_client_callbacks(newClient, closedClient);
    }

    return (spSocketManager.get() != nullptr);
}

//==================================================================================================
bool GameManager::createAcceptSocket(int acceptPort)
{
    std::shared_ptr<fly::SocketManager> spSocketManager = m_wpSocketManager.lock();
    std::shared_ptr<fly::Socket> spSocket;

    if (spSocketManager)
    {
        std::weak_ptr<fly::Socket> wpSocket =
            spSocketManager->create_async_socket(fly::Protocol::TCP);
        spSocket = wpSocket.lock();
    }

    if (spSocket)
    {
        if (!spSocket->bind(fly::Socket::in_addr_any(), acceptPort, fly::BindOption::AllowReuse))
        {
            LOGE("Could not bind socket to port %d", acceptPort);
            spSocket.reset();
        }

        else if (!spSocket->listen())
        {
            LOGE("Could not listen on port %d", acceptPort);
            spSocket.reset();
        }
    }

    return (spSocket && spSocket->is_listening());
}

//==================================================================================================
bool GameManager::receiveSingleMessage(fly::AsyncRequest &request) const
{
    std::shared_ptr<fly::SocketManager> spSocketManager = m_wpSocketManager.lock();

    if (spSocketManager)
    {
        spSocketManager->wait_for_completed_receive(request, m_spConfig->QueueWaitTime());
    }
    else
    {
        LOGE("No socket manager");
        return false;
    }

    return true;
}

//==================================================================================================
void GameManager::giveRequestToGame(const fly::AsyncRequest &request)
{
    std::shared_ptr<ChessGame> spGame;
    Message message;

    if (request.is_valid())
    {
        message = Message(request.get_request());

        if (message.IsValid())
        {
            spGame = createOrFindGame(request.get_socket_id(), message);
        }
        else
        {
            LOGW(
                "Cannot convert request to message %d: %s",
                request.get_socket_id(),
                request.get_request());
        }
    }

    if (spGame && spGame->IsValid())
    {
        LOGD("Handling message type %d: %d", spGame->GetGameID(), message.GetMessageType());

        const auto &spThis = shared_from_this();
        auto function = &GameManager::handleMessage;

        auto future = std::async(std::launch::async, function, spThis, spGame, message);

        std::lock_guard<std::mutex> lock(m_runningFuturesMutex);
        m_runningFutures.push_back(std::move(future));
    }
}

//==================================================================================================
std::shared_ptr<ChessGame> GameManager::createOrFindGame(int socketId, const Message &message)
{
    std::lock_guard<std::mutex> lock(m_gamesMutex);

    if (message.GetMessageType() == Message::START_GAME)
    {
        if (m_pendingMap.find(socketId) == m_pendingMap.end())
        {
            LOGW("No pending game associated with socket: %d", socketId);
            return std::shared_ptr<ChessGame>();
        }

        std::shared_ptr<fly::Socket> spSocket = m_pendingMap[socketId].lock();
        m_pendingMap.erase(socketId);

        if (spSocket)
        {
            std::shared_ptr<ChessGame> spGame =
                ChessGame::Create(m_spConfig, spSocket, m_spMoveSet, message);
            m_gamesMap[socketId] = spGame;
        }
        else
        {
            LOGW("Socket closed while handling START_GAME message: %d", socketId);
            return std::shared_ptr<ChessGame>();
        }
    }

    if (m_gamesMap.find(socketId) == m_gamesMap.end())
    {
        LOGW("No game associated with socket: %d", socketId);
        return std::shared_ptr<ChessGame>();
    }

    return m_gamesMap[socketId];
}

//==================================================================================================
void GameManager::handleMessage(const std::shared_ptr<ChessGame> spGame, const Message message)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    bool keepPlaying = spGame->ProcessMessage(message);
    auto endTime = std::chrono::high_resolution_clock::now();

    auto timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    int gameId = spGame->GetGameID();

    LOGD(
        "Game %d processed message type %d in %f seconds",
        gameId,
        message.GetMessageType(),
        timeSpan.count());

    if (!keepPlaying || !spGame->IsValid())
    {
        LOGI(
            "Game %d will be stopped, keepPlaying = %d, isValid = %d",
            gameId,
            keepPlaying,
            spGame->IsValid());

        StopGame(gameId);
    }
}

//==================================================================================================
void GameManager::deleteFinishedFutures()
{
    static const std::chrono::seconds noWait = std::chrono::seconds::zero();

    std::unique_lock<std::mutex> lock(m_runningFuturesMutex, std::try_to_lock);

    if (lock.owns_lock())
    {
        for (auto it = m_runningFutures.begin(); it != m_runningFutures.end();)
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

//==================================================================================================
MessageReceiver::MessageReceiver(std::weak_ptr<GameManager> wpGameManager) noexcept :
    m_wpGameManager(wpGameManager)
{
}

//==================================================================================================
void MessageReceiver::run()
{
    auto spGameManager = m_wpGameManager.lock();

    if (spGameManager && spGameManager->processMessage())
    {
        spGameManager->m_spTaskRunner->post_task(shared_from_this());
    }
}

} // namespace chessmate
