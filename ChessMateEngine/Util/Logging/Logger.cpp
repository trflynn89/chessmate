#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <fstream>
#include <limits.h>

#include "Logger.h"

#include <Util/String/String.h>
#include <Util/System/System.h>

namespace Util {

namespace
{
    template <typename T>
    unsigned int increment(std::atomic<T> &value, const T &min, const T &max)
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
std::mutex Logger::s_consoleMutex;

//=============================================================================
Logger::Logger() :
    m_maxIndex((MAX_LOG_SIZE * 1024 * 1024) / sizeof(Log)),

    m_logBuffer(m_maxIndex),
    m_logIndex(0),

    m_flushingLog(false),

    m_maxDebugIndex(static_cast<const unsigned int>(m_maxIndex * 0.5)),
    m_maxInfoIndex(static_cast<const unsigned int>(m_maxIndex * 0.8)),
    m_maxWarningIndex(static_cast<const unsigned int>(m_maxIndex * 0.95)),
    m_maxErrorIndex(m_maxIndex),

    m_debugIndex(0),
    m_infoIndex(m_maxDebugIndex),
    m_warningIndex(m_maxInfoIndex),
    m_errorIndex(m_maxWarningIndex),

    m_startTime(std::chrono::high_resolution_clock::now())
{
}

//=============================================================================
Logger::~Logger()
{
    // TODO implement flush on desctruction option
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
void Logger::ConsoleLog(const std::string &message)
{
    std::string timeStr = Util::System::LocalTime();

    std::lock_guard<std::mutex> lock(s_consoleMutex);
    std::cout << timeStr << ": " << message << std::endl;
}

//=============================================================================
void Logger::AddLog(LogLevel level, int gameId, const char *func,
    unsigned int line, const std::string &message)
{
    LoggerPtr spLogger = GetInstance();

    if (spLogger)
    {
        spLogger->addLog(level, gameId, func, line, message);
    }
    else
    {
        std::string console = Util::String::Format("%d %d %s:%d %s",
            level, gameId, func, line, message);

        ConsoleLog(console);
    }
}

//=============================================================================
void Logger::addLog(LogLevel level, int gameId, const char *func,
    unsigned int line, const std::string &message)
{
    // Disallow logging while the log is being flushed
    // TODO come up with a better way to handle this case
    if (m_flushingLog.load())
    {
        return;
    }

    auto now = std::chrono::high_resolution_clock::now();
    auto logTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - m_startTime);

    unsigned int index = 0;

    switch (level)
    {
    case LOG_DEBUG:
        index = increment(m_debugIndex, 0U, m_maxDebugIndex);
        break;

    case LOG_INFO:
        index = increment(m_infoIndex, m_maxDebugIndex, m_maxInfoIndex);
        break;

    case LOG_WARN:
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
    snprintf(logToEdit->m_message, sizeof(logToEdit->m_message), message.c_str());
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

    std::string randStr = String::String::GenerateRandomString(10);
    std::string timeStr = Util::System::LocalTime();

    String::String::ReplaceAll(timeStr, ":", "-");
    String::String::ReplaceAll(timeStr, " ", "_");

    std::string fileName = "Log_" + timeStr + "_" + randStr + ".log";

    std::ofstream file;
    file.open(fileName.c_str(), std::ios::out);

    unsigned int ptr = 0;

    while ((logCopy[ptr].m_level < NUM_LEVELS) && (ptr < m_maxErrorIndex))
    {
        file << logCopy[ptr++];
    }

    file << std::flush;
}

}
