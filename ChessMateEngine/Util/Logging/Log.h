#pragma once

#include <climits>
#include <iostream>

/**
 * Enumeration to define the level of a log.
 */
enum LogLevel
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,

    NUM_LEVELS
};

// Max allowed message length (bytes) per log. Default 256B.
#ifndef MAX_MESSAGE_SIZE
    #define MAX_MESSAGE_SIZE 256
#endif

namespace Util {

/**
 * Struct to store data about single log. A log contains:
 *
 * 1. The log level.
 * 2. The time the log was made.
 * 3. The ID of the game/socket making the log.
 * 4. The file name the log is in.
 * 5. The function name the log is in.
 * 6. The line number the log is on.
 * 7. The message being logged.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version December 19, 2012
 */
struct Log
{
    LogLevel m_level = NUM_LEVELS;
    double m_time = -1.0;
    ssize_t m_gameId = -1;
    char m_file[100];
    char m_function[100];
    unsigned int m_line = -1;
    char m_message[MAX_MESSAGE_SIZE + 1];

    friend std::ostream &operator << (std::ostream &stream, const Log &log);
};

}
