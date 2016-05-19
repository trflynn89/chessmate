// TODO: log file location, fix evaluator, instrument, faster bit board
// TODO: add ID to async requests
// TODO: max pending IO requests
// TODO: use shared_ptr more liberally

#include <atomic>
#include <chrono>
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

    //=========================================================================
    Util::LoggerPtr InitLogger()
    {
        auto spLogger = std::make_shared<Util::Logger>();

        if (spLogger->StartLogger())
        {
            Util::Logger::SetInstance(spLogger);
        }
        else
        {
            LOGC("Could not start logger, using console instead");
            spLogger.reset();
        }

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
            Util::System::CleanExit(1);
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
    Util::System::SetupSignalHandler();

    Util::LoggerPtr spLogger = InitLogger();
    Util::SocketManagerPtr spSocketManager = InitSocketManager();
    Game::GameManagerPtr spGameManager = InitGameManager(spSocketManager);

    while (Util::System::KeepRunning())
    {
        // TODO perform system monitoring, provide CLI
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    StopGameManager(spGameManager);
    StopSocketManager(spSocketManager);
    StopLogger(spLogger);

    LOGC("Exiting ChessMateEngine");
    return Util::System::ExitCode();
}
