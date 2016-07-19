#include <atomic>
#include <chrono>
#include <string>
#include <thread>

#include <Game/GameManager.h>
#include <Util/ExitCodes.h>
#include <Util/Configuration/ConfigurationManager.h>
#include <Util/Logging/Logger.h>
#include <Util/Socket/SocketManager.h>
#include <Util/Socket/SocketManagerImpl.h>
#include <Util/System/System.h>

namespace
{
    static int g_chessMatePort = 12389;

    static std::string g_chessmateDirectory;

    //==========================================================================
    void InitChessMateDirectory()
    {
        const char sep = Util::System::GetSeparator();
        const std::string temp = Util::System::GetTempDirectory();

        g_chessmateDirectory = Util::String::Join(sep, temp, "ChessMate");
    }

    //==========================================================================
    Util::ConfigurationManagerPtr InitConfigManager()
    {
        auto spConfigManager = std::make_shared<Util::ConfigurationManager>(
            Util::ConfigurationManager::CONFIG_TYPE_INI,
            g_chessmateDirectory, "ChessMate.ini"
        );

        if (!spConfigManager->StartConfigurationManager())
        {
            spConfigManager.reset();
        }

        return spConfigManager;
    }

    //==========================================================================
    void StopConfigManager(Util::ConfigurationManagerPtr &spConfigManager)
    {
        if (spConfigManager)
        {
            spConfigManager->StopConfigurationManager();
        }
    }

    //==========================================================================
    Util::LoggerPtr InitLogger(Util::ConfigurationManagerPtr &spConfigManager)
    {
        Util::LoggerPtr spLogger;

        if (spConfigManager)
        {
            spLogger = std::make_shared<Util::Logger>(
                spConfigManager, g_chessmateDirectory
            );

            if (spLogger->StartLogger())
            {
                Util::Logger::SetInstance(spLogger);
            }
            else
            {
                LOGC("Could not start logger, using console instead");
                spLogger.reset();
            }
        }

        return spLogger;
    }

    //==========================================================================
    void StopLogger(Util::LoggerPtr &spLogger)
    {
        if (spLogger)
        {
            spLogger->StopLogger();
        }
    }

    //==========================================================================
    Util::SocketManagerPtr InitSocketManager()
    {
        auto spSocketManager = std::make_shared<Util::SocketManagerImpl>();
        spSocketManager->StartSocketManager();

        return spSocketManager;
    }

    //==========================================================================
    void StopSocketManager(Util::SocketManagerPtr &spSocketManager)
    {
        if (spSocketManager)
        {
            spSocketManager->StopSocketManager();
        }
    }

    //==========================================================================
    Game::GameManagerPtr InitGameManager(const Util::SocketManagerPtr &spSocketManager)
    {
        auto spGameManager = std::make_shared<Game::GameManager>(spSocketManager);

        if (!spGameManager->StartGameManager(g_chessMatePort))
        {
            spGameManager.reset();
            Util::System::CleanExit(Util::InitFailed);
        }

        return spGameManager;
    }

    //==========================================================================
    void StopGameManager(Game::GameManagerPtr &spGameManager)
    {
        if (spGameManager)
        {
            spGameManager->StopGameManager();
        }
    }
}

//==============================================================================
int main()
{
    LOGC("Starting ChessMateEngine");
    Util::System::SetupSignalHandler();

    InitChessMateDirectory();

    Util::ConfigurationManagerPtr spConfigManager = InitConfigManager();
    Util::LoggerPtr spLogger = InitLogger(spConfigManager);
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
    StopConfigManager(spConfigManager);

    LOGC("Exiting ChessMateEngine");
    return Util::System::GetExitCode();
}
