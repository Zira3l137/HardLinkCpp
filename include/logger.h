#pragma once

// ANSI color codes
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLDBLACK "\033[90m"   /* Black */
#define BOLDRED "\033[91m"     /* Red */
#define BOLDGREEN "\033[92m"   /* Green */
#define BOLDYELLOW "\033[93m"  /* Yellow */
#define BOLDBLUE "\033[94m"    /* Blue */
#define BOLDMAGENTA "\033[95m" /* Magenta */
#define BOLDCYAN "\033[96m"    /* Cyan */
#define BOLDWHITE "\033[97m"   /* White */

#include <iostream>

namespace logger {

enum LogLevel { DEBUG, INFO, WARN, ERROR, NONE };

class Logger {

  public:
    static Logger &getInstance();

    void setLevel(LogLevel level);

    void debug(const char *file, const int line, const char *func,
               const std::string &message);
    void info(const char *file, const int line, const char *func,
              const std::string &message);
    void warn(const char *file, const int line, const char *func,
              const std::string &message);
    void error(const char *file, const int line, const char *func,
               const std::string &message);

    void debug(const char *file, const int line, const char *func,
               const char *message);
    void info(const char *file, const int line, const char *func,
              const char *message);
    void warn(const char *file, const int line, const char *func,
              const char *message);
    void error(const char *file, const int line, const char *func,
               const char *message);

  private:
    LogLevel level;

    Logger(const Logger &) = delete;            // Disable copy constructor
    Logger &operator=(const Logger &) = delete; // Disable copy assignment

    static Logger *instance; // Singleton
    Logger() : level(logger::LogLevel::WARN) {}
};

#define LOGGER_SET(level) logger::Logger::getInstance().setLevel(level)

#define LOG_DEBUG(message)                                                     \
    logger::Logger::getInstance().debug(__FILE__, __LINE__, __func__, message)

#define LOG_INFO(message)                                                      \
    logger::Logger::getInstance().info(__FILE__, __LINE__, __func__, message)

#define LOG_WARN(message)                                                      \
    logger::Logger::getInstance().warn(__FILE__, __LINE__, __func__, message)

#define LOG_ERROR(message)                                                     \
    logger::Logger::getInstance().error(__FILE__, __LINE__, __func__, message)

} // namespace logger
