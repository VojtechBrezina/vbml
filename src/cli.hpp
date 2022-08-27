#pragma once

#include "logging.hpp"

#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>

enum class ConvertStep {
    TOKENIZE, PARSE, GENERATE
};

const std::filesystem::path PROJECT_FILE_PATH = "./vbml-project";

struct Config {
    bool useColor = true;
    ConvertStep convertStep = ConvertStep::GENERATE;
    bool projectMode = false;

    // An empty path means it wasn't set so project mode can warn about setting
    // it when applicable.
    std::string inputPath = "";
    std::string outputPath = "";

    std::unordered_set<std::string> plugins;
    std::unordered_set<std::string> pluginPaths;

    unsigned maxJobs = 0; // 0 for not forking at all, 1 for foring once (The
                          // original process then won't do any converting)

    // If debug mode is enabled, we would like all the logging by default.
    LogLevel logLevel = 
#       ifdef NDEBUG
            LogLevel::WARNING;
#       else
            LogLevel::DEBUG;
#       endif

    void detectProjectMode(Logger &logger){
        projectMode = std::filesystem::exists(PROJECT_FILE_PATH);
        if(projectMode){
            logger.quickMessage(
                tag, LogLevel::DEBUG, "Detected the project file."
            );
        }
    }

    void updateFromEnvironment(Logger &logger, bool updateLogger = true);

    void updateFromArgs(
        int argc, char **argv, Logger &logger, bool updateLogger = true
    );

    private:
        const static std::string tag;

        bool parseLogLevel(
            const std::string_view &arg, Logger &logger, bool updateLogger
        );
};

int runCli(int argc, char **argv);
