#pragma once

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

namespace fs = std::filesystem;
typedef fs::path Path;

typedef std::ostringstream sStream;
typedef std::string String;
typedef std::vector<String> StringVector;
typedef std::vector<Path> PathVector;
typedef std::map<String, String> StringMap;

StringMap getCmdArgs(int argc, char *argv[]);
void print(const String &s);
void printLn(const String &s);
void printErr(const String &s);
