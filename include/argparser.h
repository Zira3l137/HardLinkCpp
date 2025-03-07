#pragma once

#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace argparser {

enum ArgType { Str, Int, Bool, Flag };

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
