#include <atomic>
#include <chrono>
#include <thread>

#include <fly/exit_codes.h>
#include <fly/config/config_manager.h>
#include <fly/logging/logger.h>
#include <fly/socket/socket_manager_impl.h>
#include <fly/system/system.h>

#include <chessmate.h>
#include <game/game_manager.h>

namespace chessmate {

//==============================================================================
ChessMateEngine::ChessMateEngine() : Runner("ChessMateEngine", 0)
{
    fly::System::SetupSignalHandler();

    const std::string temp = fly::System::GetTempDirectory();
    m_chessMateDirectory = fly::System::Join(temp, "ChessMate");

    if (!fly::System::MakeDirectory(m_chessMateDirectory))
    {
        m_chessMateDirectory = temp;
    }
}

//==============================================================================
bool ChessMateEngine::StartRunner()
{
    bool ret = (
        initConfigManager() &&
        initLogger() &&
        initSocketManager() &&
        initGameManager()
    );

    if (!ret)
    {
        fly::System::CleanExit(fly::InitFailed);
    }

    return ret;
}

//==============================================================================
void ChessMateEngine::StopRunner()
{
    stopRunner(m_spGameManager);
    stopRunner(m_spSocketManager);
    stopRunner(m_spLogger);
    stopRunner(m_spConfigManager);
}

//==============================================================================
bool ChessMateEngine::DoWork()
{
    return false;
}

//==============================================================================
bool ChessMateEngine::initConfigManager()
{
    m_spConfigManager = std::make_shared<fly::ConfigManager>(
        fly::ConfigManager::CONFIG_TYPE_INI,
        m_chessMateDirectory, "ChessMate.ini"
    );

    return m_spConfigManager->Start();
}

//==============================================================================
bool ChessMateEngine::initLogger()
{
    m_spLogger = std::make_shared<fly::Logger>(
        m_spConfigManager, m_chessMateDirectory
    );

    if (m_spLogger->Start())
    {
        fly::Logger::SetInstance(m_spLogger);
    }
    else
    {
        LOGC("Could not start logger, using console instead");
        m_spLogger.reset();
    }

    return true;
}

//==============================================================================
bool ChessMateEngine::initSocketManager()
{
    m_spSocketManager = std::make_shared<fly::SocketManagerImpl>(
        m_spConfigManager
    );

    return m_spSocketManager->Start();
}

//==============================================================================
bool ChessMateEngine::initGameManager()
{
    m_spGameManager = std::make_shared<GameManager>(
        m_spConfigManager, m_spSocketManager
    );

    return m_spGameManager->Start();
}

}

//==============================================================================
int main()
{
    chessmate::ChessMateEnginePtr spEngine(
        std::make_shared<chessmate::ChessMateEngine>()
    );

    if (spEngine && spEngine->Start())
    {
        while (fly::System::KeepRunning())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        spEngine->Stop();
    }

    return fly::System::GetExitCode();
}
