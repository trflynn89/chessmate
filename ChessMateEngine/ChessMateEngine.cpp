// TODO: log file location, fix evaluator, instrument, faster bit board
// TODO: add ID to async requests
// TODO: max pending IO requests
// TODO: use shared_ptr more liberally

#include <atomic>
#include <chrono>
#include <signal.h>
#include <string>
#include <thread>

#include <Game/GameManager.h>
#include <Util/Logging/Logger.h>
#include <Util/Socket/SocketManager.h>
#include <Util/Socket/SocketManagerImpl.h>
#include <Util/System/System.h>

//=============================================================================
static int g_chessMatePort(12389);

static std::atomic_bool g_aKeepRunning(true);
static std::atomic_int g_aExitSignal(0);

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

    Util::LoggerPtr spLogger = Util::Logger::GetInstance();

    bool fatalSignal = false;
    bool cleanExit = false;
    bool flushLog = false;

    switch (sig)
    {
    case SIGINT:
    case SIGTERM:
        LOGC("Non-fatal signal caught, exiting");
        cleanExit = true;
        break;

#ifdef CHESSMATE_LINUX
    case SIGUSR1:
        LOGC("Flushing the logger");
        flushLog = true;
        break;

    case SIGSYS:
    case SIGBUS:
#endif // CHESSMATE_LINUX
    case SIGILL:
    case SIGFPE:
    case SIGABRT:
    case SIGSEGV:
        LOGC("Fatal signal caught, flushing the logger and exiting");
        fatalSignal = true;
        cleanExit = true;
        flushLog = true;
        break;

    default:
        break;
    }

    if (flushLog && spLogger)
    {
        spLogger->Flush();
    }

    if (cleanExit)
    {
        int exitCode = 0;

        if (fatalSignal)
        {
            Util::System::PrintBacktrace();
            exitCode = sig;
        }

        CleanExit(exitCode);
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
Util::LoggerPtr InitLogger()
{
    auto spLogger = std::make_shared<Util::Logger>();
    Util::Logger::SetInstance(spLogger);

    return spLogger;
}

//=============================================================================
Util::SocketManagerPtr InitSocketManager()
{
    auto spSocketManager = std::make_shared<Util::SocketManagerImpl>();
    spSocketManager->StartSocketManager();

    return spSocketManager;
}

//=============================================================================
void StopSocketManager(Util::SocketManagerPtr &spSocketManager)
{
    if (spSocketManager)
    {
        spSocketManager->StopSocketManager();
    }
}

//=============================================================================
Game::GameManagerPtr InitGameManager(const Util::SocketManagerPtr &spSocketManager)
{
    auto spGameManager = std::make_shared<Game::GameManager>(spSocketManager);

    if (!spGameManager->StartGameManager(g_chessMatePort))
    {
        spGameManager.reset();
        CleanExit(1);
    }

    return spGameManager;
}

//=============================================================================
void StopGameManager(Game::GameManagerPtr &spGameManager)
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
        std::string arg(argv[i]);

        if (arg == "-f")
        {
            flushLogOnExit = true;
        }
    }

    SetupSignalHandler();

    Util::LoggerPtr spLogger = InitLogger();
    Util::SocketManagerPtr spSocketManager = InitSocketManager();
    Game::GameManagerPtr spGameManager = InitGameManager(spSocketManager);

    while (g_aKeepRunning.load())
    {
        // TODO perform system monitoring, provide CLI
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
