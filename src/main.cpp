#include "linkfiles.h"
#include "utils.h"

int main(int argc, char *argv[]) {
  String sourceDir, destinationDir;
  String ignorePattern = "";
  auto cmdArgs = getCmdArgs(argc, argv);

  if (cmdArgs.empty()) {
    std::cout << "No arguments provided\n";
    return 1;
  }

  auto debug = cmdArgs.find("debug");
  if (NOT(debug == cmdArgs.end())) {
    setDebugMode(true);
  }

  auto ignoreKey = cmdArgs.find("ignore");
  if (NOT(ignoreKey == cmdArgs.end())) {
    ignorePattern = ignoreKey->second;
  }

  auto help = cmdArgs.find("help");
  if (NOT(help == cmdArgs.end())) {
    std::cout << HELP;
    return 1;
  }

  auto source = cmdArgs.find("source");
  if (NOT(source == cmdArgs.end())) {
    Path absolutePath = fs::absolute(source->second);
    sourceDir = absolutePath.string();
    printLn("Linking from: " + sourceDir);
  }

  auto output = cmdArgs.find("output");
  if (NOT(output == cmdArgs.end())) {
    Path absolutePath = fs::absolute(output->second);
    destinationDir = absolutePath.string();
    printLn("To: " + destinationDir + "\n");
  }

  if (NOT fs::is_directory(destinationDir)) {
    std::cout << ("Destination directory does not exist or is not a directory");
    return 1;
  }

  if (NOT sourceDir.empty() AND NOT destinationDir.empty()) {
    if (NOT fs::is_directory(sourceDir)) {
      createLink(sourceDir, destinationDir);
    } else {
      linkFiles(sourceDir, destinationDir, false, ignorePattern);
    }
  }

  return 0;
}
