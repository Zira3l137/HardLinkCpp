#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

namespace argparser {

constexpr const char *YELLOW = "\033[33m";
constexpr const char *GREEN = "\033[32m";
constexpr const char *RESET = "\033[0m";

enum ArgType { Str, Int, Bool, Flag, Double };

inline std::ostream &operator<<(std::ostream &os, const ArgType &type) {
    switch (type) {
    case ArgType::Str:
        os << "STRING";
        break;
    case ArgType::Int:
        os << "INT";
        break;
    case ArgType::Bool:
        os << "BOOL";
        break;
    case ArgType::Flag:
        os << "FLAG";
        break;
    case ArgType::Double:
        os << "DOUBLE";
        break;
    }
    return os;
}

using ArgValue = std::variant<std::string, int, bool, double>;

struct CmdArg {
    std::string longName;
    std::string shortName;
    std::string description;
    ArgType type;
    ArgValue value;

    template <typename T> T &get() { return std::get<T>(value); }
};

class ArgParser {
  public:
    std::unordered_map<std::string, std::unique_ptr<CmdArg>> args;

    ArgParser(int argc, char *argv[]);
    ~ArgParser() = default;

    void addArg(const std::string longName, const std::string shortName,
                const ArgValue defaultValue, const std::string description = "",
                const ArgType type = ArgType::Str);

    void addArg(const std::string longName, const std::string shortName,
                const std::string description = "",
                const ArgType type = ArgType::Str);

    void parse();

    void setProgramName(const std::string name) { this->programName = name; }

    void setDescription(const std::string description) {
        this->description = description;
    }

    std::string getProgramName() { return this->programName; }

    std::string getDescription() { return this->description; }

    void printHelp();

  private:
    int argc;
    char **argv;

    std::string programName;
    std::string description = "";
};

namespace misc {

bool isNumber(const std::string &s);
std::string toLower(const std::string &s);

} // namespace misc

} // namespace argparser
