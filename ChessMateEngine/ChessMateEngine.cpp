// TODO: log file location, fix evaluator, instrument, faster bit board
// TODO: add ID to async requests
// TODO: get last error skt api
// TODO: max pending IO requests
// TODO: use shared_ptr more liberally
// TODO: MoveSet should be owned by the game manager

#include <atomic>
#include <chrono>
#include <signal.h>
#include <string>
#include <thread>

#include <ChessMateEngine.h>
#include <Game/GameManager.h>
#include <Movement/MoveSet.h>
#include <Util/Logging/Logger.h>
#include <Util/Socket/SocketManager.h>
#include <Util/Socket/SocketManagerImpl.h>
#include <Util/System/System.h>

using std::atomic_bool;
using std::atomic_int;
using std::chrono::seconds;
using std::make_shared;
using std::string;
using std::this_thread::sleep_for;

using ChessMate::Game::GameManager;
using ChessMate::Game::GameManagerPtr;
using ChessMate::Movement::MoveSet;
using Util::Logging::Logger;
using Util::Logging::LoggerPtr;
using Util::Socket::Socket;
using Util::Socket::SocketPtr;
using Util::Socket::SocketManager;
using Util::Socket::SocketManagerImpl;
using Util::Socket::SocketManagerPtr;
using Util::System::System;

//=============================================================================
atomic_bool g_aKeepRunning(true);
atomic_int g_aExitSignal(0);

//=============================================================================
void CleanExit(int sig)
{
	g_aExitSignal.store(sig);
	g_aKeepRunning.store(false);
}

//=============================================================================
void HandleSignal(int sig)
{
	LOGI(-1, "Received signal %d", sig);
	LOGC("Received signal %d", sig);

	switch (sig)
	{
	case SIGINT:
	case SIGTERM:
		CleanExit(0);
		break;

#ifdef CHESSMATE_LINUX
	case SIGUSR1:
		LOGC("Flushing the logger");

		Logger::GetInstance()->Flush();
		break;

	case SIGSYS:
	case SIGBUS:
#endif // CHESSMATE_LINUX
	case SIGILL:
	case SIGFPE:
	case SIGABRT:
	case SIGSEGV:
		System::PrintBacktrace(10);
		Logger::GetInstance()->Flush();
		CleanExit(sig);
		break;

	default:
		break;
	}
}

//=============================================================================
void SetupSignalHandler()
{
	signal(SIGINT, HandleSignal);
	signal(SIGTERM, HandleSignal);
#ifdef CHESSMATE_LINUX
	// TODO USR1 and USR2 don't exist in Windows - how to flush buffer with signals?
	signal(SIGUSR1, HandleSignal);
	signal(SIGSYS, HandleSignal);
	signal(SIGBUS, HandleSignal);
#endif // CHESSMATE_LINUX
	signal(SIGILL, HandleSignal);
	signal(SIGFPE, HandleSignal);
	signal(SIGABRT, HandleSignal);
	signal(SIGSEGV, HandleSignal);
}

//=============================================================================
LoggerPtr InitLogger()
{
	LoggerPtr spLogger = make_shared<Logger>();
	Logger::SetInstance(spLogger);

	return spLogger;
}

//=============================================================================
SocketManagerPtr InitSocketManager()
{
	SocketManagerPtr spSocketManager = make_shared<SocketManagerImpl>();
	spSocketManager->StartSocketManager();

	return spSocketManager;
}

//=============================================================================
void StopSocketManager(SocketManagerPtr &spSocketManager)
{
	if (spSocketManager)
	{
		spSocketManager->StopSocketManager();
	}
}

//=============================================================================
GameManagerPtr InitGameManager(const SocketManagerPtr &spSocketManager)
{
	GameManagerPtr spGameManager = make_shared<GameManager>(spSocketManager);

	if (!spGameManager->StartGameManager(CHESSMATE_PORT))
	{
		spGameManager.reset();
		CleanExit(0);
	}

	return spGameManager;
}

//=============================================================================
void StopGameManager(GameManagerPtr &spGameManager)
{
	if (spGameManager)
	{
		spGameManager->StopGameManager();
	}
}

//=============================================================================
int main(int argc, char **argv)
{
	bool flushLogOnExit = false;

	for (int i = 1; i < argc; ++i)
	{
		string arg(argv[i]);

		if (arg == "-f")
		{
			flushLogOnExit = true;
		}
	}

	SetupSignalHandler();

	LoggerPtr spLogger = InitLogger();

	MoveSet::Initialize();

	SocketManagerPtr spSocketManager = InitSocketManager();
	GameManagerPtr spGameManager = InitGameManager(spSocketManager);

	while (g_aKeepRunning.load())
	{
		// TODO perform system monitoring, provide CLI
		sleep_for(seconds(1));
	}

	StopGameManager(spGameManager);
	StopSocketManager(spSocketManager);

	if (flushLogOnExit)
	{
		LOGC("Flushing the logger");
		spLogger->Flush();
	}

	LOGC("Exiting ChessMateEngine");
	return g_aExitSignal.load();
}
