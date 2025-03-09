#pragma once

#include <string>
#include <vector>

namespace utils {

namespace str {

bool startswith(const std::string &haystack, const std::string &needle);
bool endswith(const std::string &haystack, const std::string &needle);
bool contains(const std::string &haystack, const std::string &needle);
bool isNumber(const std::string &s);
std::string toLower(const std::string &s);
std::vector<std::string> *split(const std::string &s, char delim);

} // namespace str

} // namespace utils
