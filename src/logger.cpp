#include "logger.h"
#include <fstream>
#include <string>
#include <unordered_map>

std::string logger::getEnv(const std::string &name) {
    const char *value = std::getenv(name.c_str());
    if (value) {
        return value;
    }
    return "";
}

std::string logger::getTempDirectory() {
#ifdef _WIN32
    std::string temp = getEnv("TEMP");
    if (temp.empty()) {
        temp = getEnv("TMP");
    }
    if (!temp.empty()) {
        return temp;
    }
#else
    std::string temp = getEnv("TMPDIR");
    if (!temp.empty()) {
        return temp;
    }
    return "/tmp";
#endif
    return "";
}

std::unordered_map<logger::LogLevel, const char *> LevelColors = {
    {logger::LogLevel::DEBUG, logger::BOLDCYAN},
    {logger::LogLevel::INFO, logger::BOLDGREEN},
    {logger::LogLevel::WARN, logger::BOLDYELLOW},
    {logger::LogLevel::ERROR, logger::BOLDRED},
};

logger::Logger::Logger() : level(logger::LogLevel::WARN), writeToFile(false) {
    this->logFilePath =
        getTempDirectory() + PATH_SEPARATOR + __DATE__ + " linker.log";
    std::cout << this->logFilePath << std::endl;
}

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

void logger::Logger::setLogFilePath(const std::string &path) {
    std::lock_guard<std::mutex> lock(loggerMutex);
    this->logFilePath = path;
}

std::string logger::Logger::getLogFilePath() {
    std::lock_guard<std::mutex> lock(loggerMutex);
    return this->logFilePath;
}

void logger::Logger::setWriteToFile(bool writeToFile) {
    std::lock_guard<std::mutex> lock(loggerMutex);
    this->writeToFile = writeToFile;
}

void logger::Logger::log(logger::LogLevel level, const char *file,
                         const int &line, const char *func,
                         const std::string &message) {
    std::lock_guard<std::mutex> lock(loggerMutex);

    if (this->writeToFile && this->logFilePath != "") {
        std::ofstream log(this->logFilePath, std::ios_base::app);
        log << "[" << level << "]-[" << __TIME__ << "]-[" << file
            << ", at line " << line << "]->[" << func << "]: " << message
            << "\n";
        log.close();
    }

    std::cout << LevelColors[level] << "[" << level << "]-[" << __TIME__
              << "]-[" << file << ", at line " << line << "]->[" << func
              << "]: " << logger::RESET << message << "\n";
}
