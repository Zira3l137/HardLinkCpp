#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

namespace argparser {

enum Type { Str, Int, Bool, Double };

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

using Value = std::variant<std::string, int, bool, double>;

struct Arg {
    std::string longName;
    std::string shortName;
    std::string desc;
    Type type;
    Value value;
    bool toggle;
};

struct PosArg {
    std::string name;
    std::string desc;
    Type type;
    Value value;
};

using ArgPtr = std::variant<std::unique_ptr<Arg>, std::unique_ptr<PosArg>>;

using OptLookup = std::unordered_map<std::string, Arg *>;
using PosLookup = std::unordered_map<unsigned short, PosArg *>;
using Args = std::unordered_map<std::string, ArgPtr>;

class ParsedArgValue {
    Value value;

  public:
    ParsedArgValue() : value(std::string("")) {}
    ParsedArgValue(const Value &v) : value(v) {}

    template <typename T> operator T() const { return std::get<T>(value); }
};

using ParsedArgs = std::unordered_map<std::string, ParsedArgValue>;

class ArgParser {
  public:
    ArgParser(int argc, char *argv[]);
    ~ArgParser() = default;

    ParsedArgs parse();
    void printHelp() const;

    // Positionals

    void addPosArg(const std::string &name, const Value &defaultValue,
                   const std::string &desc = "");

    void addPosArg(const std::string &name, const Type &type,
                   const std::string &desc = "");

    // Optionals

    void addOptArg(const std::string &longName, const std::string &shortName,
                   const Value &defaultValue, const std::string &desc = "");

    void addOptArg(const std::string &longName, const std::string &shortName,
                   const Type &type, const std::string &desc = "");

    void addSwitch(const std::string &longName, const std::string &shortName,
                   const std::string &desc = "");

    // Setters and Getters

    void setDescription(const std::string &description) {
        this->description = description;
    }
    void setProgramName(const std::string &programName) {
        this->programName = programName;
    }

    std::string getDescription() const { return this->description; }
    std::string getProgramName() const { return this->programName; }

  private:
    int argc;
    char **argv;

    int positionalCounter = 0;

    std::string programName;
    std::string description;

    Args args;
    OptLookup optionals;
    PosLookup positionals;

    Value parseArgValue(const std::string &valueString,
                        const argparser::Type &type);

    PosArg *parsePosArg(const int &index, const std::string &argName);

    Arg *parseArg(const int &index, const std::string &argName);

    size_t estimateCapacity() const;
};

namespace misc {

constexpr const char *GREEN = "\033[32m";
constexpr const char *RED = "\033[31m";
constexpr const char *YELLOW = "\033[33m";
constexpr const char *RESET = "\033[0m";
constexpr const size_t COLOR_LEN = 8;
constexpr const size_t RESET_LEN = 7;

#define THROW_ERROR(message)                                                   \
    std::cout << argparser::misc::RED << "Error: " << argparser::misc::RESET   \
              << message << "\n";                                              \
    throw std::runtime_error(message);

#define PRINT(message)                                                         \
    std::cout << __FILE__ << ":" << __LINE__ << ": " << message << "\n";

bool isNumber(const std::string &s);
std::string toLower(std::string s);

} // namespace misc

} // namespace argparser
