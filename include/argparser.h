#ifndef ARGPARSER_H
#define ARGPARSER_H

#include "utils.h"

typedef enum { Str, Int, Bool } ArgType;

class CmdArg {
public:
  String longName;
  String shortName;
  ArgType type;

  CmdArg(const String longName, const String shortName, const ArgType type);
};

class ArgParser {
public:
  Vector<uPtr<CmdArg>> args;

  ArgParser(int argc, char *argv[]);
  // ~ArgParser();

  void addArg(uPtr<CmdArg> arg);
  StringMap parse();

private:
  int argc;
  char **argv;
};

#endif
