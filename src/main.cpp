#include "argparser.h"
#include "linker.h"
#include "logger.h"
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    argparser::ArgParser parser(argc, argv);

    parser.setDescription("Hard link files from one directory to another.");

    parser.addPosArg("source", argparser::Type::Str,
                     "Source directory to link from. Required.");

    parser.addPosArg("output", argparser::Type::Str,
                     "Destination directory to link to. Required.");

    parser.addOptArg("--ignore", "-i", argparser::Type::Str,
                     "Ignore pattern to exclude files matching that pattern "
                     "from linking. Optional.");

    parser.addOptArg(
        "--debug", "-d", 2,
        "Set log level. [0-4] - DEBUG, INFO, WARN, ERROR, NONE. Optional. "
        "Defaults to 0.");

    auto cmdArgs = parser.parse();

    std::string sourceDir, destinationDir;
    std::string ignorePattern = "";

    int debugLevel = cmdArgs["debug"];
    logger::LogLevel level = static_cast<logger::LogLevel>(debugLevel);
    LOGGER_SET(level);
    // LOG_WRITE_TO_FILE(true);

    std::string source = (cmdArgs["source"]);
    if (source != "") {
        fs::path absolutePath;

        try {
            absolutePath = fs::absolute(source);
        } catch (fs::filesystem_error &e) {
            LOG_ERROR("Invalid source directory: " + source);
            parser.printHelp();
            return 1;
        }

        sourceDir = absolutePath.string();
        LOG_INFO("Linking from: " + sourceDir);
    }

    std::string output = cmdArgs["output"];
    if (output != "") {
        fs::path absolutePath;

        try {
            absolutePath = fs::absolute(output);
        } catch (fs::filesystem_error &e) {
            LOG_ERROR("Invalid output directory: " + output);
            parser.printHelp();
            return 1;
        }

        destinationDir = absolutePath.string();
        LOG_INFO("To: " + destinationDir);
    }

    std::string ignoreKey = cmdArgs["ignore"];
    if (ignoreKey != "") {
        ignorePattern = ignoreKey;
        LOG_INFO("Ignoring files matching: " + ignorePattern);
    }

    if (sourceDir != "" && destinationDir != "") {
        if (!fs::is_directory(sourceDir)) {
            LOG_DEBUG("Source is not a directory, linking a single file...");
            linker::createLink(sourceDir, destinationDir);
        } else {
            LOG_DEBUG("Source is a directory, batch linking...");
            linker::linkFiles(sourceDir, destinationDir, false, ignorePattern);
        }
    } else {
        LOG_ERROR("Source or destination not specified");
        parser.printHelp();
        return 1;
    }

    return 0;
}
