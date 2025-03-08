#include "argparser.h"
#include <charconv>
#include <iostream>
#include <unordered_map>

#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#elif defined(__linux__)
#define PATH_SEPARATOR "/"
#endif

bool argparser::misc::isallnum(const std::string &s) {
    if (s.empty()) {
        return false;
    }

    int int_val;
    const char *int_first = s.data();
    const char *int_last = int_first + s.size();
    if (std::from_chars(int_first, int_last, int_val).ec == std::errc()) {
        return true; // Successfully parsed as an integer
    }

    double double_val;
    const char *double_first = s.data();
    const char *double_last = double_first + s.size();
    if (std::from_chars(double_first, double_last, double_val).ec ==
        std::errc()) {
        return true; // Successfully parsed as a double
    }

    return false; // Neither integer nor double
}

std::string argparser::misc::toLower(const std::string &s) {
    std::string copy(s);

    for (int i = 0; i < (int)copy.length(); i++) {
        copy[i] = tolower(copy[i]);
    }

    return copy;
}

argparser::ArgParser::ArgParser(int argc, char *argv[])
    : argc(argc), argv(argv) {
    std::string executable(argv[0]);
    this->programName =
        executable.substr(executable.find_last_of(PATH_SEPARATOR) + 1);
    this->addArg("--help", "-h", "Show this help message.", ArgType::Flag);
}

void argparser::ArgParser::addArg(const std::string longName,
                                  const std::string shortName,
                                  const std::string description,
                                  const argparser::ArgType type) {

    for (const auto &pair : this->args) {
        if (pair.second->longName == longName ||
            pair.second->shortName == shortName) {
            throw std::invalid_argument("Duplicate argument name: " +
                                        pair.second->longName);
        }
    }

    std::unique_ptr<CmdArg> arg = std::make_unique<CmdArg>();

    arg->longName = longName;
    arg->shortName = shortName;
    arg->type = type;

    switch (type) {
    case argparser::ArgType::Str:
        arg->value = std::string("");
        break;
    case argparser::ArgType::Int:
        arg->value = int(0);
        break;
    case argparser::ArgType::Bool:
    case argparser::ArgType::Flag:
        arg->value = bool(false);
        break;
    case argparser::ArgType::Double:
        arg->value = double(0.0);
    }

    if (description != "") {
        arg->description = description;
    }

    this->args[longName] = std::move(arg);
}

void argparser::ArgParser::addArg(const std::string longName,
                                  const std::string shortName,
                                  const argparser::ArgValue defaultValue,
                                  const std::string description,
                                  const argparser::ArgType type) {

    for (const auto &pair : this->args) {
        if (pair.second->longName == longName ||
            pair.second->shortName == shortName) {
            throw std::invalid_argument("Duplicate argument name: " +
                                        pair.second->longName);
        }
    }

    std::unique_ptr<CmdArg> arg = std::make_unique<CmdArg>();

    arg->longName = longName;
    arg->shortName = shortName;
    arg->type = type;

    switch (type) {
    case ArgType::Str:
        try {
            arg->value = std::get<std::string>(defaultValue);
            break;
        } catch (const std::bad_variant_access &e) {
            throw std::invalid_argument("Invalid default value for a string "
                                        "argument: " +
                                        longName);
        }
    case ArgType::Int:
        try {
            arg->value = std::get<int>(defaultValue);
            break;
        } catch (const std::bad_variant_access &e) {
            throw std::invalid_argument("Invalid default value for a numeric "
                                        "argument: " +
                                        longName);
        }
    case ArgType::Bool:
    case ArgType::Flag:
        try {
            arg->value = std::get<bool>(defaultValue);
            break;
        } catch (const std::bad_variant_access &e) {
            throw std::invalid_argument("Invalid default value for a boolean "
                                        "argument: " +
                                        longName);
        }
    case ArgType::Double:
        try {
            arg->value = std::get<double>(defaultValue);
            break;
        } catch (const std::bad_variant_access &e) {
            throw std::invalid_argument(
                "Invalid default value for a floating point numeric"
                "argument: " +
                longName);
        }
    }

    if (description != "") {
        arg->description = description;
    }

    this->args[longName] = std::move(arg);
}

void argparser::ArgParser::parse() {
    // CMD args iteration begins
    for (int i = 1; i < this->argc; i++) {
        if (this->argv[i] == nullptr) {
            continue;
        }

        std::string argName(this->argv[i]);

        // Flag iteration begins
        for (const auto &pair : this->args) {
            if (pair.second->longName != argName &&
                pair.second->shortName != argName) {
                continue;
            }

            // Flag switch begins
            switch (pair.second->type) {

            case ArgType::Flag: {
                pair.second->value = true;
                break;
            }

            case ArgType::Bool: {
                if (i + 1 >= this->argc || this->argv[i + 1] == nullptr) {
                    std::cout << "Missing argument for "
                              << pair.second->longName << " \n";
                    throw std::invalid_argument("Missing argument for " +
                                                pair.second->longName);
                }

                auto argValue = argparser::misc::toLower(this->argv[i + 1]);
                if (argValue != "false" && argValue != "true" &&
                    argValue != "0" && argValue != "1") {
                    std::cout << "Provided argument is not a valid bool "
                              << pair.second->longName << " \n";
                    throw std::invalid_argument("Provided argument is not a "
                                                "valid bool " +
                                                pair.second->longName);
                }

                pair.second->value =
                    (argValue == "true" || argValue == "1") ? true : false;
                i++;
                break;
            }

            case ArgType::Str: {
                if (i + 1 >= this->argc || this->argv[i + 1] == nullptr) {
                    std::cout << "Missing argument for "
                              << pair.second->longName << " \n";
                    throw std::invalid_argument("Missing argument for " +
                                                pair.second->longName);
                }
                pair.second->value = this->argv[i + 1];
                i++;
                break;
            }

            case ArgType::Int: {
                if (i + 1 >= this->argc || this->argv[i + 1] == nullptr ||
                    std::string(this->argv[i + 1]) == "") {
                    std::cout << "Missing argument for "
                              << pair.second->longName << " \n";
                    throw std::invalid_argument("Missing argument for " +
                                                pair.second->longName);
                }

                if (!(argparser::misc::isallnum(this->argv[i + 1]))) {
                    std::cout << "Provided argument is not a valid number "
                              << pair.second->longName << " \n";
                    throw std::invalid_argument("Provided argument is not a "
                                                "valid number " +
                                                pair.second->longName);
                }

                pair.second->value = std::stoi(this->argv[i + 1]);
                i++;
                break;
            }

            case ArgType::Double: {
                if (i + 1 >= this->argc || this->argv[i + 1] == nullptr ||
                    std::string(this->argv[i + 1]) == "") {
                    std::cout << "Missing argument for "
                              << pair.second->longName << " \n";
                    throw std::invalid_argument("Missing argument for " +
                                                pair.second->longName);
                }

                if (!(argparser::misc::isallnum(this->argv[i + 1]))) {
                    std::cout << "Provided argument is not a valid number "
                              << pair.second->longName << " \n";
                    throw std::invalid_argument("Provided argument is not a "
                                                "valid number " +
                                                pair.second->longName);
                }

                pair.second->value = std::stod(this->argv[i + 1]);
            }
            } // Flag switch ends

            break;
        } // Flag iteration ends

    } // CMD args iteration ends
}

void argparser::ArgParser::printHelp() {
    std::cout << "\n" << GREEN << this->getProgramName() << RESET << "\n";

    if (this->getDescription() != "") {
        std::cout << this->description << "\n";
    }

    std::cout << "Usage: " << this->getProgramName() << " ";
    for (const auto &pair : this->args) {
        std::cout << pair.second->shortName << "(" << pair.second->longName
                  << ") ";
        if (pair.second->type != ArgType::Flag) {
            std::cout << "[" << pair.second->type << "] ";
        }
    }
    std::cout << "\n";

    for (const auto &pair : this->args) {
        std::cout << YELLOW << "\n\t" << pair.second->longName << ", "
                  << pair.second->shortName << GREEN << " - "
                  << pair.second->type << " \n\t\t" << RESET
                  << pair.second->description << "\n\t";
    }
}
