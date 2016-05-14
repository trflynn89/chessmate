#pragma once

#include <atomic>
#include <chrono>
#include <future>
#include <memory>
#include <mutex>
#include <stdarg.h>
#include <string>
#include <vector>

#include <Util/Utilities.h>
#include <Util/Logging/Log.h>
#include <Util/String/String.h>

//=============================================================================
#define LOG(lvl, gameId, fmt)                                                 \
(                                                                             \
    Util::Logger::AddLog(lvl, gameId, __FILE__, __FUNCTION__, __LINE__, fmt)  \
)

//=============================================================================
#define LOGD(gameId, fmt, ...)                                                \
(                                                                             \
    LOG(LOG_DEBUG, gameId, Util::String::Format(fmt, ##__VA_ARGS__))          \
)

//=============================================================================
#define LOGI(gameId, fmt, ...)                                                \
(                                                                             \
    LOG(LOG_INFO, gameId, Util::String::Format(fmt, ##__VA_ARGS__))           \
)

//=============================================================================
#define LOGW(gameId, fmt, ...)                                                \
(                                                                             \
    LOG(LOG_WARN, gameId, Util::String::Format(fmt, ##__VA_ARGS__))           \
)

//=============================================================================
#define LOGE(gameId, fmt, ...)                                                \
(                                                                             \
    LOG(LOG_ERROR, gameId, Util::String::Format(fmt, ##__VA_ARGS__))          \
)

//=============================================================================
#define LOGC(fmt, ...)                                                        \
(                                                                             \
    Util::Logger::ConsoleLog(true, Util::String::Format(fmt, ##__VA_ARGS__))  \
)

//=============================================================================
#define LOGC_NO_LOCK(fmt, ...)                                                \
(                                                                             \
    Util::Logger::ConsoleLog(false, Util::String::Format(fmt, ##__VA_ARGS__)) \
)

// Max log size (MB) to store in memory. Default 256MB.
#ifndef MAX_LOG_SIZE
    #define MAX_LOG_SIZE 256
#endif

namespace Util {

DEFINE_CLASS_PTRS(Logger);

/**
 * Provides in-memory, thread safe instrumentation, with the ability to be
 * flushed to a randomly generated file. There are 4 levels of instrumentation:
 * 1. Debug = Really common points.
 * 2. Info = Less common, event based point (e.g. client connection, game over)
 * 3. Warning = Something went wrong, but the system is OK.
 * 4. Error = Something went wrong, and the sytem is not OK.
 *
 * The amount of memory allocated to the instrumentation buffer is configurable
 * via the macro MAX_LOG_SIZE, in megabytes. Default is 256 MB. The buffer is
 * split into 4 sections, for each instrumentation level. Debug points hold 50%
 * of the buffer, info points hold 30%, warning points hold 15%, and error
 * points hold 5%.
 *
 * The amount of memory allocated per instrumentation point is configurable via
 * the macro MAX_MESSAGE_SIZE, in bytes. Default is 256 bytes.
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
 * @version February 3, 2016
 */
class Logger : public std::enable_shared_from_this<Logger>
{
public:
    Logger();

    ~Logger();

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

    /**
     * Flush the log at a later time.
     */
    void FlushLater();

    /**
     * Flush the log to a file who's name is randomly generated.
     */
    void Flush();

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

    static LoggerWPtr s_wpInstance;
    static std::mutex s_consoleMutex;

    const unsigned int m_maxIndex;

    std::vector<Log> m_logBuffer;
    std::atomic_ullong m_logIndex;

    std::vector<std::future<void>> m_futures;
    std::atomic_bool m_flushingLog;
    std::atomic_bool m_flushLog;

    const unsigned int m_maxDebugIndex;
    const unsigned int m_maxInfoIndex;
    const unsigned int m_maxWarningIndex;
    const unsigned int m_maxErrorIndex;

    std::atomic<unsigned int> m_debugIndex;
    std::atomic<unsigned int> m_infoIndex;
    std::atomic<unsigned int> m_warningIndex;
    std::atomic<unsigned int> m_errorIndex;

    std::chrono::high_resolution_clock::time_point m_startTime;
};

}
