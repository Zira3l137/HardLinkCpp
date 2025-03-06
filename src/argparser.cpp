#include "argparser.h"
#include "utils.h"

CmdArg::CmdArg(const String longName, const String shortName,
               const ArgType type)
    : longName(longName), shortName(shortName), type(type) {}

ArgParser::ArgParser(int argc, char *argv[]) : argc(argc), argv(argv) {}

void ArgParser::addArg(uPtr<CmdArg> arg) { args.push_back(move_uPtr(arg)); }

StringMap ArgParser::parse() {
  StringMap found;
  for (const auto &arg : args) {
    if (arg->type == ArgType::Bool) {
      found[arg->longName] = "false";
    }
  }

  for (int i = 1; i < this->argc; i++) {
    if (this->argv[i] == NULL) {
      continue;
    }

    String argName(this->argv[i]);

    for (const auto &arg : args) {
      if (arg->longName == argName OR arg->shortName == argName) {
        switch (arg->type) {
        case ArgType::Bool: {
          found[arg->longName] = "true";
          break;
        }
        case ArgType::Str: {
          found[arg->longName] = this->argv[i + 1];
          break;
        }
        case ArgType::Int: {
          if (isallnum(this->argv[i + 1])) {
            found[arg->longName] = this->argv[i + 1];
          } else {
            std::cout << "Error: " << this->argv[i + 1]
                      << " is not a valid integer\n";
            exit(1);
          }
        }
        }
        break;
      }
    }
  }

  return found;
}
