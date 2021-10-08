#include "chessmate.h"

#include "game/game_config.h"
#include "game/game_manager.h"

#include <fly/coders/coder_config.hpp>
#include <fly/config/config_manager.hpp>
#include <fly/logger/logger.hpp>
#include <fly/logger/logger_config.hpp>
#include <fly/net/network_config.hpp>
#include <fly/net/socket/socket_service.hpp>
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
        LOGS("Could not create directory: {}", m_chessMateDirectory.string());
        m_chessMateDirectory = std::filesystem::temp_directory_path();
    }
}

//==================================================================================================
bool ChessMateEngine::Start()
{
    fly::system::set_signal_handler(
        [this](int signal)
        {
            m_killSignal = signal;
            m_keepRunning = false;
        });

    if (initTaskManager() && initConfigManager() && initLoggers() && initSocketService() &&
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

    fly::system::set_signal_handler(nullptr);
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
    m_spTaskManager = fly::task::TaskManager::create(std::thread::hardware_concurrency() * 2);
    return static_cast<bool>(m_spTaskManager);
}

//==================================================================================================
bool ChessMateEngine::initConfigManager()
{
    auto task_runner = fly::task::SequencedTaskRunner::create(m_spTaskManager);

    m_spConfigManager = fly::config::ConfigManager::create(
        task_runner,
        fly::config::ConfigFileType::Json,
        m_chessMateDirectory / "chessmate.json");

    return static_cast<bool>(m_spConfigManager);
}

//==================================================================================================
bool ChessMateEngine::initLoggers()
{
    auto task_runner = fly::task::SequencedTaskRunner::create(m_spTaskManager);
    auto logger_config = m_spConfigManager->create_config<fly::logger::LoggerConfig>();
    auto coder_config = m_spConfigManager->create_config<fly::coders::CoderConfig>();

    m_spConsoleLogger = fly::logger::create_console_logger("console", task_runner, logger_config);

    m_spFileLogger = fly::logger::create_file_logger(
        "chessmate",
        task_runner,
        logger_config,
        coder_config,
        m_chessMateDirectory);

    if (m_spFileLogger)
    {
        fly::logger::Logger::set_default_logger(m_spFileLogger);
    }
    else
    {
        LOGW("Could not start file logger, using default logger instead");
    }

    return true;
}

//==================================================================================================
bool ChessMateEngine::initSocketService()
{
    auto task_runner = fly::task::SequencedTaskRunner::create(m_spTaskManager);
    auto network_config = m_spConfigManager->create_config<fly::net::NetworkConfig>();

    m_spSocketService =
        fly::net::SocketService::create(std::move(task_runner), std::move(network_config));
    return static_cast<bool>(m_spSocketService);
}

//==================================================================================================
bool ChessMateEngine::initGameManager()
{
    auto task_runner = fly::task::ParallelTaskRunner::create(m_spTaskManager);
    auto game_config = m_spConfigManager->create_config<GameConfig>();

    m_spGameManager = std::make_shared<GameManager>(task_runner, m_spSocketService, game_config);
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

        fly::logger::Logger::get("console")->info("Received terminal signal {}, quitting", signal);
        engine.Stop();
    }
    else
    {
        if (fly::logger::Logger::get("console"))
        {
            fly::logger::Logger::get("console")->error("Failed initialization");
        }

        return -1;
    }

    return ((signal == SIGINT) || (signal == SIGTERM)) ? 0 : signal;
}
