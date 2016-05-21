#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cwchar>
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
Logger::Logger(const std::string &filePath, size_t maxFileSize) :
    m_filePath(filePath),
    m_maxFileSize(maxFileSize),
    m_fileSize(0),
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
bool Logger::StartLogger()
{
    if (System::MakeDirectory(m_filePath) && createLogFile())
    {
        const LoggerPtr &spThis = shared_from_this();
        auto function = &Logger::ioThread;

        m_aKeepRunning.store(true);
        m_future = std::async(std::launch::async, function, spThis);
    }

    return m_future.valid();
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
    std::string timeStr = System::LocalTime();

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

    if (spLogger && spLogger->m_aKeepRunning.load())
    {
        spLogger->addLog(level, gameId, file, func, line, message);
    }
    else
    {
        std::string console = String::Format("%d %d %s:%s:%d %s",
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
bool Logger::createLogFile()
{
    std::string randStr = String::GenerateRandomString(10);
    std::string timeStr = System::LocalTime();

    String::ReplaceAll(timeStr, ":", "-");
    String::ReplaceAll(timeStr, " ", "_");

    std::string fileName = String::Format("Log_%s_%s.log", timeStr, randStr);
    fileName = String::Join(System::GetSeparator(), m_filePath, fileName);

    if (m_logFile.is_open())
    {
        m_logFile.close();
    }

    LOGC("Creating logger file: %s", fileName);
    m_logFile.open(fileName, std::ios::out);
    m_fileSize = 0;

    return m_logFile.good();
}

//=============================================================================
void Logger::ioThread()
{
    unsigned long long int index = U64(0);

    while (m_aKeepRunning.load() && m_logFile.good())
    {
        Log log;

        if (m_logQueue.Pop(log, s_queueWaitTime) && m_logFile.good())
        {
            const std::string logStr = String::Format("%u\t%s", index++, log);

            m_logFile << logStr << std::flush;
            m_fileSize += logStr.size();

            if (m_fileSize > m_maxFileSize)
            {
                createLogFile();
            }
        }
    }

    // Clear in case log file was in a bad state before StopLogger() was called
    m_aKeepRunning.store(false);
}

}
