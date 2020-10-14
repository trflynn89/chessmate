#include "chessmate.h"

#include "fly/logger/log.hpp"
#include "game/game_config.h"
#include "game/game_manager.h"

#include <fly/coders/coder_config.hpp>
#include <fly/config/config_manager.hpp>
#include <fly/logger/logger.hpp>
#include <fly/logger/logger_config.hpp>
#include <fly/socket/socket_config.hpp>
#include <fly/socket/socket_manager.hpp>
#include <fly/system/system.hpp>
#include <fly/task/task_manager.hpp>
#include <fly/task/task_runner.hpp>

#include <atomic>
#include <chrono>
#include <csignal>
#include <functional>
#include <thread>

namespace chessmate {

//==================================================================================================
ChessMateEngine::ChessMateEngine() :
    m_chessMateDirectory(std::filesystem::temp_directory_path() / "ChessMate")
{
    if (!std::filesystem::exists(m_chessMateDirectory) &&
        !std::filesystem::create_directories(m_chessMateDirectory))
    {
        LOGS("Could not create directory: %s", m_chessMateDirectory);
        m_chessMateDirectory = std::filesystem::temp_directory_path();
    }
}

//==================================================================================================
bool ChessMateEngine::Start()
{
    fly::System::set_signal_handler([this](int signal) {
        m_killSignal = signal;
        m_keepRunning = false;
    });

    if (initTaskManager() && initConfigManager() && initLoggers() && initSocketManager() &&
        initGameManager())
    {
        m_keepRunning = true;
        return true;
    }

    return false;
}

//==================================================================================================
void ChessMateEngine::Stop()
{
    if (m_spGameManager)
    {
        m_spGameManager->Stop();
    }
    if (m_spTaskManager)
    {
        m_spTaskManager->stop();
    }

    fly::System::set_signal_handler(nullptr);
}

//==================================================================================================
bool ChessMateEngine::KeepRunning(int &signal) const
{
    if (m_keepRunning.load())
    {
        return true;
    }

    signal = m_killSignal.load();
    return false;
}

//==================================================================================================
bool ChessMateEngine::initTaskManager()
{
    m_spTaskManager = std::make_shared<fly::TaskManager>(4);
    return m_spTaskManager->start();
}

//==================================================================================================
bool ChessMateEngine::initConfigManager()
{
    auto task_runner = m_spTaskManager->create_task_runner<fly::SequencedTaskRunner>();

    m_spConfigManager = std::make_shared<fly::ConfigManager>(
        task_runner,
        fly::ConfigManager::ConfigFileType::Json,
        m_chessMateDirectory / "chessmate.json");

    return m_spConfigManager->start();
}

//==================================================================================================
bool ChessMateEngine::initLoggers()
{
    auto task_runner = m_spTaskManager->create_task_runner<fly::SequencedTaskRunner>();
    auto logger_config = m_spConfigManager->create_config<fly::LoggerConfig>();
    auto coder_config = m_spConfigManager->create_config<fly::CoderConfig>();

    m_spConsoleLogger = fly::Logger::create_console_logger("console", task_runner, logger_config);

    m_spFileLogger = fly::Logger::create_file_logger(
        "chessmate",
        task_runner,
        logger_config,
        coder_config,
        m_chessMateDirectory);

    if (m_spFileLogger)
    {
        fly::Logger::set_default_logger(m_spFileLogger);
    }
    else
    {
        LOGW("Could not start file logger, using default logger instead");
    }

    return true;
}

//==================================================================================================
bool ChessMateEngine::initSocketManager()
{
    auto task_runner = m_spTaskManager->create_task_runner<fly::SequencedTaskRunner>();
    auto socket_config = m_spConfigManager->create_config<fly::SocketConfig>();

    m_spSocketManager = std::make_shared<fly::SocketManagerImpl>(task_runner, socket_config);
    m_spSocketManager->start();

    return true;
}

//==================================================================================================
bool ChessMateEngine::initGameManager()
{
    auto task_runner = m_spTaskManager->create_task_runner<fly::ParallelTaskRunner>();
    auto game_config = m_spConfigManager->create_config<GameConfig>();

    m_spGameManager = std::make_shared<GameManager>(task_runner, m_spSocketManager, game_config);
    return m_spGameManager->Start();
}

} // namespace chessmate

//==================================================================================================
int main()
{
    chessmate::ChessMateEngine engine;
    int signal = 0;

    if (engine.Start())
    {
        while (engine.KeepRunning(signal))
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        fly::Logger::get("console")->info("Received terminal signal %d, quitting", signal);
        engine.Stop();
    }
    else
    {
        if (fly::Logger::get("console"))
        {
            fly::Logger::get("console")->error("Failed initialization");
        }

        return -1;
    }

    return ((signal == SIGINT) || (signal == SIGTERM)) ? 0 : signal;
}
