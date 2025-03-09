#include "utils.h"
#include <charconv>
#include <sstream>

bool utils::str::startswith(const std::string &haystack,
                            const std::string &needle) {
    return haystack.find(needle) == 0;
}

bool utils::str::endswith(const std::string &haystack,
                          const std::string &needle) {
    return haystack.find(needle) == (haystack.length() - needle.length());
}

bool utils::str::contains(const std::string &haystack,
                          const std::string &needle) {
    return haystack.find(needle) != std::string::npos;
}

bool utils::str::isNumber(const std::string &s) {
    if (s.empty()) {
        return false;
    }

    int int_val;
    const char *int_first = s.data();
    const char *int_last = int_first + s.size();
    if (std::from_chars(int_first, int_last, int_val).ec == std::errc()) {
        return true; // Successfully parsed as an integer
    }

    double double_val;
    const char *double_first = s.data();
    const char *double_last = double_first + s.size();
    if (std::from_chars(double_first, double_last, double_val).ec ==
        std::errc()) {
        return true; // Successfully parsed as a double
    }

    return false; // Neither integer nor double
}

std::string utils::str::toLower(const std::string &s) {
    std::string copy(s);

    for (int i = 0; i < (int)copy.length(); i++) {
        copy[i] = tolower(copy[i]);
    }

    return copy;
}

std::vector<std::string> *utils::str::split(const std::string &s, char delim) {
    std::vector<std::string> *result = new std::vector<std::string>;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim)) {
        result->push_back(item);
    }

    return result;
}
