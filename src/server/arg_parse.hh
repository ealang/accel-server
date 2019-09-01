#pragma once

#include <string>

struct CommandLineInput {
  std::string configPath = "src/server/default-config.json";
  bool showHelp = false;
};

CommandLineInput  parseArgs(int argc, char** argv);
