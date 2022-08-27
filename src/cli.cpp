#include "cli.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>

#include <getopt.h>

const std::string Config::tag = "[cli][config]";

void Config::updateFromEnvironment(Logger &logger, bool updateLogger){
    // Respect the NO_COLOR unofficial standard. (see https://no-color.org/)
    {
        const char *noColor = std::getenv("NO_COLOR");
        if(noColor && *noColor){ // found && not empty
            useColor = false;
            if(updateLogger)
                logger.enableColor(false);
            logger.quickMessage(
                tag, LogLevel::DEBUG, "Disabling color to respect NO_COLOR."
            );
        }
    }

    // Changing the log level through the environment allows you to change it
    // for option parsing itself. (except for the NO_COLOR message)
    {
        const char *llRaw = std::getenv("VBML_LOG_LEVEL");

        if(llRaw){
            if(parseLogLevel(llRaw, logger, updateLogger)){
                if(updateLogger)
                    logger.changeLogLevel(logLevel);
                logger.startMessage(tag, LogLevel::DEBUG);
                logger.write("Found VBML_LOG_LEVEL=");
                logger.write(llRaw);
                logger.put('.');
                logger.endMessage();
            }
        }
    }
}

// Here we go down to C, the favourite activity of a C++ programmer.
// Note to self: If I ever forget about what is going on here, see these:
// - get_optlong(3) manpage, the NOTES section for setting optind to 0
// - https://www.gnu.org/software/libc/manual/html_node/Getopt.html for the rest
// About Logger::getMessageHeader:
//     I figured out that the glibc error messages do actually have a
//     configurable prefix. The way to do that is to change argv[0]. This
//     solution is rather confusing, but short of writing my own arg parser I
//     don't have a better solution yet.
void Config::updateFromArgs(
    int argc, char **argv, Logger &logger, bool updateLogger
){
    static const struct option longOptions[] = {
        { "project", no_argument, 0, 'p'},
        { "log-level", required_argument, 0, 0 },
        { "output", required_argument, 0, 'o' },
        { "jobs", required_argument, 0, 'j' },
        { "step", required_argument, 0, 0 },
        { 0, 0, 0, 0 }
    };

    // Dark magic close your eyes.
    opterr = logger.getLogLevel() >= LogLevel::ERROR;
    const std::string errorHeader =
        logger.getMessageHeader(LogLevel::ERROR, tag);
    char *const oldArgv0 = argv[0];
    argv[0] = strdup(errorHeader.c_str());

    // Make sure we are starting fresh.
    optind = 0;

    while(true){
        int optionINdex;
        int code = getopt_long(
            argc, argv, "TPGbcCe:Ej:o:psv", longOptions, &optionINdex
        );

        if(code == -1)
            break;

        switch(code){
            case 'T': convertStep = ConvertStep::TOKENIZE; break;
            case 'P': convertStep = ConvertStep::PARSE; break;
            case 'G': convertStep = ConvertStep::GENERATE; break;
            case 'p': projectMode = true; break;
            case 's': projectMode = false; break;
            case 'c':
                useColor = true;
                if(updateLogger)
                    logger.enableColor(useColor);
                break;
            case 'C':
                useColor = false;
                if(updateLogger)
                    logger.enableColor(useColor);
                break;
            case 'v':
                if(logLevel < LogLevel::DEBUG){
                    logLevel = (LogLevel)((int)logLevel + 1);
                    if(updateLogger)
                        logger.changeLogLevel(logLevel);
                }
                break;
            case 'b':
                if(logLevel > LogLevel::ERROR){
                    logLevel = (LogLevel)((int)logLevel - 1);
                    if(updateLogger)
                        logger.changeLogLevel(logLevel);
                }
            case 'o': outputPath = optarg; break;
            case 'j':
                try {
                    int val = std::stoi(optarg);
                    if(val <= 0)
                        throw std::exception();
                    maxJobs = val;
                } catch(const std::exception &e){
                    logger.startMessage(tag, LogLevel::WARNING);
                    logger.write("Invalid value for the job count: ");
                    logger.write(TextColor::STRING, optarg);
                    logger.endMessage();
                }
                break;
            
            case 0:
                switch(optionINdex){
                    case 1: parseLogLevel(optarg, logger, updateLogger); break;
                    case 4: { // step
                        bool valid = false;
                        std::string str = optarg;

                        if(str == "tokenize"){
                            convertStep = ConvertStep::TOKENIZE;
                            valid = true;
                        } else if (str == "parse") {
                            convertStep = ConvertStep::PARSE;
                            valid = true;
                        } else if (str == "generate"){
                            convertStep = ConvertStep::GENERATE;
                            valid = true;
                        }

                        if(!valid){
                            logger.startMessage(tag, LogLevel::WARNING);
                            logger.write("Invalid convert step: ");
                            logger.write(TextColor::STRING, str);
                            logger.endMessage();
                        }

                        break;
                    }
                    default: throw;
                }
                break;
            case '?': break;
            default:
                logger.startMessage(tag, LogLevel::DEBUG);
                logger.write("Unhandled option: ");
                logger.put(TextColor::YELLOW, code);
                logger.endMessage();
                break;
        }
    }

    // Nice and clean, valgrind approves.
    free(argv[0]);
    argv[0] = oldArgv0;
}

bool Config::parseLogLevel(
    const std::string_view &arg, Logger &logger, bool updateLogger
){
    bool updated = false;
    if(arg == "error"){
        updated = true;
        logLevel = LogLevel::ERROR;
    } else if(arg == "warning"){
        updated = true;
        logLevel = LogLevel::WARNING;
    } else if(arg == "info"){
        updated = true;
        logLevel = LogLevel::INFO;
    } else if(arg == "debug"){
        updated = true;
        logLevel = LogLevel::DEBUG;
    }

    if(updated){
        if(updateLogger)
            logger.changeLogLevel(logLevel);
    } else {
        logger.startMessage(tag, LogLevel::WARNING);
        logger.write("Unrecognized value for VBML_LOG_LEVEL: ");
        logger.write(arg);
        logger.endMessage();
    }

    return updated;
}

int runCli(int argc, char **argv){
    Config config;
    Logger logger(config.logLevel, std::clog);

    config.updateFromEnvironment(logger);
    config.detectProjectMode(logger);
    config.updateFromArgs(argc, argv, logger);

    return 0;
}
