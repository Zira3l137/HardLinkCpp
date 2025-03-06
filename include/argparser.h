#ifndef ARGPARSER_H
#define ARGPARSER_H

#include "utils.h"

typedef enum { Str, Int, Bool } ArgType;

struct CmdArg {
  String longName;
  String shortName;
  ArgType type;
};

class ArgParser {
public:
  std::vector<uPtr<CmdArg>> args;

  ArgParser(int argc, char *argv[]);
  // ~ArgParser();

  void addArg(const String longName, const String shortName,
              const ArgType type);
  StringMap parse();

private:
  int argc;
  char **argv;
};

#endif
