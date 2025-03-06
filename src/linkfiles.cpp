#include "linkfiles.h"

// Returns:
// 0 - if `WHAT` is already linked at `WHERE`
// 1 - if `WHAT` is not already linked at `WHERE`
// -1 - if error during comparison occured
int alreadyLinked(const String &WHAT, const String &WHERE) {
  bool errorOccurred = false;

  try {
    for (const auto &entry : fs::directory_iterator(WHERE)) {
      // Skip directories
      if (fs::is_directory(entry.path())) {
        continue;
      }

      try {
        if (fs::equivalent(WHAT, entry.path().string())) {
          return 0; // Found a hard link
        }
      } catch (const fs::filesystem_error &e) {
        printErr("Error comparing \"" + WHAT + "\" with \"" +
                 entry.path().string() + "\"");
        errorOccurred = true;
      }
    }
  } catch (const fs::filesystem_error &e) {
    printErr("Failed to access directory \"" + WHERE + "\": " + e.what());
    return -1;
  }

  // If no match was found, return 1 or return -1 if an error occurred
  return errorOccurred ? -1 : 1;
}

bool createLink(const String &WHAT, const String &WHERE, bool exist_ok) {
  Path sourcePath(WHAT);
  Path destinationPath(WHERE);

  // Normalize paths to handle trailing slashes
  sourcePath = sourcePath.lexically_normal();
  destinationPath = destinationPath.lexically_normal();

  // Construct the link path
  Path linkPath = destinationPath / sourcePath.filename();

  // Delete the existing file with the same name or back it up
  if (fs::exists(linkPath)) {
    printErr("File with the same name already exists - " + linkPath.string());
    return false;
  }

  // Check if the source is already linked at the destination
  int linkCheckResult = alreadyLinked(WHAT, WHERE);
  if (linkCheckResult == 0) {
    if (!exist_ok) {
      printErr("File is already linked - " + WHAT);
      return false;
    } else {
      return true;
    }
  } else if (linkCheckResult == -1) {
    if (!exist_ok) {
      printErr("Failed to check if file is already linked: " + WHAT);
      return false;
    }
  }

  // Attempt to create the hard link
  try {
    fs::create_hard_link(sourcePath, linkPath);
  } catch (const fs::filesystem_error &e) {
    printErr("Failed to create hard link: " + String(e.what()));
    return false;
  }

  return true;
}

void batchCreateLink(const String &FROM, const String &TO, int &LINKED,
                     bool exist_ok, const String &ignorePattern) {
  Path destinationPath(TO);
  try {
    int patternPos = -1;
    String pattern = "";

    // Parse the ignore pattern
    if (!ignorePattern.empty()) {
      if (startswith(ignorePattern, "*")) {
        patternPos = 0;
        pattern = ignorePattern.substr(1);
      } else if (endswith(ignorePattern, "*")) {
        patternPos = 1;
        pattern = ignorePattern.substr(0, ignorePattern.length() - 1);
      } else {
        patternPos = 2;
        pattern = ignorePattern;
      }
    }

    // Iterate over directory entries
    for (const auto &entry : fs::directory_iterator(FROM)) {
      String entryPath = entry.path().string();
      String entryName = entry.path().filename().string();
      if (!fs::is_directory(entry.path())) {
        // Skip files that match the ignore pattern
        bool ignored = false;
        if ((patternPos == 0 && startswith(entryName, pattern)) ||
            (patternPos == 1 && endswith(entryName, pattern)) ||
            (patternPos == 2 && contains(entryName, pattern))) {
          ignored = true;
        }

        if (ignored) {
          sStream message;
          message << "Skipping " << entryName << '\n';
          print(message.str());
          continue;
        }

        // Create a link for a file entry
        if (createLink(entryPath, TO, exist_ok)) {
          // Increment the counter for a successfully created link
          sStream message;
          message << "Linked " << entryName << " to " << TO << '\n';
          print(message.str());
          LINKED++;
        }
      } else {
        // Batch create links for the subdirectory recursively
        Path newDestination = destinationPath / entryName;
        if (!fs::exists(newDestination)) {
          fs::create_directories(newDestination);
        }
        batchCreateLink(entryPath, newDestination.string(), LINKED, exist_ok,
                        ignorePattern);
      }
    }
  } catch (const fs::filesystem_error &e) {
    printErr("Error in batchCreateLink: " + String(e.what()));
    return; // Indicate failure
  }
}

int linkFiles(const String &FROM, const String &TO, bool exist_ok,
              const String &ignorePattern) {
  int linkedCount = 0;
  batchCreateLink(FROM, TO, linkedCount, exist_ok, ignorePattern);

  sStream message;
  message << "\nSuccesfully created links for " << linkedCount << " files\n";

  print(message.str());

  return linkedCount;
}
