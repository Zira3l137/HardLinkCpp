#include "utils.h"

bool DEBUG_MODE = false;

void setDebugMode(bool debug) { DEBUG_MODE = debug; }

StringMap getCmdArgs(int argc, char *argv[]) {
  StringMap args;

  int helpFlagIndex = -1;
  int srcFlagIndex = -1;
  int destFlagIndex = -1;
  int ignoreFlagIndex = -1;
  int debugFlagIndex = -1;

  for (int i = 1; i < argc; i++) {
    if (argv[i] == NULL) {
      continue;
    }

    String sArg(argv[i]);

    if (sArg == "-h" OR sArg == "--help") {
      helpFlagIndex = i;
    } else if (sArg == "-s" OR sArg == "--source") {
      srcFlagIndex = ++i;
    } else if (sArg == "-o" OR sArg == "--output") {
      destFlagIndex = ++i;
    } else if (sArg == "-i" OR sArg == "--ignore") {
      ignoreFlagIndex = ++i;
    } else if (sArg == "-d" OR sArg == "--debug") {
      debugFlagIndex = i;
    }
  }

  if (NOT(helpFlagIndex == -1)) {
    args["help"] = String(argv[helpFlagIndex]);
  }
  if (NOT(srcFlagIndex == -1)) {
    args["source"] = String(argv[srcFlagIndex]);
  }
  if (NOT(destFlagIndex == -1)) {
    args["output"] = String(argv[destFlagIndex]);
  }
  if (NOT(destFlagIndex == -1)) {
    args["ignore"] = String(argv[ignoreFlagIndex]);
  }
  if (NOT(debugFlagIndex == -1)) {
    args["debug"] = String(argv[debugFlagIndex]);
  }

  return args;
}

bool startswith(const String &haystack, const String &needle) {
  return haystack.find(needle) == 0;
}

bool endswith(const String &haystack, const String &needle) {
  return haystack.find(needle) == (haystack.length() - needle.length());
}

bool contains(const String &haystack, const String &needle) {
  return haystack.find(needle) != String::npos;
}

StringVector *split(const String &s, char delim) {
  StringVector *result = new StringVector();
  sStream ss(s);
  String item;

  while (std::getline(ss, item, delim)) {
    result->push_back(item);
  }
  return result;
}

void print(const String &s) {
  if (DEBUG_MODE) {
    std::cout << s;
  }
}

void printLn(const String &s) {
  if (DEBUG_MODE) {
    std::cout << s << std::endl;
  }
}

void printErr(const String &s) {
  if (DEBUG_MODE) {
    std::cout << "Error: " << s << std::endl;
  }
}
