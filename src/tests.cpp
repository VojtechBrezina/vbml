#include "tests.hpp"

bool Tester::assert(const std::string &message, bool cond){
    result.total++;

    logger->startMessage(tag, LogLevel::INFO);
    logger->write(message);
    logger->write(": ");

    if(cond){
        logger->setColor(TextColor::GREEN);
        logger->write("OK");
        result.ok++;
    } else {
        logger->setColor(TextColor::RED);
        logger->write("Fail");
    }

    logger->endMessage();

    return cond;
}

bool runTests(){
    const std::string tag = "[tests]";
    Logger logger(LogLevel::DEBUG, std::clog);
    TestResult result;

    logger.quickMessage(tag, LogLevel::INFO, "");
    logger.quickMessage(tag, LogLevel::INFO, "Running tests.");
    logger.quickMessage(tag, LogLevel::INFO, "");

    result += loggingTests(logger);
    result += lexerTests(logger);
    result += loaderTests(logger);

    // Log the final results.

    logger.startMessage(tag, LogLevel::INFO);
    logger.write("Final result: ");
    logger.setColor(
        result.ok == result.total ? TextColor::GREEN : TextColor::RED
    );
    logger.write(std::to_string(result.ok));
    logger.setColor(TextColor::NONE);
    logger.write("/");
    logger.write(std::to_string(result.total));
    logger.endMessage();

    return result.ok == result.total;
}
