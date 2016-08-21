#include <memory>
#include <string>
#include <type_traits>

#include <fly/fly.h>
#include <fly/task/runner.h>

namespace fly {

DEFINE_CLASS_PTRS(ConfigManager);
DEFINE_CLASS_PTRS(Logger);
DEFINE_CLASS_PTRS(SocketManager);

}

namespace chessmate {

DEFINE_CLASS_PTRS(ChessMateEngine);
DEFINE_CLASS_PTRS(GameManager);

/**
 * Class to initialize and manage the ChessMate engine.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 21, 2016
 */
class ChessMateEngine : public fly::Runner
{
public:
    /**
     * Constructor. Create the ChessMate directory used for logging and
     * configuration. Set up signal handlers.
     */
    ChessMateEngine();

protected:
    /**
     * Start the ChessMate engine. Initialize all subsystems.
     *
     * @retun True if each subsystem could be initialized.
     */
    virtual bool StartRunner();

    /**
     * Stop the ChessMate engine. Deinitialize all subsystems.
     */
    virtual void StopRunner();

    /**
     * @return False - no workers are used, thus this should not be called.
     */
    virtual bool DoWork();

private:
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
    bool initLogger();

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

    /**
     * Deinitialize a running subsystem.
     */
    template <typename T>
    void stopRunner(std::shared_ptr<T> &);

    fly::ConfigManagerPtr m_spConfigManager;
    fly::LoggerPtr m_spLogger;
    fly::SocketManagerPtr m_spSocketManager;
    GameManagerPtr m_spGameManager;

    std::string m_chessMateDirectory;
};

//==============================================================================
template <typename T>
void ChessMateEngine::stopRunner(std::shared_ptr<T> &spRunner)
{
    static_assert(std::is_base_of<Runner, T>::value,
        "Given type is not a runnable type");

    if (spRunner)
    {
        spRunner->Stop();
    }
}

}
