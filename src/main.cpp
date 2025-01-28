#include "linkfiles.h"
#include "utils.h"

int main(int argc, char *argv[]) {
  String sourceDir, destinationDir;
  auto cmdArgs = getCmdArgs(argc, argv);

  if (cmdArgs.empty()) {
    printErr("No arguments provided");
    return 0;
  }

  auto help = cmdArgs.find("help");
  if (NOT(help == cmdArgs.end())) {
    printLn(
        "linkfiles -i(--input) INPUT_DIR -o(--output) OUTPUT_DIR [-h(--help)]");
    return 0;
  }

  auto input = cmdArgs.find("input");
  if (NOT(input == cmdArgs.end())) {
    sourceDir = String(input->second);
    printLn("Linking from: " + sourceDir);
  }

  auto output = cmdArgs.find("output");
  if (NOT(output == cmdArgs.end())) {
    destinationDir = String(output->second);
    printLn("To: " + destinationDir);
  }

  if (NOT sourceDir.empty() AND NOT destinationDir.empty()) {
    linkFiles(sourceDir, destinationDir);
  }

  return 0;
}
