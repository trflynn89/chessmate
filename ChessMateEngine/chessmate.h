#include <atomic>
#include <filesystem>
#include <memory>
#include <string>
#include <type_traits>

namespace fly {
class ConfigManager;
class Logger;
class SocketManager;
class TaskManager;
} // namespace fly

namespace chessmate {

class GameManager;

/**
 * Class to initialize and manage the ChessMate engine.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 21, 2016
 */
class ChessMateEngine
{
public:
    /**
     * Constructor. Create the ChessMate directory used for logging and
     * configuration. Set up signal handlers.
     */
    ChessMateEngine();

    /**
     * Start the ChessMate engine. Initialize all subsystems.
     *
     * @retun True if each subsystem could be initialized.
     */
    bool Start();

    /**
     * Stop the ChessMate engine. Deinitialize all subsystems.
     */
    void Stop();

    bool KeepRunning(int &signal) const;

private:
    /**
     * Initialize the task subsystem.
     *
     * @retun True if it could be initialized.
     */
    bool initTaskManager();

    /**
     * Initialize the configuration subsystem.
     *
     * @retun True if it could be initialized.
     */
    bool initConfigManager();

    /**
     * Initialize the logging subsystem.
     *
     * @retun True if it could be initialized.
     */
    bool initLoggers();

    /**
     * Initialize the socket subsystem.
     *
     * @retun True if it could be initialized.
     */
    bool initSocketManager();

    /**
     * Initialize the game subsystem.
     *
     * @retun True if it could be initialized.
     */
    bool initGameManager();

    std::shared_ptr<fly::TaskManager> m_spTaskManager;
    std::shared_ptr<fly::ConfigManager> m_spConfigManager;
    std::shared_ptr<fly::Logger> m_spFileLogger;
    std::shared_ptr<fly::Logger> m_spConsoleLogger;
    std::shared_ptr<fly::SocketManager> m_spSocketManager;
    std::shared_ptr<GameManager> m_spGameManager;

    std::filesystem::path m_chessMateDirectory;

    std::atomic_bool m_keepRunning {false};
    std::atomic_int m_killSignal {0};
};

} // namespace chessmate
