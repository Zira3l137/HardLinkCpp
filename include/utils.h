#pragma once
#define HELP                                                                   \
  "linkfiles -s(--source) SOURCE_DIR -o(--output) OUTPUT_DIR [-h(--help)] "    \
  "[-i(--ignore) IGNORE_PATTERN] [-d(--debug)]"

#include <filesystem>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

#define NOT !
#define OR ||
#define AND &&

extern bool DEBUG_MODE;

void setDebugMode(bool debugMode);

namespace fs = std::filesystem;
typedef fs::path Path;

typedef std::stringstream sStream;
typedef std::string String;
typedef std::vector<String> StringVector;
typedef std::vector<Path> PathVector;
typedef std::map<String, String> StringMap;

StringMap getCmdArgs(int argc, char *argv[]);
void print(const String &s);
void printLn(const String &s);
void printErr(const String &s);

bool startswith(const String &haystack, const String &needle);
bool endswith(const String &haystack, const String &needle);
bool contains(const String &haystack, const String &needle);

StringVector *split(const String &s, char delim);
