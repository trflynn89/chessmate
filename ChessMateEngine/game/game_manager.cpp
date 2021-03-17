#include "game_manager.h"

#include "game/chess_game.h"
#include "game/message.h"
#include "movement/move_set.h"

#include <fly/config/config_manager.hpp>
#include <fly/logger/logger.hpp>
#include <fly/net/endpoint.hpp>
#include <fly/net/ipv4_address.hpp>
#include <fly/net/socket/listen_socket.hpp>
#include <fly/net/socket/socket_service.hpp>
#include <fly/net/socket/tcp_socket.hpp>
#include <fly/task/task_runner.hpp>
#include <fly/types/numeric/literals.hpp>

#include <chrono>
#include <functional>
#include <thread>

using namespace fly::literals::numeric_literals;

namespace chessmate {

//==================================================================================================
GameManager::GameManager(
    const std::shared_ptr<fly::task::ParallelTaskRunner> &spTaskRunner,
    const std::shared_ptr<fly::net::SocketService> &spSocketService,
    const std::shared_ptr<GameConfig> &spConfig) :
    m_spTaskRunner(spTaskRunner),
    m_wpSocketService(spSocketService),
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
    const std::uint16_t acceptPort = static_cast<std::uint16_t>(m_spConfig->AcceptPort());
    bool ret = false;

    if (createAcceptSocket(acceptPort))
    {
        const unsigned int receivers = std::max(1_u32, std::thread::hardware_concurrency());

        LOGI("Accepting games on port {} with {} receivers", acceptPort, receivers);
        fly::logger::Logger::get("console")->info(
            "Accepting games on port {} with {} receivers",
            acceptPort,
            receivers);

        for (unsigned int i = 0_u32; i < receivers; ++i)
        {
            if (!processMessage())
            {
                return false;
            }
        }

        ret = true;
    }
    else
    {
        LOGE("Could not start game manager on port {}", acceptPort);
        fly::logger::Logger::get("console")->error(
            "Could not start game manager on port {}",
            acceptPort);
    }

    return ret;
}

//==================================================================================================
void GameManager::Stop()
{
    fly::logger::Logger::get("console")->info("Stopping game manager");
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
void GameManager::StartGame(std::shared_ptr<TcpSocket> spClientSocket)
{
    std::lock_guard<std::mutex> lock(m_gamesMutex);

    spClientSocket->receive_async(
        [this, socket_id = spClientSocket->socket_id()](std::string message)
        {
            m_pending_messages.push({socket_id, std::move(message)});
        });

    m_pendingMap[spClientSocket->socket_id()] = std::move(spClientSocket);
}

//==================================================================================================
void GameManager::StopGame(std::shared_ptr<TcpSocket> spClientSocket)
{
    StopGame(spClientSocket->socket_id());
}

//==================================================================================================
void GameManager::StopGame(int socketId)
{
    LOGI("Stopping game {}", socketId);

    std::lock_guard<std::mutex> lock(m_gamesMutex);
    m_pendingMap.erase(socketId);
    m_gamesMap.erase(socketId);
}

//==================================================================================================
void GameManager::StopAllGames()
{
    std::lock_guard<std::mutex> lock(m_gamesMutex);

    LOGI("Stopping {} games", m_gamesMap.size());
    m_gamesMap.clear();
}

//==================================================================================================
bool GameManager::receive_message(std::uint64_t socket_id)
{
    if (auto it = m_gamesMap.find(socket_id); it != m_gamesMap.end())
    {
        return it->second->client()->receive_async(
            [this, socket_id](std::string message)
            {
                if (!message.empty())
                {
                    m_pending_messages.push({socket_id, std::move(message)});
                    receive_message(socket_id);
                }
            });
    }

    LOGW("No game associated with socket: {}", socket_id);
    return false;
}

//==================================================================================================
bool GameManager::processMessage()
{
    std::weak_ptr<GameManager> weak_self = shared_from_this();

    auto task = [weak_self]()
    {
        if (auto self = weak_self.lock(); self)
        {
            AsyncRequest request;

            if (self->m_pending_messages.pop(request, self->m_spConfig->QueueWaitTime()))
            {
                self->giveRequestToGame(request);
            }

            self->processMessage();
            self->deleteFinishedFutures();
        }
    };

    return m_spTaskRunner->post_task(FROM_HERE, std::move(task));
}

//==================================================================================================
bool GameManager::createAcceptSocket(std::uint16_t acceptPort)
{
    std::shared_ptr<fly::net::SocketService> spSocketService = m_wpSocketService.lock();
    m_accept_socket = spSocketService->create_socket<ListenSocket>();

    if (m_accept_socket)
    {
        if (!m_accept_socket->bind(
                {fly::net::IPv4Address::in_addr_any(), acceptPort},
                fly::net::BindMode::AllowReuse))
        {
            LOGE("Could not bind socket to port {}", acceptPort);
            m_accept_socket.reset();
        }
        else if (!m_accept_socket->listen())
        {
            LOGE("Could not listen on port {}", acceptPort);
            m_accept_socket.reset();
        }

        receive_client();
    }

    return (m_accept_socket && m_accept_socket->is_listening());
}

//==================================================================================================
void GameManager::receive_client()
{
    if (!m_accept_socket->accept_async(
            [this](std::shared_ptr<TcpSocket> spClientSocket)
            {
                StartGame(std::move(spClientSocket));
                receive_client();
            }))
    {
        LOGE("Could not initiate accepting new client");
        m_accept_socket.reset();
    }
}

//==================================================================================================
void GameManager::giveRequestToGame(const AsyncRequest &request)
{
    Message message(request.m_message);
    std::shared_ptr<ChessGame> spGame;

    if (message.IsValid())
    {
        spGame = createOrFindGame(request.m_socket_id, message);
    }
    else
    {
        LOGW("Cannot convert request to message {}: {}", request.m_socket_id, request.m_message);
    }

    if (spGame && spGame->IsValid())
    {
        LOGD("Handling message type {}: {}", spGame->GetGameID(), message.GetMessageType());

        const auto &spThis = shared_from_this();
        auto function = &GameManager::handleMessage;

        auto future = std::async(std::launch::async, function, spThis, spGame, message);

        std::lock_guard<std::mutex> lock(m_runningFuturesMutex);
        m_runningFutures.push_back(std::move(future));
    }
}

//==================================================================================================
std::shared_ptr<ChessGame>
GameManager::createOrFindGame(std::uint64_t socketId, const Message &message)
{
    std::lock_guard<std::mutex> lock(m_gamesMutex);

    if (message.GetMessageType() == Message::START_GAME)
    {
        if (m_pendingMap.find(socketId) == m_pendingMap.end())
        {
            LOGW("No pending game associated with socket: {}", socketId);
            return nullptr;
        }

        std::shared_ptr<TcpSocket> spSocket = std::move(m_pendingMap[socketId]);
        m_pendingMap.erase(socketId);

        if (spSocket)
        {
            std::shared_ptr<ChessGame> spGame =
                ChessGame::Create(m_spConfig, std::move(spSocket), m_spMoveSet, message);
            m_gamesMap[socketId] = spGame;

            receive_message(socketId);
        }
        else
        {
            LOGW("Socket closed while handling START_GAME message: {}", socketId);
            return nullptr;
        }
    }

    if (m_gamesMap.find(socketId) == m_gamesMap.end())
    {
        LOGW("No game associated with socket: {}", socketId);
        return nullptr;
    }

    return m_gamesMap[socketId];
}

//==================================================================================================
void GameManager::handleMessage(const std::shared_ptr<ChessGame> spGame, const Message message)
{
    auto startTime = std::chrono::steady_clock::now();
    bool keepPlaying = spGame->ProcessMessage(message);
    auto endTime = std::chrono::steady_clock::now();

    auto timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    int gameId = spGame->GetGameID();

    LOGD(
        "Game {} processed message type {} in {:f} seconds",
        gameId,
        message.GetMessageType(),
        timeSpan.count());

    if (keepPlaying && spGame->IsValid())
    {
        spGame->client()->receive_async(
            [this, socket_id = spGame->client()->socket_id()](std::string message)
            {
                m_pending_messages.push({socket_id, std::move(message)});
            });
    }
    else
    {
        LOGI(
            "Game {} will be stopped, keepPlaying = {}, isValid = {}",
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

} // namespace chessmate
