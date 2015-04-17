#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <stdarg.h>
#include <string>
#include <vector>

#include <Util/Utilities.h>
#include <Util/Logging/Log.h>

//=============================================================================
#define LOGD(gameId, format, ...)                                             \
(                                                                             \
	Util::Logging::Logger::AddLog                                             \
	(                                                                         \
		LOG_DEBUG, gameId, __FUNCTION__, __LINE__, format, ##__VA_ARGS__      \
	)                                                                         \
)

//=============================================================================
#define LOGI(gameId, format, ...)                                             \
(                                                                             \
	Util::Logging::Logger::AddLog                                             \
	(                                                                         \
		LOG_INFO, gameId, __FUNCTION__, __LINE__, format, ##__VA_ARGS__       \
	)                                                                         \
)

//=============================================================================
#define LOGW(gameId, format, ...)                                             \
(                                                                             \
	Util::Logging::Logger::AddLog                                             \
	(                                                                         \
		LOG_WARNING, gameId, __FUNCTION__, __LINE__, format, ##__VA_ARGS__    \
	)                                                                         \
)

//=============================================================================
#define LOGE(gameId, format, ...)                                             \
(                                                                             \
	Util::Logging::Logger::AddLog                                             \
	(                                                                         \
		LOG_ERROR, gameId, __FUNCTION__, __LINE__, format, ##__VA_ARGS__      \
	)                                                                         \
)

//=============================================================================
#define LOGC(format, ...)                                                     \
(                                                                             \
	Util::Logging::Logger::ConsoleLog(format, ##__VA_ARGS__)                  \
)

// Max log size (MB) to store in memory. Default 256MB.
#ifndef MAX_LOG_SIZE
	#define MAX_LOG_SIZE 256
#endif

namespace Util { namespace Logging {

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
 * The LOGC macro is also provided for thread-safe console logging.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version December 19, 2012
 */
class Logger
{
public:
	Logger();

	~Logger();

	static void SetInstance(const LoggerPtr &);
	static LoggerPtr GetInstance();

	/**
	 * Log to the console in a thread-safe manner.
	 *
	 * @param const char * The variadic message to store.
	 */
	static void ConsoleLog(const char *, ...);

	/**
	 * Add a log to the logging system.
	 *
	 * @param LogLevel The level (debug, info, etc.) of this log.
	 * @param int The ID of the game storing this entry.
	 * @param const char * Name of the function storing this log.
	 * @param unsigned int The line number this log point occurs.
	 * @param const char * The variadic message to store.
	 */
	static void AddLog(LogLevel, int, const char *, unsigned int, const char *, ...);

	/**
	 * Flush the log to a file who's name is randomly generated.
	 */
	void Flush();

private:
	static std::string GetSystemTime();

	void addLog(LogLevel, int, const char *, unsigned int, const char *, va_list);

	static LoggerWPtr s_wpInstance;
	static std::mutex s_consoleMutex;

	const unsigned int m_maxIndex;

	std::vector<Log> m_logBuffer;
	std::atomic_ullong m_logIndex;

	std::atomic_bool m_flushingLog;

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

}}
