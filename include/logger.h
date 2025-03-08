#pragma once

#include <iostream>
#include <mutex>

namespace logger {

// ANSI color codes
constexpr const char *RESET = "\033[0m";
constexpr const char *BLACK = "\033[30m";
constexpr const char *RED = "\033[31m";
constexpr const char *GREEN = "\033[32m";
constexpr const char *YELLOW = "\033[33m";
constexpr const char *BLUE = "\033[34m";
constexpr const char *MAGENTA = "\033[35m";
constexpr const char *CYAN = "\033[36m";
constexpr const char *WHITE = "\033[37m";
constexpr const char *BOLDBLACK = "\033[90m";
constexpr const char *BOLDRED = "\033[91m";
constexpr const char *BOLDGREEN = "\033[92m";
constexpr const char *BOLDYELLOW = "\033[93m";
constexpr const char *BOLDBLUE = "\033[94m";
constexpr const char *BOLDMAGENTA = "\033[95m";
constexpr const char *BOLDCYAN = "\033[96m";
constexpr const char *BOLDWHITE = "\033[97m";

enum LogLevel { DEBUG, INFO, WARN, ERROR, NONE };

inline std::ostream &operator<<(std::ostream &os, const LogLevel &level) {
    switch (level) {
    case LogLevel::DEBUG:
        os << "DEBUG";
        break;
    case LogLevel::INFO:
        os << "INFO";
        break;
    case LogLevel::WARN:
        os << "WARN";
        break;
    case LogLevel::ERROR:
        os << "ERROR";
        break;
    case LogLevel::NONE:
        os << "NONE";
        break;
    }
    return os;
}

class Logger {
    std::mutex loggerMutex;

  public:
    static Logger &getInstance();

    void setLevel(LogLevel level);
    LogLevel getLevel();

    void log(LogLevel level, const char *file, const int &line,
             const char *func, const std::string &message);

  private:
    LogLevel level;

    Logger(const Logger &) = delete;            // Disable copy constructor
    Logger &operator=(const Logger &) = delete; // Disable copy assignment

    Logger() : level(LogLevel::WARN) {}
};

#define LOGGER_SET(level) logger::Logger::getInstance().setLevel(level)

#define LOG_DEBUG(message)                                                     \
    if (logger::Logger::getInstance().getLevel() <= logger::LogLevel::DEBUG) { \
        logger::Logger::getInstance().log(logger::LogLevel::DEBUG, __FILE__,   \
                                          __LINE__, __func__, message);        \
    }

#define LOG_INFO(message)                                                      \
    if (logger::Logger::getInstance().getLevel() <= logger::LogLevel::INFO) {  \
        logger::Logger::getInstance().log(logger::LogLevel::INFO, __FILE__,    \
                                          __LINE__, __func__, message);        \
    }

#define LOG_WARN(message)                                                      \
    if (logger::Logger::getInstance().getLevel() <= logger::LogLevel::WARN) {  \
        logger::Logger::getInstance().log(logger::LogLevel::WARN, __FILE__,    \
                                          __LINE__, __func__, message);        \
    }

#define LOG_ERROR(message)                                                     \
    if (logger::Logger::getInstance().getLevel() <= logger::LogLevel::ERROR) { \
        logger::Logger::getInstance().log(logger::LogLevel::ERROR, __FILE__,   \
                                          __LINE__, __func__, message);        \
    }

} // namespace logger
