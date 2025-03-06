#pragma once

#define HELP                                                                   \
  "linkfiles -s(--source) SOURCE_DIR -o(--output) OUTPUT_DIR [-h(--help)] "    \
  "[-i(--ignore) IGNORE_PATTERN] [-d(--debug)]"
#define NOT !
#define OR ||
#define AND &&

#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#define Vector std::vector
#define uPtr std::unique_ptr
#define to_uPtr std::make_unique
#define move_uPtr std::move

extern bool DEBUG_MODE;

void setDebugMode(bool debugMode);

namespace fs = std::filesystem;
typedef fs::path Path;

typedef std::stringstream sStream;
typedef std::string String;
typedef Vector<String> StringVector;
typedef Vector<Path> PathVector;
typedef std::map<String, String> StringMap;

bool startswith(const String &haystack, const String &needle);
bool endswith(const String &haystack, const String &needle);
bool contains(const String &haystack, const String &needle);
bool isallnum(const String &s);
StringVector *split(const String &s, char delim);

void print(const String &s);
void printLn(const String &s);
void printErr(const String &s);
