#include "argparser.h"
#include <algorithm>
#include <charconv>
#include <memory>
#include <unordered_map>
#include <vector>

#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#elif defined(__linux__)
#define PATH_SEPARATOR "/"
#endif

using string = std::string;

std::unordered_map<int, const argparser::Type> typeMap = {
    {0, argparser::Type::Str},
    {1, argparser::Type::Int},
    {2, argparser::Type::Bool},
    {3, argparser::Type::Double},
};

std::unordered_map<const argparser::Type, argparser::Value> typeDefaults = {
    {argparser::Type::Str, string("")},
    {argparser::Type::Int, 0},
    {argparser::Type::Bool, false},
    {argparser::Type::Double, 0.0},
};

bool argparser::misc::isNumber(const string &s) {
    if (s.empty())
        return false;

    int int_val;
    const char *begin = s.data();
    const char *end = begin + s.size();

    auto [ptr, ec] = std::from_chars(begin, end, int_val);
    if (ec == std::errc())
        return true;

    double double_val;
    auto [dptr, dec] = std::from_chars(begin, end, double_val);
    return (dec == std::errc());
}

string argparser::misc::toLower(string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

argparser::ArgParser::ArgParser(int argc, char *argv[])
    : argc(argc), argv(argv) {
    string executable(argv[0]);
    this->programName =
        executable.substr(executable.find_last_of(PATH_SEPARATOR) + 1);
    this->addSwitch("--help", "-h", "Show this help message.");
}

void argparser::ArgParser::addPosArg(const string &name,
                                     const argparser::Value &defaultValue,
                                     const string &desc) {

    string normalized_name = argparser::misc::toLower(name);

    if (name.empty()) {
        THROW_ERROR("Positional argument name cannot be empty.");
    }
    if (this->args.find(normalized_name) != this->args.end()) {
        THROW_ERROR("Positional argument " + name + " already exists.");
    }

    auto newArg = std::make_unique<argparser::PosArg>();
    newArg->name = normalized_name;
    newArg->desc = desc;
    newArg->value = defaultValue;
    newArg->type = typeMap[defaultValue.index()];

    this->args[normalized_name] = std::move(newArg);
    this->positionals[this->positionalCounter++] =
        std::get<std::unique_ptr<argparser::PosArg>>(
            (this->args[normalized_name]))
            .get();
}

void argparser::ArgParser::addPosArg(const string &name,
                                     const argparser::Type &type,
                                     const string &desc) {

    string normalized_name = argparser::misc::toLower(name);

    if (name.empty()) {
        THROW_ERROR("Positional argument name cannot be empty.");
    }
    if (this->args.find(normalized_name) != this->args.end()) {
        THROW_ERROR("Positional argument " + name + " already exists.");
    }

    auto newArg = std::make_unique<argparser::PosArg>();
    newArg->name = normalized_name;
    newArg->desc = desc;
    newArg->type = type;
    newArg->value = typeDefaults[type];

    this->args[normalized_name] = std::move(newArg);

    this->positionals[this->positionalCounter++] =
        std::get<std::unique_ptr<argparser::PosArg>>(
            (this->args[normalized_name]))
            .get();
}

void argparser::ArgParser::addOptArg(const string &longName,
                                     const string &shortName,
                                     const Value &defaultValue,
                                     const string &desc) {

    string normalized_lname = argparser::misc::toLower(longName);
    string normalized_sname = argparser::misc::toLower(shortName);

    if (longName.empty()) {
        THROW_ERROR("Option argument name cannot be empty.");
    }
    if (shortName.empty()) {
        THROW_ERROR("Option argument short name cannot be empty.");
    }
    if (this->args.find(normalized_lname) != this->args.end()) {
        THROW_ERROR("Option argument " + longName + " already exists.");
    }

    auto newArg = std::make_unique<argparser::Arg>();
    newArg->longName = normalized_lname;
    newArg->shortName = normalized_sname;
    newArg->desc = desc;
    newArg->value = defaultValue;
    newArg->type = typeMap[defaultValue.index()];

    this->args[normalized_lname] = std::move(newArg);

    this->optionals[normalized_lname] =
        std::get<std::unique_ptr<argparser::Arg>>(this->args[normalized_lname])
            .get();

    this->optionals[normalized_sname] =
        std::get<std::unique_ptr<argparser::Arg>>(this->args[normalized_lname])
            .get();
}

void argparser::ArgParser::addOptArg(const string &longName,
                                     const string &shortName,
                                     const argparser::Type &type,
                                     const string &desc) {

    string normalized_lname = argparser::misc::toLower(longName);
    string normalized_sname = argparser::misc::toLower(shortName);

    if (longName.empty()) {
        THROW_ERROR("Option argument name cannot be empty.");
    }
    if (shortName.empty()) {
        THROW_ERROR("Option argument short name cannot be empty.");
    }
    if (this->args.find(normalized_lname) != this->args.end()) {
        THROW_ERROR("Option argument " + longName + " already exists.");
    }

    auto newArg = std::make_unique<argparser::Arg>();
    newArg->longName = longName;
    newArg->shortName = shortName;
    newArg->desc = desc;
    newArg->type = type;
    newArg->value = typeDefaults[type];

    this->args[normalized_lname] = std::move(newArg);

    this->optionals[normalized_lname] =
        std::get<std::unique_ptr<argparser::Arg>>(this->args[normalized_lname])
            .get();

    this->optionals[normalized_sname] =
        std::get<std::unique_ptr<argparser::Arg>>(this->args[normalized_lname])
            .get();
}

void argparser::ArgParser::addSwitch(const string &longName,
                                     const string &shortName,
                                     const string &desc) {

    string normalized_lname = argparser::misc::toLower(longName);
    string normalized_sname = argparser::misc::toLower(shortName);

    if (longName.empty()) {
        THROW_ERROR("Option argument name cannot be empty.");
    }
    if (shortName.empty()) {
        THROW_ERROR("Option argument short name cannot be empty.");
    }
    if (this->args.find(normalized_lname) != this->args.end()) {
        THROW_ERROR("Option argument " + longName + " already exists.");
    }

    auto newArg = std::make_unique<argparser::Arg>();
    newArg->longName = longName;
    newArg->shortName = shortName;
    newArg->desc = desc;
    newArg->type = argparser::Type::Bool;
    newArg->value = false;
    newArg->toggle = true;

    this->args[normalized_lname] = std::move(newArg);

    this->optionals[normalized_lname] =
        std::get<std::unique_ptr<argparser::Arg>>(this->args[normalized_lname])
            .get();

    this->optionals[normalized_sname] =
        std::get<std::unique_ptr<argparser::Arg>>(this->args[normalized_lname])
            .get();
}

argparser::Value
argparser::ArgParser::parseArgValue(const string &valueString,
                                    const argparser::Type &type) {

    switch (type) {

    case argparser::Type::Str:
        return valueString;

    case argparser::Type::Int:
        if (!argparser::misc::isNumber(valueString)) {
            THROW_ERROR("Value " + valueString + " is not an integer.");
        }
        return std::stoi(valueString);

    case argparser::Type::Bool:
        if (valueString != "true" && valueString != "false" &&
            valueString != "1" && valueString != "0") {
            THROW_ERROR("Value " + valueString + " is not a boolean.");
        }
        return (valueString == "true" || valueString == "1");

    case argparser::Type::Double:
        if (!argparser::misc::isNumber(valueString)) {
            THROW_ERROR("Value " + valueString + " is not an integer.");
        }
        return std::stod(valueString);

    default:
        return valueString;
    }
}

argparser::Arg *argparser::ArgParser::parseArg(const int &index,
                                               const string &argName) {
    auto registeredArg = this->optionals.find(argName);

    if (registeredArg == this->optionals.end()) {
        THROW_ERROR("Optional argument " + argName + " does not exist.");
    }

    if ((!registeredArg->second->toggle) && this->argv[index + 1] == nullptr) {
        THROW_ERROR("Optional argument " + argName + " requires a value.");
    }

    string argValue = this->argv[index + 1];
    registeredArg->second->value =
        registeredArg->second->toggle
            ? true
            : this->parseArgValue(argValue, registeredArg->second->type);

    return registeredArg->second;
}

argparser::ParsedArgs argparser::ArgParser::parse() {

    ParsedArgs parsedArgs;
    int posIndex = 0;

    for (int i = 1; i < this->argc; i++) {
        string token = argparser::misc::toLower(this->argv[i]);

        if (token == "--help" || token == "-h") {
            this->printHelp();
            std::exit(0);
        }

        if (token[0] != '-') { // Positional argument
            if (posIndex >= this->positionalCounter) {
                THROW_ERROR("Too many positional arguments.");
            }

            PosArg *argPtr = this->positionals[posIndex++];
            argPtr->value = this->parseArgValue(token, argPtr->type);
            parsedArgs[argPtr->name] = argPtr->value;
        } else { // Optional argument
            Arg *argPtr = this->parseArg(i, token);
            parsedArgs[argPtr->longName.substr(2)] = argPtr->value;
            if (!argPtr->toggle)
                i++;
        }
    }

    for (const auto &entry : this->optionals) {
        std::string key = entry.first.substr(2);
        if (parsedArgs.find(key) == parsedArgs.end()) {
            parsedArgs[key] = entry.second->value;
        }
    }

    if (posIndex < this->positionalCounter) {
        THROW_ERROR("Not enough positional arguments.");
    }

    return parsedArgs;
}

void argparser::ArgParser::printHelp() const {
    std::cout << "\n"
              << argparser::misc::GREEN << this->getProgramName()
              << argparser::misc::RESET << "\n\n";

    if (this->getDescription() != "") {
        std::cout << this->description << "\n";
    }

    std::cout << "Usage: " << argparser::misc::GREEN << this->getProgramName()
              << " " << argparser::misc::RESET;

    for (const auto &entry : this->positionals) {
        std::cout << argparser::misc::YELLOW << "<" << entry.second->name
                  << "> " << argparser::misc::RESET;
    }

    std::cout << "[OPTIONS]\n";
    std::cout << "\n\tPositional arguments:\n";

    for (const auto &entry : this->positionals) {
        std::cout << argparser::misc::YELLOW << "\t\t" << entry.second->name
                  << argparser::misc::RESET << ":\n\t\t\t" << entry.second->desc
                  << "\n";
    }

    std::vector<std::string> alreadyPrinted;
    std::cout << "\tOptions:\n";
    for (const auto &entry : this->optionals) {

        if (std::find(alreadyPrinted.begin(), alreadyPrinted.end(),
                      entry.second->longName) != alreadyPrinted.end())
            continue;

        alreadyPrinted.push_back(entry.second->longName);
        std::cout << argparser::misc::YELLOW << "\t\t" << entry.second->longName
                  << ", " << entry.second->shortName << argparser::misc::RESET
                  << ":\n\t\t\t" << entry.second->desc << "\n";
    }
    std::cout << "\n";
}
