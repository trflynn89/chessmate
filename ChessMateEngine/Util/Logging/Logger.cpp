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
    static const std::chrono::seconds s_queueWaitTime(1);
}

//=============================================================================
LoggerWPtr Logger::s_wpInstance;
std::mutex Logger::s_consoleMutex;

//=============================================================================
Logger::Logger() :
    m_aLogIndex(0U),
    m_aKeepRunning(false),
    m_startTime(std::chrono::high_resolution_clock::now())
{
}

//=============================================================================
Logger::~Logger()
{
    StopLogger();
}

//=============================================================================
void Logger::StartLogger()
{
    const LoggerPtr &spThis = shared_from_this();
    auto function = &Logger::ioThread;

    m_aKeepRunning.store(true);
    m_future = std::async(std::launch::async, function, spThis);
}

//=============================================================================
void Logger::StopLogger()
{
    LOGC("Stopping logger");
    bool expected = true;

    if (m_aKeepRunning.compare_exchange_strong(expected, false))
    {
        if (m_future.valid())
        {
            m_future.get();
        }
    }
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
void Logger::ConsoleLog(bool acquireLock, const std::string &message)
{
    std::unique_lock<std::mutex> lock(s_consoleMutex, std::defer_lock);
    std::string timeStr = Util::System::LocalTime();

    if (acquireLock)
    {
        lock.lock();
    }

    std::cout << timeStr << ": " << message << std::endl;
}

//=============================================================================
void Logger::AddLog(LogLevel level, ssize_t gameId, const char *file,
    const char *func, unsigned int line, const std::string &message)
{
    LoggerPtr spLogger = GetInstance();

    if (spLogger)
    {
        spLogger->addLog(level, gameId, file, func, line, message);
    }
    else
    {
        std::string console = Util::String::Format("%d %d %s:%s:%d %s",
            level, gameId, file, func, line, message);

        ConsoleLog(true, console);
    }
}

//=============================================================================
void Logger::addLog(LogLevel level, ssize_t gameId, const char *file,
    const char *func, unsigned int line, const std::string &message)
{
    auto now = std::chrono::high_resolution_clock::now();
    auto logTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - m_startTime);

    if ((level >= LOG_DEBUG) && (level < NUM_LEVELS))
    {
        Log log;

        log.m_index = m_aLogIndex.fetch_add(1);
        log.m_level = level;
        log.m_time = logTime.count();
        log.m_gameId = gameId;
        log.m_line = line;

        snprintf(log.m_file, sizeof(log.m_file), "%s", file);
        snprintf(log.m_function, sizeof(log.m_function), "%s", func);
        snprintf(log.m_message, sizeof(log.m_message), "%s", message.c_str());

        m_logQueue.Push(log);
    }
}

//=============================================================================
void Logger::ioThread()
{
    std::string randStr = String::String::GenerateRandomString(10);
    std::string timeStr = Util::System::LocalTime();

    String::String::ReplaceAll(timeStr, ":", "-");
    String::String::ReplaceAll(timeStr, " ", "_");

    std::string fileName = "Log_" + timeStr + "_" + randStr + ".log";
    std::ofstream file(fileName, std::ios::out);

    while (m_aKeepRunning.load() && file.good())
    {
        Log log;

        if (m_logQueue.Pop(log, s_queueWaitTime) && file.good())
        {
            file << log << std::flush;
        }
    }
}

}
