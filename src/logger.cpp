#include "logger.h"
#include <string>
#include <unordered_map>

std::unordered_map<logger::LogLevel, const char *> LevelColors = {
    {logger::LogLevel::DEBUG, logger::BOLDCYAN},
    {logger::LogLevel::INFO, logger::BOLDGREEN},
    {logger::LogLevel::WARN, logger::BOLDYELLOW},
    {logger::LogLevel::ERROR, logger::BOLDRED},
};

logger::Logger *logger::Logger::instance = nullptr;

logger::Logger &logger::Logger::getInstance() {
    static logger::Logger instance;
    return instance;
}

void logger::Logger::setLevel(logger::LogLevel level) {
    std::lock_guard<std::mutex> lock(loggerMutex);
    this->level = level;
}

logger::LogLevel logger::Logger::getLevel() {
    std::lock_guard<std::mutex> lock(loggerMutex);
    return this->level;
}

void logger::Logger::log(const char *file, const int &line, const char *func,
                         const std::string &message) {
    std::lock_guard<std::mutex> lock(loggerMutex);
    std::cout << LevelColors[this->level] << "[" << this->level << "]-[" << file
              << ", at line " << line << "]->[" << func << "]: " << RESET
              << message << "\n";
}
