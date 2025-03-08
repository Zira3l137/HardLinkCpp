#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace argparser {

constexpr const char *YELLOW = "\033[33m";
constexpr const char *GREEN = "\033[32m";
constexpr const char *RESET = "\033[0m";

enum ArgType { Str, Int, Bool, Flag };

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
    }
    return os;
}

struct CmdArg {
    std::string longName;
    std::string shortName;
    std::string description;
    ArgType type;
};

class ArgParser {
  public:
    std::vector<std::unique_ptr<CmdArg>> args;

    ArgParser(int argc, char *argv[]);
    // ~ArgParser();

    void addArg(const std::string longName, const std::string shortName,
                const std::string description = "",
                const ArgType type = ArgType::Str);

    std::map<std::string, std::string> parse();

    void printHelp();

  private:
    int argc;
    char **argv;
};

} // namespace argparser
