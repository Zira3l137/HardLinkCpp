#include "argparser.h"
#include "utils.h"
#include <cstring>
#include <iostream>

argparser::ArgParser::ArgParser(int argc, char *argv[])
    : argc(argc), argv(argv) {
    this->addArg("--help", "-h", "Show this help message.", ArgType::Flag);
}

void argparser::ArgParser::addArg(const std::string longName,
                                  const std::string shortName,
                                  const std::string description,
                                  const ArgType type) {
    std::unique_ptr<CmdArg> arg = std::make_unique<CmdArg>();

    arg->longName = longName;
    arg->shortName = shortName;
    arg->type = type;

    if (description != "") {
        arg->description = description;
    }

    args.push_back(std::move(arg));
}

std::map<std::string, std::string> argparser::ArgParser::parse() {
    std::map<std::string, std::string> found;

    for (const auto &arg : args) {
        if (arg->type == ArgType::Flag) {
            found[arg->longName] = "off";
        }
    }

    // CMD args iteration begins
    for (int i = 1; i < this->argc; i++) {
        if (this->argv[i] == nullptr) {
            continue;
        }

        std::string argName(this->argv[i]);

        // Flag iteration begins
        for (const auto &arg : args) {
            if (arg->longName != argName && arg->shortName != argName) {
                continue;
            }

            // Flag switch ends
            switch (arg->type) {

            case ArgType::Flag: {
                found[arg->longName] = "on";
                break;
            }

            case ArgType::Bool: {
                if (i + 1 >= this->argc || this->argv[i + 1] == nullptr) {
                    std::cout << "Missing argument for " << arg->longName
                              << " \n";
                    exit(1);
                }

                auto argValue = utils::str::toLower_c(this->argv[i + 1]);
                if (strcmp(argValue, "false") != 0 &&
                    strcmp(argValue, "true") != 0 &&
                    strcmp(argValue, "0") != 0 && strcmp(argValue, "1") != 0) {
                    std::cout << "Provided argument is not a valid bool "
                              << arg->longName << " \n";
                    exit(1);
                }

                found[arg->longName] = this->argv[i + 1];
                i++;
                break;
            }

            case ArgType::Str: {
                if (i + 1 >= this->argc || this->argv[i + 1] == nullptr) {
                    std::cout << "Missing argument for " << arg->longName
                              << " \n";
                    exit(1);
                }

                found[arg->longName] = this->argv[i + 1];
                i++;
                break;
            }

            case ArgType::Int: {
                if (i + 1 >= this->argc || this->argv[i + 1] == nullptr ||
                    std::string(this->argv[i + 1]) == "") {
                    std::cout << "Missing argument for " << arg->longName
                              << " \n";
                    exit(1);
                }

                if (!(utils::str::isallnum(this->argv[i + 1]))) {
                    std::cout << "Provided argument is not a valid number "
                              << arg->longName << " \n";
                    exit(1);
                }

                found[arg->longName] = this->argv[i + 1];
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
    for (const auto &arg : this->args) {
        std::cout << YELLOW << "\n"
                  << arg->longName << ", " << arg->shortName << GREEN << " - "
                  << arg->type << " \n\t" << RESET << arg->description
                  << std::endl;
    }
}
