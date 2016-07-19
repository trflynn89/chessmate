#pragma once

#include <atomic>
#include <chrono>
#include <fstream>
#include <future>
#include <memory>
#include <mutex>
#include <stdarg.h>
#include <string>
#include <vector>

#include <Util/Utilities.h>
#include <Util/Concurrency/ConcurrentQueue.h>
#include <Util/Config/ConfigManager.h>
#include <Util/Logging/Log.h>
#include <Util/Logging/LoggerConfig.h>
#include <Util/String/String.h>

//==============================================================================
#define LOG(lvl, gameId, fmt)                                                 \
(                                                                             \
    Util::Logger::AddLog(lvl, gameId, __FILE__, __FUNCTION__, __LINE__, fmt)  \
)

//==============================================================================
#define LOGD(gameId, fmt, ...)                                                \
(                                                                             \
    LOG(LOG_DEBUG, gameId, Util::String::Format(fmt, ##__VA_ARGS__))          \
)

//==============================================================================
#define LOGI(gameId, fmt, ...)                                                \
(                                                                             \
    LOG(LOG_INFO, gameId, Util::String::Format(fmt, ##__VA_ARGS__))           \
)

//==============================================================================
#define LOGW(gameId, fmt, ...)                                                \
(                                                                             \
    LOG(LOG_WARN, gameId, Util::String::Format(fmt, ##__VA_ARGS__))           \
)

//==============================================================================
#define LOGE(gameId, fmt, ...)                                                \
(                                                                             \
    LOG(LOG_ERROR, gameId, Util::String::Format(fmt, ##__VA_ARGS__))          \
)

//==============================================================================
#define LOGC(fmt, ...)                                                        \
(                                                                             \
    Util::Logger::ConsoleLog(true, Util::String::Format(fmt, ##__VA_ARGS__))  \
)

//==============================================================================
#define LOGC_NO_LOCK(fmt, ...)                                                \
(                                                                             \
    Util::Logger::ConsoleLog(false, Util::String::Format(fmt, ##__VA_ARGS__)) \
)

namespace Util {

DEFINE_CLASS_PTRS(Logger);

/**
 * Provides thread safe instrumentation. There are 4 levels of instrumentation:
 * 1. Debug = Really common points.
 * 2. Info = Less common, event based point (e.g. client connection, game over)
 * 3. Warning = Something went wrong, but the system is OK.
 * 4. Error = Something went wrong, and the sytem is not OK.
 *
 * The following macros should be used to add points to the log: LOGD, LOGI,
 * LOGW, LOGE. Usage is as follows:
 *
 *   LOGD(game ID, message, message arguments)
 *   For example, LOGD(1, "This is message number %d", 10)
 *
 * The LOGC macro is also provided for thread-safe console logging. LOGC_NO_LOCK
 * is also provided for console logging without acquiring the console lock while
 * inside, e.g., a signal handler.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 18, 2016
 */
class Logger : public std::enable_shared_from_this<Logger>
{
public:
    /**
     * Constructor.
     *
     * @param ConfigManagerPtr Reference to the configuration manager.
     * @param string Path to store the log file.
     */
    Logger(ConfigManagerPtr &, const std::string &);

    /**
     * Destructor - stop the logger if necessary.
     */
    ~Logger();

    /**
     * Start the logger. Create a thread to perform all IO operations.
     *
     * @return True if the logger was started successfully.
     */
    bool StartLogger();

    /**
     * Stop the logger. Signal for the IO thread to stop.
     */
    void StopLogger();

    /**
     * Set the logger instance so that the LOG* macros function.
     *
     * @param LoggerPtr The logger instance.
     */
    static void SetInstance(const LoggerPtr &);

    /**
     * @return The logger instance.
     */
    static LoggerPtr GetInstance();

    /**
     * Log to the console in a thread-safe manner.
     *
     * @param bool Whether to acquire lock before logging.
     * @param string The message to log.
     */
    static void ConsoleLog(bool, const std::string &);

    /**
     * Add a log to the static logger instance.
     *
     * @param LogLevel The level (debug, info, etc.) of this log.
     * @param ssize_t The ID of the game storing this entry.
     * @param const char * Name of the file storing this log.
     * @param const char * Name of the function storing this log.
     * @param unsigned int The line number this log point occurs.
     * @param string The message to log.
     */
    static void AddLog(LogLevel, ssize_t, const char *, const char *, unsigned int, const std::string &);

private:
    /**
     * Add a log to this logger instance.
     *
     * @param LogLevel The level (debug, info, etc.) of this log.
     * @param ssize_t The ID of the game storing this entry.
     * @param const char * Name of the file storing this log.
     * @param const char * Name of the function storing this log.
     * @param unsigned int The line number this log point occurs.
     * @param string The message to log.
     */
    void addLog(LogLevel, ssize_t, const char *, const char *, unsigned int, const std::string &);

    /**
     * Create the log file. If a log file is already open, close it.
     *
     * @return True if the log file could be opened.
     */
    bool createLogFile();

    /**
     * Thread to perform all IO operations. Wait for a log item to be available
     * and write it to disk.
     */
    void ioThread();

    static LoggerWPtr s_wpInstance;
    static std::mutex s_consoleMutex;

    std::ofstream m_logFile;
    Util::ConcurrentQueue<Log> m_logQueue;
    std::future<void> m_future;

    LoggerConfigPtr m_spConfig;

    const std::string m_filePath;
    size_t m_fileSize;

    std::atomic_bool m_aKeepRunning;

    const std::chrono::high_resolution_clock::time_point m_startTime;
};

}
