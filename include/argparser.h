#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

namespace argparser {

enum Type { Str, Int, Bool, Double };
using string = std::string;

inline std::ostream &operator<<(std::ostream &os, const Type &type) {
    switch (type) {
    case Type::Str:
        os << "STRING";
        break;
    case Type::Int:
        os << "INT";
        break;
    case Type::Bool:
        os << "BOOL";
        break;
    case Type::Double:
        os << "DOUBLE";
        break;
    }
    return os;
}

using Value = std::variant<string, int, bool, double>;

struct Arg {
    std::string longName;
    std::string shortName;
    std::string desc;
    Type type;
    Value value;
    bool toggle;
};

struct PosArg {
    string name;
    string desc;
    Type type;
    Value value;
};

using ArgPtr = std::variant<std::unique_ptr<Arg>, std::unique_ptr<PosArg>>;

using OptLookup = std::unordered_map<std::string, Arg *>;
using PosLookup = std::unordered_map<unsigned short, PosArg *>;
using Args = std::unordered_map<std::string, ArgPtr>;
using ParsedArgs = std::unordered_map<std::string, Value>;

class ArgParser {
  public:
    ArgParser(int argc, char *argv[]);
    ~ArgParser() = default;

    ParsedArgs parse();
    void printHelp() const;

    // Positionals

    void addPosArg(const string &name, const Value &defaultValue,
                   const string &desc = "");

    void addPosArg(const string &name, const Type &type,
                   const string &desc = "");

    // Optionals

    void addOptArg(const string &longName, const string &shortName,
                   const Value &defaultValue, const string &desc = "");

    void addOptArg(const string &longName, const string &shortName,
                   const Type &type, const string &desc = "");

    void addSwitch(const string &longName, const string &shortName,
                   const string &desc = "");

    // Setters and Getters

    void setDescription(const string &description) {
        this->description = description;
    }
    void setProgramName(const string &programName) {
        this->programName = programName;
    }

    string getDescription() const { return this->description; }
    string getProgramName() const { return this->programName; }

  private:
    int argc;
    char **argv;

    int positionalCounter = 0;

    string programName;
    string description;

    Args args;
    OptLookup optionals;
    PosLookup positionals;

    Value parseArgValue(const string &valueString, const argparser::Type &type);
    PosArg *parsePosArg(const int &index, const string &argName);
    Arg *parseArg(const int &index, const string &argName);
};

namespace misc {

constexpr const char *GREEN = "\033[32m";
constexpr const char *RED = "\033[31m";
constexpr const char *YELLOW = "\033[33m";
constexpr const char *RESET = "\033[0m";

#define THROW_ERROR(message)                                                   \
    std::cout << argparser::misc::RED << "Error: " << argparser::misc::RESET   \
              << message << "\n";                                              \
    throw std::runtime_error(message);

#define PRINT(message)                                                         \
    std::cout << __FILE__ << ":" << __LINE__ << ": " << message << "\n";

bool isNumber(const string &s);
string toLower(string s);

} // namespace misc

} // namespace argparser
