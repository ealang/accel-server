#include <iostream>
#include <string.h>

#include "src/server/arg_parse.hh"

CommandLineInput  parseArgs(int argc, char** argv) {
  CommandLineInput input;

  int i = 1;
  while (i < argc) {
    int remaining = argc - i;
    char* token = argv[i];

    if (strcmp(token, "--help") == 0) {
      input.showHelp = true;
      ++i;
      continue;
    } else if (strcmp(token, "--config") == 0 && remaining >= 2) {
      input.configPath = argv[i + 1];
      i += 2;
      continue;
    }
    break;
  }

  if (i < argc) {
    input.showHelp = true;
  }

  return input;
}
