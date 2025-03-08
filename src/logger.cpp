#include "logger.h"
#include <string>

logger::Logger *logger::Logger::instance = nullptr;

logger::Logger &logger::Logger::getInstance() {
    if (instance == nullptr) {
        instance = new logger::Logger();
    }
    return *instance;
}

void logger::Logger::setLevel(logger::LogLevel level) { this->level = level; }

void logger::Logger::debug(const char *file, const int line, const char *func,
                           const std::string &message) {
    if (this->level <= logger::LogLevel::DEBUG) {
        std::cout << BOLDCYAN << "[DEBUG]-[" << file << ", at line " << line
                  << "]->[" << func << "]: " << RESET << message << std::endl;
    }
}

void logger::Logger::info(const char *file, const int line, const char *func,
                          const std::string &message) {
    if (this->level <= logger::LogLevel::INFO) {
        std::cout << BOLDGREEN << "[INFO]-[" << file << ", at line " << line
                  << "]->[" << func << "]: " << RESET << message << std::endl;
    }
}

void logger::Logger::warn(const char *file, const int line, const char *func,
                          const std::string &message) {
    if (this->level <= logger::LogLevel::WARN) {
        std::cout << BOLDYELLOW << "[WARNING]-[" << file << ", at line " << line
                  << "]->[" << func << "]: " << RESET << message << std::endl;
    }
}

void logger::Logger::error(const char *file, const int line, const char *func,
                           const std::string &message) {
    if (this->level <= logger::LogLevel::ERROR) {
        std::cout << BOLDRED << "[ERROR]-[" << file << ", at line " << line
                  << "]->[" << func << "]: " << RESET << message << std::endl;
    }
}

void logger::Logger::debug(const char *file, const int line, const char *func,
                           const char *message) {
    if (this->level <= logger::LogLevel::DEBUG) {
        std::cout << BOLDCYAN << "[DEBUG]-[" << file << ", at line " << line
                  << "]->[" << func << "]: " << RESET << message << std::endl;
    }
}

void logger::Logger::info(const char *file, const int line, const char *func,
                          const char *message) {
    if (this->level <= logger::LogLevel::INFO) {
        std::cout << BOLDGREEN << "[INFO]-[" << file << ", at line " << line
                  << "]->[" << func << "]: " << RESET << message << std::endl;
    }
}

void logger::Logger::warn(const char *file, const int line, const char *func,
                          const char *message) {
    if (this->level <= logger::LogLevel::WARN) {
        std::cout << BOLDYELLOW << "[WARNING]-[" << file << ", at line " << line
                  << "]->[" << func << "]: " << RESET << message << std::endl;
    }
}

void logger::Logger::error(const char *file, const int line, const char *func,
                           const char *message) {
    if (this->level <= logger::LogLevel::ERROR) {
        std::cout << BOLDRED << "[ERROR]-[" << file << ", at line " << line
                  << "]->[" << func << "]: " << RESET << message << std::endl;
    }
}
