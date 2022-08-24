#pragma once

#include "logging.hpp"

#include <string>
#include <vector>

enum class ConvertStep {
    TOKENIZE, PARSE, GENERATE
};

struct Config {
    bool useColor = true;
    ConvertStep convertStep = ConvertStep::GENERATE;
    bool projectMode = false;

    // An empty path means it wasn't set so project mode can warn about setting
    // it when applicable.
    std::string inputPath = "";
    std::string outputPath = "";

    std::vector<std::string> plugins;

    // If debug mode is enabled, we would like all the logging by default.
    LogLevel logLevel = 
#       ifdef NDEBUG
            LogLevel::WARNING;
#       else
            LogLevel::DEBUG;
#       endif

    void updateFromEnvironment(Logger &logger);

    void updateFromArgs(int argc, char **argv, Logger &logger);
};

int runCli(int argc, char **argv);
