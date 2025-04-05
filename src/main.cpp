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

    int debugLevel = cmdArgs["debug"];
    std::string source = cmdArgs["source"];
    std::string output = cmdArgs["output"];
    std::string ignoreKey = cmdArgs["ignore"];

    logger::LogLevel level = static_cast<logger::LogLevel>(debugLevel);
    LOGGER_SET(level);
    // LOG_WRITE_TO_FILE(true);

    if (source != "") {
        try {
            source = fs::absolute(source).string();
        } catch (fs::filesystem_error &e) {
            LOG_ERROR("Invalid source directory: " + source);
            parser.printHelp();
            return 1;
        }

        LOG_INFO("Linking from: " + source);
    }

    if (output != "") {
        try {
            output = fs::absolute(output).string();
        } catch (fs::filesystem_error &e) {
            LOG_ERROR("Invalid output directory: " + output);
            parser.printHelp();
            return 1;
        }

        LOG_INFO("To: " + output);
    }

    if (ignoreKey != "") {
        LOG_INFO("Ignoring files matching: " + ignoreKey);
    }

    if (source != "" && output != "") {
        if (!fs::is_directory(source)) {
            LOG_DEBUG("Source is not a directory, linking a single file...");
            linker::createLink(source, output);
        } else {
            LOG_DEBUG("Source is a directory, batch linking...");
            linker::linkFiles(source, output, false, ignoreKey);
        }
    } else {
        LOG_ERROR("Source or destination not specified");
        parser.printHelp();
        return 1;
    }

    return 0;
}
