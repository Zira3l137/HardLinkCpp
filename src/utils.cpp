#include "utils.h"

bool DEBUG_MODE = false;

void setDebugMode(bool debug) { DEBUG_MODE = debug; }

bool startswith(const String &haystack, const String &needle) {
  return haystack.find(needle) == 0;
}

bool endswith(const String &haystack, const String &needle) {
  return haystack.find(needle) == (haystack.length() - needle.length());
}

bool contains(const String &haystack, const String &needle) {
  return haystack.find(needle) != String::npos;
}

bool isallnum(const String &s) {
  if (s.empty()) {
    return false;
  }
  for (char c : s) {
    if (!std::isdigit(c)) {
      return false;
    }
  }
  return true;
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
