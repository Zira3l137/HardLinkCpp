#include "utils.h"
#include <cstring>
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

bool utils::str::isallnum(const std::string &s) {
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

char *utils::str::toLower_c(const char *s) {
    char *copy = new char[strlen(s) + 1];

    strcpy(copy, s);
    for (int i = 0; i < strlen(copy); i++) {
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
