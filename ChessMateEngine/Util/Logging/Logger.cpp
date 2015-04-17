#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <fstream>
#include <limits.h>

#include "Logger.h"

#include <Util/String/String.h>

using std::atomic;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::system_clock;
using std::ios;
using std::lock_guard;
using std::mutex;
using std::ofstream;
using std::string;

namespace Util { namespace Logging {

namespace
{
	template <typename T>
	unsigned int increment(atomic<T> &value, const T &min, const T &max)
	{
		T currValue = 0, nextValue = 0;

		auto getNextValue = [&min, &max](const T &val)
		{
			return ((val < max - 1) ? val + 1 : min);
		};

		do
		{
			currValue = value.load();
			nextValue = getNextValue(currValue);
		} while (!value.compare_exchange_weak(currValue, nextValue));

		return currValue;
	}
}

//=============================================================================
LoggerWPtr Logger::s_wpInstance;
mutex Logger::s_consoleMutex;

//=============================================================================
Logger::Logger() :
	m_maxIndex((MAX_LOG_SIZE * 1024 * 1024) / sizeof(Log)),

	m_logBuffer(m_maxIndex),
	m_logIndex(0),

	m_flushingLog(false),

	m_maxDebugIndex(m_maxIndex * 0.5),
	m_maxInfoIndex(m_maxIndex * 0.8),
	m_maxWarningIndex(m_maxIndex * 0.95),
	m_maxErrorIndex(m_maxIndex),

	m_debugIndex(0),
	m_infoIndex(m_maxDebugIndex),
	m_warningIndex(m_maxInfoIndex),
	m_errorIndex(m_maxWarningIndex),

	m_startTime(high_resolution_clock::now())
{
}

//=============================================================================
Logger::~Logger()
{
	m_logBuffer.clear();
}

//=============================================================================
void Logger::SetInstance(const LoggerPtr &spLogger)
{
	s_wpInstance = spLogger;
}

//=============================================================================
LoggerPtr Logger::GetInstance()
{
	return s_wpInstance.lock();
}

//=============================================================================
void Logger::ConsoleLog(const char *format, ...)
{
	string timeStr = GetSystemTime();

	va_list argList;
	va_start(argList, format);

	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s: %s\n", timeStr.c_str(), format);

	lock_guard<mutex> lock(s_consoleMutex);
	vfprintf(stdout, buffer, argList);

	va_end(argList);
}

//=============================================================================
void Logger::AddLog(LogLevel level, int gameId, const char *func,
	unsigned int line, const char *format, ...)
{
	LoggerPtr spLogger = GetInstance();

	if (spLogger)
	{
		va_list argList;
		va_start(argList, format);

		spLogger->addLog(level, gameId, func, line, format, argList);

		va_end(argList);
	}
}

//=============================================================================
void Logger::addLog(LogLevel level, int gameId, const char *func,
	unsigned int line, const char *format, va_list argList)
{
	// Disallow logging while the log is being flushed
	// TODO come up with a better way to handle this case
	if (m_flushingLog.load())
	{
		return;
	}

	high_resolution_clock::time_point now = high_resolution_clock::now();
	duration<double> logTime = duration_cast<duration<double>>(now - m_startTime);

	unsigned int index = 0;

	switch (level)
	{
	case LOG_DEBUG:
		index = increment(m_debugIndex, 0U, m_maxDebugIndex);
		break;

	case LOG_INFO:
		index = increment(m_infoIndex, m_maxDebugIndex, m_maxInfoIndex);
		break;

	case LOG_WARNING:
		index = increment(m_warningIndex, m_maxInfoIndex, m_maxWarningIndex);
		break;

	case LOG_ERROR:
		index = increment(m_errorIndex, m_maxWarningIndex, m_maxErrorIndex);
		break;

	default:
		return;
	}

	Log *logToEdit = &m_logBuffer[index];

	logToEdit->m_index = m_logIndex.fetch_add(1);
	logToEdit->m_level = level;
	logToEdit->m_time = logTime.count();
	logToEdit->m_gameId = gameId;
	logToEdit->m_line = line;

	snprintf(logToEdit->m_function, sizeof(logToEdit->m_function), func);
	vsnprintf(logToEdit->m_message, sizeof(logToEdit->m_message), format, argList);
}

//=============================================================================
void Logger::Flush()
{
	bool expected = false;

	// Only allow one thread to flush the log at a time
	if (!m_flushingLog.compare_exchange_strong(expected, true))
	{
		return;
	}

	// Move the contents of the log buffer to another vector
	std::vector<Log> logCopy;
	m_logBuffer.swap(logCopy);

	// Any thread can now try to add or flush logs
	m_flushingLog.store(false);

	// Sort based on log index
	auto compare = [](Log a, Log b) { return (a.m_index < b.m_index); };
	std::sort(logCopy.begin(), logCopy.end(), compare);

	string randStr = Util::String::String::GenerateRandomString(10);
	string timeStr = GetSystemTime();

	Util::String::String::ReplaceAll(timeStr, ":", "-");
	Util::String::String::ReplaceAll(timeStr, " ", "_");

	string fileName = "Log_" + timeStr + "_" + randStr + ".log";

	ofstream file;
	file.open(fileName.c_str(), ios::out);

	unsigned int ptr = 0;

	while ((logCopy[ptr].m_level < NUM_LEVELS) && (ptr < m_maxErrorIndex))
	{
		file << logCopy[ptr++];
	}

	file << std::flush;
	file.close();
}

//=============================================================================
string Logger::GetSystemTime()
{
	time_t now = system_clock::to_time_t(system_clock::now());

	char timeStr[32];
	size_t len = strftime(timeStr, sizeof(timeStr),
		"%m-%d-%Y %H:%M:%S", localtime(&now));

	if (len != 0)
	{
		return string(timeStr);
	}

	return string();
}

}}
