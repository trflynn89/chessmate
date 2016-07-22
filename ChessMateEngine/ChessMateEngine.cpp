#include <atomic>
#include <chrono>
#include <thread>

#include <ChessMateEngine.h>
#include <Game/GameManager.h>
#include <Util/ExitCodes.h>
#include <Util/Config/ConfigManager.h>
#include <Util/Logging/Logger.h>
#include <Util/Socket/SocketManagerImpl.h>
#include <Util/System/System.h>
#include <Util/Task/Runner.h>

//==============================================================================
ChessMateEngine::ChessMateEngine() : Runner("ChessMateEngine", 0)
{
    Util::System::SetupSignalHandler();

    const char sep = Util::System::GetSeparator();
    const std::string temp = Util::System::GetTempDirectory();

    m_chessMateDirectory = Util::String::Join(sep, temp, "ChessMate");

    if (!Util::System::MakeDirectory(m_chessMateDirectory))
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
        Util::System::CleanExit(Util::InitFailed);
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
    m_spConfigManager = std::make_shared<Util::ConfigManager>(
        Util::ConfigManager::CONFIG_TYPE_INI,
        m_chessMateDirectory, "ChessMate.ini"
    );

    return m_spConfigManager->Start();
}

//==============================================================================
bool ChessMateEngine::initLogger()
{
    m_spLogger = std::make_shared<Util::Logger>(
        m_spConfigManager, m_chessMateDirectory
    );

    if (m_spLogger->Start())
    {
        Util::Logger::SetInstance(m_spLogger);
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
    m_spSocketManager = std::make_shared<Util::SocketManagerImpl>(
        m_spConfigManager
    );

    return m_spSocketManager->Start();
}

//==============================================================================
bool ChessMateEngine::initGameManager()
{
    m_spGameManager = std::make_shared<Game::GameManager>(
        m_spConfigManager, m_spSocketManager
    );

    return m_spGameManager->Start();
}

//==============================================================================
int main()
{
    ChessMateEnginePtr spEngine(std::make_shared<ChessMateEngine>());

    if (spEngine && spEngine->Start())
    {
        while (Util::System::KeepRunning())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        spEngine->Stop();
    }

    return Util::System::GetExitCode();
}
