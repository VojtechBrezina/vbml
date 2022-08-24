#include "cli.hpp"

#include <iostream>

#include <stdlib.h>
#include <getopt.h>

void Config::updateFromEnvironment(Logger &logger){
}

void Config::updateFromArgs(int argc, char **argv, Logger &logger){
}

int runCli(int argc, char **argv){
    Config config;
    Logger logger(std::clog, config.loogLevel);



    return 0;
}
