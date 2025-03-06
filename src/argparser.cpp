#include "argparser.h"
#include "utils.h"

ArgParser::ArgParser(int argc, char *argv[]) : argc(argc), argv(argv) {}

void ArgParser::addArg(const String longName, const String shortName,
                       const ArgType type) {
  uPtr<CmdArg> arg = to_uPtr<CmdArg>();

  arg->longName = longName;
  arg->shortName = shortName;
  arg->type = type;

  args.push_back(move_uPtr(arg));
}

StringMap ArgParser::parse() {
  StringMap found;

  // Set default values for boolean flags
  for (const auto &arg : args) {
    if (arg->type == ArgType::Bool) {
      found[arg->longName] = "false";
    }
  }

  // Iterate over command line arguments
  for (int i = 1; i < this->argc; i++) {
    if (this->argv[i] == nullptr) {
      continue;
    }

    String argName(this->argv[i]);

    // Iterate over known flags
    for (const auto &arg : args) {

      // Skip unknown flags
      if (arg->longName != argName && arg->shortName != argName) {
        continue;
      }

      switch (arg->type) {

      // Handle boolean flags
      case ArgType::Bool: {
        found[arg->longName] = "true";
        break;
      }

      // Handle string flags
      case ArgType::Str: {
        // Check if the next argument exists
        if (i + 1 >= this->argc || this->argv[i + 1] == nullptr ||
            String(this->argv[i + 1]) == "") {
          printErr("Missing argument for " + arg->longName);
          exit(1);
        }

        // Set the value for the flag and skip the next argument as
        // it has been consumed
        found[arg->longName] = this->argv[i + 1];
        i++;
        break;
      }

      // Handle integer flags
      case ArgType::Int: {
        if (i + 1 >= this->argc || this->argv[i + 1] == nullptr ||
            String(this->argv[i + 1]) == "") {
          printErr("Missing argument for " + arg->longName);
          exit(1);
        }

        // Check if the next argument is a number
        if (!(isallnum(this->argv[i + 1]))) {
          printErr("Expected a number for " + arg->longName + " got \"" +
                   argv[i + 1] + "\"");
          exit(1);
        }

        found[arg->longName] = this->argv[i + 1];
        i++;
        break;
      }
      }

      // Found the argument no need to look further
      break;
    }
  }
  return found;
}
