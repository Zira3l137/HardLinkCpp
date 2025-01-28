#include "utils.h"

StringMap getCmdArgs(int argc, char *argv[]) {
  StringMap args;

  int helpFlagIndex = -1;
  int srcFlagIndex = -1;
  int destFlagIndex = -1;

  for (int i = 1; i < argc; i++) {
    if (argv[i] == NULL) {
      continue;
    }

    String sArg(argv[i]);

    if (sArg == "-h" OR sArg == "--help") {
      helpFlagIndex = i;
    } else if (sArg == "-i" OR sArg == "--input") {
      srcFlagIndex = ++i;
    } else if (sArg == "-o" OR sArg == "--output") {
      destFlagIndex = ++i;
    }
  }

  if (NOT(helpFlagIndex == -1)) {
    args["help"] = String(argv[helpFlagIndex]);
  }
  if (NOT(srcFlagIndex == -1)) {
    args["input"] = String(argv[srcFlagIndex]);
  }
  if (NOT(destFlagIndex == -1)) {
    args["output"] = String(argv[destFlagIndex]);
  }

  return args;
}

void print(const String &s) { std::cout << s; }
void printLn(const String &s) { std::cout << s << std::endl; }
void printErr(const String &s) { std::cout << "Error: " << s << std::endl; }
