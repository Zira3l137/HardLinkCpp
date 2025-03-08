#include "argparser.h"
#include "utils.h"
#include <iostream>
#include <unordered_map>

#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#elif defined(__linux__)
#define PATH_SEPARATOR "/"
#endif

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
                                  const ArgType type) {
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

    if (description != "") {
        arg->description = description;
    }

    this->args[longName] = std::move(arg);
}

std::unordered_map<std::string, std::string> argparser::ArgParser::parse() {
    std::unordered_map<std::string, std::string> found;

    for (const auto &pair : this->args) {
        if (pair.second->type == ArgType::Flag) {
            found[pair.second->longName] = "off";
        }
    }

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

            // Flag switch ends
            switch (pair.second->type) {

            case ArgType::Flag: {
                found[pair.second->longName] = "on";
                break;
            }

            case ArgType::Bool: {
                if (i + 1 >= this->argc || this->argv[i + 1] == nullptr) {
                    std::cout << "Missing argument for "
                              << pair.second->longName << " \n";
                    throw std::invalid_argument("Missing argument for " +
                                                pair.second->longName);
                }

                auto argValue = utils::str::toLower(this->argv[i + 1]);
                if (argValue != "false" && argValue != "true" &&
                    argValue != "0" && argValue != "1") {
                    std::cout << "Provided argument is not a valid bool "
                              << pair.second->longName << " \n";
                    throw std::invalid_argument("Provided argument is not a "
                                                "valid bool " +
                                                pair.second->longName);
                }

                found[pair.second->longName] =
                    (argValue == "true" || argValue == "1") ? "on" : "off";
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

                found[pair.second->longName] = this->argv[i + 1];
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

                if (!(utils::str::isallnum(this->argv[i + 1]))) {
                    std::cout << "Provided argument is not a valid number "
                              << pair.second->longName << " \n";
                    throw std::invalid_argument("Provided argument is not a "
                                                "valid number " +
                                                pair.second->longName);
                }

                found[pair.second->longName] = this->argv[i + 1];
                i++;
                break;
            }
            } // Flag switch ends

            break;
        } // Flag iteration ends

    } // CMD args iteration ends
    return found;
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
