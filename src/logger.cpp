#include "logger.h"
#include <map>
#include <string>

std::map<logger::LogLevel, const char *> LevelColors = {
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

void logger::Logger::setLevel(logger::LogLevel level) { this->level = level; }
logger::LogLevel logger::Logger::getLevel() { return this->level; }

void logger::Logger::log(const std::string &file, const int &line,
                         const std::string &func, const std::string &message) {
    std::cout << LevelColors[this->level] << "[" << this->level << "]-[" << file
              << ", at line " << line << "]->[" << func << "]: " << RESET
              << message << "\n";
}
