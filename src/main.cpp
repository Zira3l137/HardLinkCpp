#include "argparser.h"
#include "linkfiles.h"
#include "utils.h"
#include <filesystem>

StringMap parse_args(int argc, char *argv[]) {
  ArgParser parser(argc, argv);

  parser.addArg("--help", "-h", ArgType::Bool);
  parser.addArg("--debug", "-d", ArgType::Bool);
  parser.addArg("--source", "-s", ArgType::Str);
  parser.addArg("--output", "-o", ArgType::Str);
  parser.addArg("--ignore", "-i", ArgType::Str);

  return parser.parse();
}

int main(int argc, char *argv[]) {
  String sourceDir, destinationDir;
  String ignorePattern = "";
  StringMap cmdArgs = parse_args(argc, argv);

  auto debug = cmdArgs.find("--debug");
  if (debug != cmdArgs.end()) {
    if (debug->second == "true") {
      setDebugMode(true);
    }
  }

  auto help = cmdArgs.find("--help");
  if (help != cmdArgs.end()) {
    if (help->second == "true") {
      std::cout << HELP;
      return 0;
    }
  }

  auto source = cmdArgs.find("--source");
  if (source != cmdArgs.end()) {
    Path absolutePath = fs::absolute(source->second);
    sourceDir = absolutePath.string();
    printLn("Linking from: " + sourceDir);
  }

  auto output = cmdArgs.find("--output");
  if (output != cmdArgs.end()) {
    Path absolutePath = fs::absolute(output->second);
    destinationDir = absolutePath.string();
    printLn("To: " + destinationDir + "\n");
  }

  auto ignoreKey = cmdArgs.find("--ignore");
  if (ignoreKey != cmdArgs.end()) {
    ignorePattern = ignoreKey->second;
  }

  if (sourceDir != "" && destinationDir != "") {
    if (!fs::is_directory(sourceDir)) {
      createLink(sourceDir, destinationDir);
    } else {
      linkFiles(sourceDir, destinationDir, false, ignorePattern);
    }
  } else {
    std::cout << ("Missing source or destination directory");
    return 1;
  }

  return 0;
}
