#include "argparser.h"
#include "linkfiles.h"
#include "utils.h"
#include <filesystem>

StringMap parse_args(int argc, char *argv[]) {
  ArgParser parser(argc, argv);

  auto help_ = to_uPtr<CmdArg>("--help", "-h", ArgType::Bool);
  parser.addArg(move_uPtr(help_));

  auto debug_ = to_uPtr<CmdArg>("--debug", "-d", ArgType::Bool);
  parser.addArg(move_uPtr(debug_));

  auto source_ = to_uPtr<CmdArg>("--source", "-s", ArgType::Str);
  parser.addArg(move_uPtr(source_));

  auto output_ = to_uPtr<CmdArg>("--output", "-o", ArgType::Str);
  parser.addArg(move_uPtr(output_));

  auto ignore_ = to_uPtr<CmdArg>("--ignore", "-i", ArgType::Str);
  parser.addArg(move_uPtr(ignore_));

  return parser.parse();
}

int main(int argc, char *argv[]) {
  String sourceDir, destinationDir;
  String ignorePattern = "";
  StringMap cmdArgs = parse_args(argc, argv);

  auto debug = cmdArgs.find("--debug");
  if (NOT(debug == cmdArgs.end())) {
    if (debug->second == "true") {
      setDebugMode(true);
    }
  }

  auto help = cmdArgs.find("--help");
  if (NOT(help == cmdArgs.end())) {
    if (help->second == "true") {
      std::cout << HELP;
      return 0;
    }
  }

  auto source = cmdArgs.find("--source");
  if (NOT(source == cmdArgs.end())) {
    Path absolutePath = fs::absolute(source->second);
    sourceDir = absolutePath.string();
    printLn("Linking from: " + sourceDir);
  }

  auto output = cmdArgs.find("--output");
  if (NOT(output == cmdArgs.end())) {
    Path absolutePath = fs::absolute(output->second);
    destinationDir = absolutePath.string();
    printLn("To: " + destinationDir + "\n");
  }

  auto ignoreKey = cmdArgs.find("--ignore");
  if (NOT(ignoreKey == cmdArgs.end())) {
    ignorePattern = ignoreKey->second;
  }

  if (NOT(sourceDir == "") AND NOT(destinationDir == "")) {
    if (NOT fs::is_directory(sourceDir)) {
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
