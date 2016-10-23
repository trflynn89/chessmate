#include <fly/exit_codes.h>
#include <fly/config/config_manager.h>
#include <fly/logging/logger.h>
#include <fly/socket/socket_manager_impl.h>
#include <fly/system/system.h>

#include <chessmate.h>
#include <game/game_manager.h>

namespace chessmate {

//==============================================================================
ChessMate::ChessMate(bool isServer) :
    Runner("ChessMate", 0),
    m_isServer(isServer)
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
ChessMate::~ChessMate()
{
    Stop();
}

//==============================================================================
fly::ConfigManagerPtr ChessMate::GetConfigManager() const
{
    return m_spConfigManager;
}

//==============================================================================
fly::LoggerPtr ChessMate::GetLogger() const
{
    return m_spLogger;
}

//==============================================================================
fly::SocketManagerPtr ChessMate::GetSocketManager() const
{
    return m_spSocketManager;
}

//==============================================================================
GameManagerPtr ChessMate::GetGameManager() const
{
    return m_spGameManager;
}

//==============================================================================
bool ChessMate::StartRunner()
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
void ChessMate::StopRunner()
{
    stopRunner(m_spGameManager);
    stopRunner(m_spSocketManager);
    stopRunner(m_spLogger);
    stopRunner(m_spConfigManager);
}

//==============================================================================
bool ChessMate::DoWork()
{
    return false;
}

//==============================================================================
bool ChessMate::initConfigManager()
{
    m_spConfigManager = std::make_shared<fly::ConfigManager>(
        fly::ConfigManager::CONFIG_TYPE_INI,
        m_chessMateDirectory, "ChessMate.ini"
    );

    return m_spConfigManager->Start();
}

//==============================================================================
bool ChessMate::initLogger()
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
bool ChessMate::initSocketManager()
{
    m_spSocketManager = std::make_shared<fly::SocketManagerImpl>(
        m_spConfigManager
    );

    return m_spSocketManager->Start();
}

//==============================================================================
bool ChessMate::initGameManager()
{
    m_spGameManager = GameManager::Create(
        m_spConfigManager, m_spSocketManager, m_isServer
    );

    return m_spGameManager->Start();
}

}
