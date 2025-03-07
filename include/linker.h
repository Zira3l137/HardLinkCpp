#pragma once

#include <string>

namespace linker {

int alreadyLinked(const std::string &WHAT, const std::string &WHERE);
bool createLink(const std::string &WHAT, const std::string &WHERE,
                bool exist_ok = false);
void batchCreateLink(const std::string &FROM, const std::string &TO,
                     int &LINKED, bool exist_ok = false,
                     const std::string &ignorePattern = "");
int linkFiles(const std::string &FROM, const std::string &TO,
              bool exist_ok = false, const std::string &ignorePattern = "");

} // namespace linker
