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

namespace
{
    static int g_chessMatePort(12389);

    static std::atomic_bool g_aKeepRunning(true);
    static std::atomic_int g_aExitSignal(0);

    //=========================================================================
    void CleanExit(int exitCode)
    {
        g_aExitSignal.store(exitCode);
        g_aKeepRunning.store(false);
    }

    //=========================================================================
    void HandleSignal(int sig)
    {
        LOGC_NO_LOCK("Received signal %d", sig);
        LOGI(-1, "Received signal %d", sig);

        Util::LoggerPtr spLogger = Util::Logger::GetInstance();

        bool fatalSignal = false;
        bool cleanExit = false;

        switch (sig)
        {
        case SIGINT:
        case SIGTERM:
            LOGC_NO_LOCK("Non-fatal exit signal caught");
            cleanExit = true;
            break;

    #ifdef BUILD_LINUX
        case SIGSYS:
        case SIGBUS:
    #endif // BUILD_LINUX
        case SIGILL:
        case SIGFPE:
        case SIGABRT:
        case SIGSEGV:
            LOGC_NO_LOCK("Fatal exit signal caught");
            fatalSignal = true;
            cleanExit = true;
            break;

        default:
            break;
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

    //=========================================================================
    void SetupSignalHandler()
    {
        signal(SIGINT, HandleSignal);
        signal(SIGTERM, HandleSignal);
    #ifdef BUILD_LINUX
        signal(SIGSYS, HandleSignal);
        signal(SIGBUS, HandleSignal);
    #endif // BUILD_LINUX
        signal(SIGILL, HandleSignal);
        signal(SIGFPE, HandleSignal);
        signal(SIGABRT, HandleSignal);
        signal(SIGSEGV, HandleSignal);
    }

    //=========================================================================
    Util::LoggerPtr InitLogger()
    {
        auto spLogger = std::make_shared<Util::Logger>();

        Util::Logger::SetInstance(spLogger);
        spLogger->StartLogger();

        return spLogger;
    }

    //=========================================================================
    void StopLogger(Util::LoggerPtr &spLogger)
    {
        if (spLogger)
        {
            spLogger->StopLogger();
        }
    }

    //=========================================================================
    Util::SocketManagerPtr InitSocketManager()
    {
        auto spSocketManager = std::make_shared<Util::SocketManagerImpl>();
        spSocketManager->StartSocketManager();

        return spSocketManager;
    }

    //=========================================================================
    void StopSocketManager(Util::SocketManagerPtr &spSocketManager)
    {
        if (spSocketManager)
        {
            spSocketManager->StopSocketManager();
        }
    }

    //=========================================================================
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

    //=========================================================================
    void StopGameManager(Game::GameManagerPtr &spGameManager)
    {
        if (spGameManager)
        {
            spGameManager->StopGameManager();
        }
    }
}

//=============================================================================
int main()
{
    LOGC("Starting ChessMateEngine");
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
    StopLogger(spLogger);

    LOGC("Exiting ChessMateEngine");
    return g_aExitSignal.load();
}
