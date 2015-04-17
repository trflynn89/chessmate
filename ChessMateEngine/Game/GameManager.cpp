#include <chrono>
#include <functional>

#include "GameManager.h"

#include <Util/Logging/Logger.h>

using std::async;
using std::bind;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::seconds;
using std::future_status;
using std::launch;
using std::lock_guard;
using std::make_shared;
using std::mutex;
using std::placeholders::_1;
using std::thread;
using std::vector;

using Util::Socket::AsyncRequest;
using Util::Socket::Socket;
using Util::Socket::SocketPtr;
using Util::Socket::SocketManagerPtr;

namespace ChessMate { namespace Game {

namespace
{
	static const seconds s_queueWaitTime(1);
	static const int s_defaultNumberOfCores = 1;
}

//=============================================================================
GameManager::GameManager(const SocketManagerPtr &spSocketManager) :
	m_wpSocketManager(spSocketManager),
	m_aKeepRunning(true)
{
}

//=============================================================================
GameManager::~GameManager()
{
	if (m_aKeepRunning.load())
	{
		StopGameManager();
	}
}

//=============================================================================
bool GameManager::StartGameManager(int acceptPort)
{
	bool ret = false;

	if (setSocketCallbacks() && createAcceptSocket(acceptPort))
	{
		createMessageReceivers();

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

//=============================================================================
void GameManager::StopGameManager()
{
	LOGC("Stopping game manager");

	SocketManagerPtr spSocketManager = m_wpSocketManager.lock();

	if (spSocketManager)
	{
		spSocketManager->ClearClientCallbacks();
	}

	m_aKeepRunning.store(false);

	for (auto &future : m_workerFutures)
	{
		if (future.valid())
		{
			future.get();
		}
	}

	StopAllGames();
}

//=============================================================================
void GameManager::StartGame(const SocketPtr &spClientSocket)
{
	ChessGamePtr spGame = make_shared<ChessGame>(spClientSocket);

	if (spGame->IsValid())
	{
		lock_guard<mutex> lock(m_gamesMutex);
		m_gamesMap[spClientSocket->GetSocketId()] = spGame;
	}
}

//=============================================================================
void GameManager::StopGame(int socketId)
{
	LOGI(-1, "Stopping game %d", socketId);

	lock_guard<mutex> lock(m_gamesMutex);
	m_gamesMap.erase(socketId);
}

//=============================================================================
void GameManager::StopAllGames()
{
	LOGI(-1, "Stopping %u games", GetNumberOfGames());

	lock_guard<mutex> lock(m_gamesMutex);
	m_gamesMap.clear();
}

//=============================================================================
GameManager::GamesMapSize GameManager::GetNumberOfGames()
{
	lock_guard<mutex> lock(m_gamesMutex);
	return m_gamesMap.size();
}

//=============================================================================
bool GameManager::setSocketCallbacks()
{
	SocketManagerPtr spSocketManager = m_wpSocketManager.lock();

	if (spSocketManager)
	{
		const GameManagerPtr &spThis = shared_from_this();

		auto newClient = bind(&GameManager::StartGame, spThis, _1);
		auto closedClient = bind(&GameManager::StopGame, spThis, _1);

		spSocketManager->SetClientCallbacks(newClient, closedClient);
	}

	return (spSocketManager.get() != NULL);
}

//=============================================================================
bool GameManager::createAcceptSocket(int acceptPort)
{
	SocketManagerPtr spSocketManager = m_wpSocketManager.lock();
	SocketPtr spSocket;

	if (spSocketManager)
	{
		spSocket = spSocketManager->CreateAsyncTcpSocket();
	}

	if (spSocket)
	{
		if (!spSocket->BindForReuse(Socket::InAddrAny(), acceptPort))
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

//=============================================================================
void GameManager::createMessageReceivers()
{
	unsigned int numCores = thread::hardware_concurrency();

	if (numCores == 0)
	{
		numCores = s_defaultNumberOfCores;
	}

	LOGI(-1, "Creating %u message receivers", numCores);

	for (unsigned int i = 0; i < numCores; ++i)
	{
		const GameManagerPtr &spThis = shared_from_this();
		auto function = &GameManager::messageReceiver;

		m_workerFutures.push_back(async(launch::async, function, spThis));
	}
}

//=============================================================================
void GameManager::messageReceiver()
{
	FutureVector futures;

	while (m_aKeepRunning.load())
	{
		AsyncRequest request = receiveSingleMessage();

		// The receiver may have been signaled to stop while waiting for a
		// request, so check if we should actually process the message
		if (m_aKeepRunning.load())
		{
			giveRequestToGame(futures, request);
		}

		deleteFinishedFutures(futures);
	}
}

//=============================================================================
AsyncRequest GameManager::receiveSingleMessage() const
{
	SocketManagerPtr spSocketManager = m_wpSocketManager.lock();
	AsyncRequest request;

	// TODO what to do if the socket manager is gone? Stop game manager?
	if (spSocketManager)
	{
		spSocketManager->WaitForCompletedReceive(request, s_queueWaitTime);
	}
	else
	{
		LOGE(-1, "No socket manager");
	}

	return request;
}

//=============================================================================
void GameManager::giveRequestToGame(FutureVector &futures, const AsyncRequest &request)
{
	ChessGamePtr spGame;
	Message message;

	if (request.IsValid())
	{
		message = Message(request.GetRequest());

		if (message.IsValid())
		{
			lock_guard<mutex> lock(m_gamesMutex);
			spGame = m_gamesMap[request.GetSocketId()];
		}
		else
		{
			LOGW(request.GetSocketId(), "Cannot convert request to message: %s",
				request.GetRequest().c_str());
		}
	}

	if (spGame)
	{
		LOGD(spGame->GetGameID(), "Handling message type: %d", message.GetMessageType());

		const GameManagerPtr &spThis = shared_from_this();
		auto function = &GameManager::handleMessage;

		auto future = async(launch::async, function, spThis, spGame, message);
		futures.push_back(std::move(future));
	}
	else if (message.IsValid())
	{
		LOGW(request.GetSocketId(), "No game associated with socket");
	}
}

//=============================================================================
void GameManager::handleMessage(const ChessGamePtr spGame, const Message message)
{
	high_resolution_clock::time_point startTime = high_resolution_clock::now();

	bool keepPlaying = spGame->ProcessMessage(message);

	high_resolution_clock::time_point endTime = high_resolution_clock::now();

	duration<double> timeSpan = duration_cast<duration<double>>(endTime - startTime);

	int gameId = spGame->GetGameID();

	LOGD(gameId, "Processed message type %d in %f seconds",
		message.GetMessageType(), timeSpan.count());

	if (!keepPlaying || !spGame->IsValid())
	{
		LOGI(gameId, "Game will be stopped, keepPlaying = %d isValid = %d",
			keepPlaying, spGame->IsValid());

		StopGame(gameId);
	}
}

//=============================================================================
void GameManager::deleteFinishedFutures(FutureVector &futures)
{
	for (auto it = futures.begin(); it != futures.end(); )
	{
		future_status status = it->wait_for(seconds::zero());

		if (status == future_status::ready)
		{
			it = futures.erase(it);
		}
		else
		{
			++it;
		}
	}
}

}}
