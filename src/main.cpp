#include "argparser.h"
#include "linker.h"
#include "logger.h"
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    argparser::ArgParser parser(argc, argv);

    parser.addArg(
        "--debug", "-d",
        "Set log level. [0-4] - DEBUG, INFO, WARN, ERROR, NONE. Optional. "
        "Defaults to 0.",
        argparser::ArgType::Int);

    parser.addArg("--source", "-s", "Source directory to link from. Required.",
                  argparser::ArgType::Str);

    parser.addArg("--output", "-o",
                  "Destination directory to link to. Required.",
                  argparser::ArgType::Str);

    parser.addArg("--ignore", "-i",
                  "Ignore pattern to exclude files matching that pattern from "
                  "linking. Optional.",
                  argparser::ArgType::Str);

    auto cmdArgs = parser.parse();

    std::string sourceDir, destinationDir;
    std::string ignorePattern = "";

    auto debug = cmdArgs.find("--debug");
    if (debug != cmdArgs.end()) {
        logger::LogLevel level =
            static_cast<logger::LogLevel>(std::stoi(debug->second));
        LOGGER_SET(level);
    }

    auto help = cmdArgs.find("--help");
    if (help != cmdArgs.end()) {
        if (std::string(help->second) == "on") {
            parser.printHelp();
            return 0;
        }
    }

    auto source = cmdArgs.find("--source");
    if (source != cmdArgs.end()) {
        fs::path absolutePath = fs::absolute(source->second);
        sourceDir = absolutePath.string();
        LOG_INFO("Linking from: " + sourceDir);
    }

    auto output = cmdArgs.find("--output");
    if (output != cmdArgs.end()) {
        fs::path absolutePath = fs::absolute(output->second);
        destinationDir = absolutePath.string();
        LOG_INFO("To: " + sourceDir);
    }

    auto ignoreKey = cmdArgs.find("--ignore");
    if (ignoreKey != cmdArgs.end()) {
        ignorePattern = ignoreKey->second;
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
