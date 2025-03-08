#include "linker.h"
#include "logger.h"
#include "utils.h"
#include <filesystem>

namespace fs = std::filesystem;

int linker::alreadyLinked(const std::string &WHAT, const std::string &WHERE) {
    LOG_DEBUG("Checking if " + WHAT + " is already linked to " + WHERE);

    try {
        LOG_DEBUG("Scanning directory: " + WHERE);
        for (const auto &entry : fs::directory_iterator(WHERE)) {
            if (fs::is_directory(entry.path())) {
                LOG_DEBUG("Skipping sub-directory: " + entry.path().string());
                continue;
            }

            if (fs::equivalent(WHAT, entry.path().string())) {
                LOG_DEBUG("Found match: " + entry.path().string());
                return 0; // Found a hard link
            }
        }
    } catch (const fs::filesystem_error &e) {
        LOG_ERROR(e.what());
        return -1; // Error
    }

    LOG_DEBUG("No matching links found");
    return 1; // Not found
}

bool linker::createLink(const std::string &WHAT, const std::string &WHERE,
                        bool exist_ok) {
    auto sourcePath = fs::path(WHAT).lexically_normal();
    auto destinationPath = fs::path(WHERE).lexically_normal();
    fs::path linkPath = destinationPath / sourcePath.filename();
    LOG_DEBUG("Linking " + sourcePath.string() + " to " + linkPath.string());

    if (fs::exists(linkPath)) {
        LOG_WARN("File with the same name already exists: " +
                 linkPath.string());
        return false;
    }

    int linkExists = linker::alreadyLinked(WHAT, WHERE);
    if (linkExists == 0) {
        LOG_DEBUG("File is already linked: " + linkPath.string());
        if (!exist_ok) {
            LOG_WARN("File already linked: " + linkPath.string());
            return false; // Already linked, throw error
        } else {
            return true; // Already linked do nothing
        }
    } else if (linkExists == -1) {
        LOG_DEBUG("Could not check for link: " + linkPath.string());
        if (!exist_ok) {
            LOG_ERROR("Error checking for link: " + linkPath.string());
            return false; // Error during existence check, throw error
        }
    }

    try {
        fs::create_hard_link(sourcePath, linkPath);
    } catch (const fs::filesystem_error &e) {
        LOG_ERROR(e.what());
        return false; // Error during link creation
    }

    LOG_DEBUG("Link created: " + linkPath.string());
    return true; // Link created
}

void linker::batchCreateLink(const std::string &FROM, const std::string &TO,
                             int &LINKED, bool exist_ok,
                             const std::string &ignorePattern) {
    fs::path destinationPath(TO);
    LOG_DEBUG("Linking from: " + FROM);

    try {
        int patternPos = -1;
        std::string pattern = "";

        // Parse the ignore pattern
        if (!ignorePattern.empty()) {
            if (utils::str::startswith(ignorePattern, "*")) {
                patternPos = 0;
                pattern = ignorePattern.substr(1);
            } else if (utils::str::endswith(ignorePattern, "*")) {
                patternPos = 1;
                pattern = ignorePattern.substr(0, ignorePattern.length() - 1);
            } else {
                patternPos = 2;
                pattern = ignorePattern;
            }
        }

        switch (patternPos) {
        case 0:
            LOG_DEBUG("Ignoring files that start with: " + pattern);
            break;
        case 1:
            LOG_DEBUG("Ignoring files that end with: " + pattern);
            break;
        case 2:
            LOG_DEBUG("Ignoring files that contain: " + pattern);
            break;
        }

        // Iterate over directory entries
        for (const auto &entry : fs::directory_iterator(FROM)) {
            std::string entryPath = entry.path().string();
            std::string entryName = entry.path().filename().string();
            if (!fs::is_directory(entry.path())) {
                bool ignored = false;
                if ((patternPos == 0 && // If entry name starts with pattern
                     utils::str::startswith(entryName, pattern)) ||
                    (patternPos == 1 && // Or if entry name ends with pattern
                     utils::str::endswith(entryName, pattern)) ||
                    (patternPos == 2 && // Or If entry name contains pattern
                     utils::str::contains(entryName, pattern))) {

                    ignored = true;
                }

                if (ignored) {
                    LOG_INFO("File ignored: " + entryPath);
                    continue;
                }

                if (createLink(entryPath, TO, exist_ok)) {
                    LOG_INFO("File linked: " + entryPath);
                    LINKED++;
                }
            } else { // If entry is a directory
                LOG_DEBUG("Found directory");
                std::string newDestination =
                    (destinationPath / entryName).string();

                if (!fs::exists(newDestination)) {
                    LOG_DEBUG("Creating the same directory at destination");
                    fs::create_directories(newDestination);
                }

                LOG_DEBUG("Recursing into: " + entryPath);
                batchCreateLink(entryPath, newDestination, LINKED, exist_ok,
                                ignorePattern);
            }
        }
    } catch (const fs::filesystem_error &e) {
        LOG_ERROR(e.what());
        return; // File access error
    }
}

int linker::linkFiles(const std::string &FROM, const std::string &TO,
                      bool exist_ok, const std::string &ignorePattern) {
    int linkedCount = 0;
    batchCreateLink(FROM, TO, linkedCount, exist_ok, ignorePattern);

    LOG_INFO("Linked " + std::to_string(linkedCount) + " files.");

    return linkedCount;
}
